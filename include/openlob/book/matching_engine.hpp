#pragma once

#include <vector>

#include "openlob/book/order_book.hpp"
#include "openlob/core/order.hpp"
#include "openlob/core/trade.hpp"

namespace openlob {

class MatchingEngine {
 public:
  explicit MatchingEngine(Symbol symbol);

  // MatchingEngine is called only by Simulator.
  // Agents must interact through Simulator/EventQueue so latency,
  // acknowledgments, fills, and rejects remain realistic.
  std::vector<Trade> submit_order(const Order& order);
  bool cancel_order(OrderId order_id);

  OrderBook& book();
  const OrderBook& book() const;

 private:
  OrderBook order_book_;

  // TODO(openlob): Implement price-time priority matching behavior.
  // TODO(openlob): Implement market order handling and marketable limit logic.
};

}  // namespace openlob
