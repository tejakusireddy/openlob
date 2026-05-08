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

  openlob::Order order{
      .id = openlob::OrderId{1},
      .agent_id = openlob::AgentId{7},
      .symbol = "MSFT",
      .side = openlob::Side::Buy,
      .type = openlob::OrderType::Limit,
      .price = openlob::Price{10000},
      .quantity = openlob::Quantity{10},
      .timestamp = openlob::Timestamp{1},
  };

  const openlob::ExecutionReport report = engine.submit_order(order);
  assert(report.order_id == order.id);
  assert(report.trades.empty());
  assert(report.acks.size() == 1);
  assert(report.acks.front().order_id == order.id);
  assert(report.acks.front().timestamp == order.timestamp);
  assert(report.rejects.empty());
  return 0;
}
