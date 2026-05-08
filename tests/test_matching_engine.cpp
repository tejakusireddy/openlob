#include <cassert>

#include "openlob/book/matching_engine.hpp"

int main() {
  openlob::MatchingEngine engine("MSFT");

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

  const auto trades = engine.submit_order(order);
  assert(trades.empty());
  return 0;
}
