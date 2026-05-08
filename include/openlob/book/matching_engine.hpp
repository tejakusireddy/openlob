#pragma once

#include <optional>

#include "openlob/book/order_book.hpp"
#include "openlob/core/instrument.hpp"
#include "openlob/core/execution_report.hpp"
#include "openlob/core/order.hpp"

namespace openlob {

class MatchingEngine {
 public:
  explicit MatchingEngine(Instrument instrument);

  // MatchingEngine is synchronous for deterministic matching and direct unit testing.
  // It must only be called by Simulator/exchange plumbing.
  // Agents must interact through Simulator/EventQueue so latency, acknowledgments,
  // fills, and rejects remain realistic.
  ExecutionReport submit_order(const Order& order);
  bool cancel_order(OrderId order_id);

  [[nodiscard]] const Instrument& instrument() const;
  OrderBook& book();
  const OrderBook& book() const;

 private:
  [[nodiscard]] std::optional<OrderReject> validate_order(const Order& order) const;

  OrderBook order_book_;

  // TODO(openlob): Implement price-time priority matching behavior.
  // TODO(openlob): Implement market order handling and marketable limit logic.
};

}  // namespace openlob
