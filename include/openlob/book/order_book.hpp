#pragma once

#include <functional>
#include <map>
#include <optional>
#include <unordered_map>
#include <vector>

#include "openlob/book/price_level.hpp"
#include "openlob/core/instrument.hpp"
#include "openlob/core/order.hpp"
#include "openlob/core/trade.hpp"
#include "openlob/core/types.hpp"

namespace openlob {

class OrderBook {
 public:
  struct OrderIdHash {
    std::size_t operator()(const OrderId& id) const noexcept {
      return std::hash<std::uint64_t>{}(id.value);
    }
  };

  struct OrderLocation {
    Side side{};
    Price price{};
    std::list<Order>::iterator iterator{};
  };

  explicit OrderBook(Instrument instrument);

  [[nodiscard]] const Instrument& instrument() const;
  [[nodiscard]] const Symbol& symbol() const;
  [[nodiscard]] bool empty() const;
  [[nodiscard]] std::optional<Price> best_bid() const;
  [[nodiscard]] std::optional<Price> best_ask() const;

  std::vector<Trade> add_order(const Order& order);
  bool cancel_order(OrderId order_id);
  [[nodiscard]] std::size_t order_count() const;
  [[nodiscard]] std::size_t bid_level_count() const;
  [[nodiscard]] std::size_t ask_level_count() const;

 private:
  Instrument instrument_;
  std::map<Price, PriceLevel, std::greater<Price>> bids_{};
  std::map<Price, PriceLevel, std::less<Price>> asks_{};
  std::unordered_map<OrderId, OrderLocation, OrderIdHash> order_index_{};

  // TODO(openlob): Add duplicate OrderId rejection via ExecutionReport path.
  // TODO(openlob): Implement full price-time priority matching behavior.
  // TODO(openlob): Implement partial fills and residual order handling.
  // TODO(openlob): Track queue position transitions at each level.
};

}  // namespace openlob
