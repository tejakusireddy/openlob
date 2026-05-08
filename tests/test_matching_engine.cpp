#include <cassert>
#include <string>

#include "openlob/book/matching_engine.hpp"

int main() {
  const openlob::Instrument instrument{
      .symbol = "MSFT",
      .tick_size = openlob::Price{1},
      .lot_size = openlob::Quantity{100},
  };

  openlob::MatchingEngine engine(instrument);
  assert(engine.instrument().symbol == instrument.symbol);

  // Test 1: duplicate live order is rejected.
  openlob::Order first_live_order{
      .id = openlob::OrderId{1},
      .agent_id = openlob::AgentId{7},
      .symbol = "MSFT",
      .side = openlob::Side::Buy,
      .type = openlob::OrderType::Limit,
      .price = openlob::Price{10000},
      .quantity = openlob::Quantity{5},
      .timestamp = openlob::Timestamp{1},
  };
  const openlob::ExecutionReport first_report = engine.submit_order(first_live_order);
  assert(first_report.rejects.empty());

  openlob::Order duplicate_live_order{
      .id = openlob::OrderId{1},
      .agent_id = openlob::AgentId{9},
      .symbol = "MSFT",
      .side = openlob::Side::Sell,
      .type = openlob::OrderType::Limit,
      .price = openlob::Price{10100},
      .quantity = openlob::Quantity{3},
      .timestamp = openlob::Timestamp{2},
  };
  const openlob::ExecutionReport duplicate_report = engine.submit_order(duplicate_live_order);
  assert(duplicate_report.order_id == openlob::OrderId{1});
  assert(duplicate_report.rejects.size() == 1);
  assert(duplicate_report.rejects[0].order_id == openlob::OrderId{1});
  assert(duplicate_report.rejects[0].timestamp == duplicate_live_order.timestamp);
  assert(duplicate_report.rejects[0].reason.find("duplicate") != std::string::npos);
  assert(duplicate_report.acks.empty());
  assert(duplicate_report.trades.empty());

  // Test 2: duplicate rejection does not mutate book.
  assert(engine.book().contains_order(openlob::OrderId{1}));
  assert(engine.cancel_order(openlob::OrderId{1}));
  assert(!engine.cancel_order(openlob::OrderId{1}));

  // Test 3: reused id after cancel is allowed.
  openlob::Order cancelable_order{
      .id = openlob::OrderId{2},
      .agent_id = openlob::AgentId{8},
      .symbol = "MSFT",
      .side = openlob::Side::Buy,
      .type = openlob::OrderType::Limit,
      .price = openlob::Price{9900},
      .quantity = openlob::Quantity{5},
      .timestamp = openlob::Timestamp{3},
  };
  const openlob::ExecutionReport cancelable_report = engine.submit_order(cancelable_order);
  assert(cancelable_report.rejects.empty());
  assert(engine.cancel_order(openlob::OrderId{2}));

  openlob::Order reused_after_cancel{
      .id = openlob::OrderId{2},
      .agent_id = openlob::AgentId{8},
      .symbol = "MSFT",
      .side = openlob::Side::Sell,
      .type = openlob::OrderType::Limit,
      .price = openlob::Price{10200},
      .quantity = openlob::Quantity{2},
      .timestamp = openlob::Timestamp{4},
  };
  const openlob::ExecutionReport reused_after_cancel_report = engine.submit_order(reused_after_cancel);
  assert(reused_after_cancel_report.rejects.empty());

  // Test 4: reused id after full fill is allowed.
  openlob::Order sell_to_fill{
      .id = openlob::OrderId{10},
      .agent_id = openlob::AgentId{11},
      .symbol = "MSFT",
      .side = openlob::Side::Sell,
      .type = openlob::OrderType::Limit,
      .price = openlob::Price{10000},
      .quantity = openlob::Quantity{5},
      .timestamp = openlob::Timestamp{5},
  };
  const openlob::ExecutionReport sell_report = engine.submit_order(sell_to_fill);
  assert(sell_report.rejects.empty());

  openlob::Order buy_to_fill{
      .id = openlob::OrderId{11},
      .agent_id = openlob::AgentId{12},
      .symbol = "MSFT",
      .side = openlob::Side::Buy,
      .type = openlob::OrderType::Limit,
      .price = openlob::Price{10000},
      .quantity = openlob::Quantity{5},
      .timestamp = openlob::Timestamp{6},
  };
  const openlob::ExecutionReport fill_report = engine.submit_order(buy_to_fill);
  assert(fill_report.rejects.empty());
  assert(fill_report.trades.size() == 1);
  assert(!engine.book().contains_order(openlob::OrderId{10}));

  openlob::Order reused_after_fill{
      .id = openlob::OrderId{10},
      .agent_id = openlob::AgentId{13},
      .symbol = "MSFT",
      .side = openlob::Side::Sell,
      .type = openlob::OrderType::Limit,
      .price = openlob::Price{10100},
      .quantity = openlob::Quantity{1},
      .timestamp = openlob::Timestamp{7},
  };
  const openlob::ExecutionReport reused_after_fill_report = engine.submit_order(reused_after_fill);
  assert(reused_after_fill_report.rejects.empty());

  return 0;
}
