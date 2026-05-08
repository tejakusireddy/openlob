#pragma once

#include <optional>
#include <vector>

#include "openlob/core/instrument.hpp"
#include "openlob/core/order.hpp"
#include "openlob/core/trade.hpp"
#include "openlob/core/types.hpp"

namespace openlob {

class OrderBook {
 public:
  explicit OrderBook(Instrument instrument);

  [[nodiscard]] const Instrument& instrument() const;
  [[nodiscard]] const Symbol& symbol() const;
  [[nodiscard]] bool empty() const;
  [[nodiscard]] std::optional<Price> best_bid() const;
  [[nodiscard]] std::optional<Price> best_ask() const;

  std::vector<Trade> add_order(const Order& order);
  bool cancel_order(OrderId order_id);

 private:
  Instrument instrument_;
  std::size_t live_order_count_{0};

  // TODO(openlob): Implement full L3 price-time priority model.
  // TODO(openlob): Add bid/ask maps keyed by price.
  // TODO(openlob): Add per-price FIFO queues for resting orders.
  // TODO(openlob): Add order-id lookup for cancel/replace operations.
  // TODO(openlob): Implement partial fills and residual order handling.
  // TODO(openlob): Track queue position transitions at each level.
};

}  // namespace openlob
