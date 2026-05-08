#include <cassert>

#include "openlob/book/matching_engine.hpp"

int main() {
  const openlob::Instrument instrument{
      .symbol = "MSFT",
      .tick_size = openlob::Price{1},
      .lot_size = openlob::Quantity{100},
  };

  openlob::MatchingEngine engine(instrument);
  assert(engine.instrument().symbol == instrument.symbol);

  // Non-crossing order still yields placeholder ack and no trades.
  openlob::Order resting_order{
      .id = openlob::OrderId{1},
      .agent_id = openlob::AgentId{7},
      .symbol = "MSFT",
      .side = openlob::Side::Sell,
      .type = openlob::OrderType::Limit,
      .price = openlob::Price{10000},
      .quantity = openlob::Quantity{10},
      .timestamp = openlob::Timestamp{1},
  };

  const openlob::ExecutionReport resting_report = engine.submit_order(resting_order);
  assert(resting_report.order_id == resting_order.id);
  assert(resting_report.trades.empty());
  assert(resting_report.acks.size() == 1);
  assert(resting_report.acks.front().order_id == resting_order.id);
  assert(resting_report.acks.front().timestamp == resting_order.timestamp);
  assert(resting_report.rejects.empty());

  // Crossing order should produce a trade in ExecutionReport.
  openlob::Order incoming_order{
      .id = openlob::OrderId{2},
      .agent_id = openlob::AgentId{8},
      .symbol = "MSFT",
      .side = openlob::Side::Buy,
      .type = openlob::OrderType::Limit,
      .price = openlob::Price{10000},
      .quantity = openlob::Quantity{10},
      .timestamp = openlob::Timestamp{2},
  };
  const openlob::ExecutionReport crossing_report = engine.submit_order(incoming_order);
  assert(crossing_report.order_id == incoming_order.id);
  assert(crossing_report.trades.size() == 1);
  assert(crossing_report.trades.front().price == openlob::Price{10000});
  assert(crossing_report.trades.front().quantity == openlob::Quantity{10});
  assert(crossing_report.acks.size() == 1);
  assert(crossing_report.acks.front().order_id == incoming_order.id);
  assert(crossing_report.acks.front().timestamp == incoming_order.timestamp);
  assert(crossing_report.rejects.empty());
  return 0;
}
