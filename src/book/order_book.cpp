#include "openlob/book/order_book.hpp"

#include <iterator>
#include <algorithm>

namespace openlob {

OrderBook::OrderBook(Instrument instrument) : instrument_(std::move(instrument)) {}

const Instrument& OrderBook::instrument() const { return instrument_; }

const Symbol& OrderBook::symbol() const { return instrument_.symbol; }

bool OrderBook::empty() const { return order_index_.empty(); }

std::optional<Price> OrderBook::best_bid() const {
  if (bids_.empty()) {
    return std::nullopt;
  }
  return bids_.begin()->first;
}

std::optional<Price> OrderBook::best_ask() const {
  if (asks_.empty()) {
    return std::nullopt;
  }
  return asks_.begin()->first;
}

std::vector<Trade> OrderBook::add_order(const Order& order) {
  if (order_index_.contains(order.id)) {
    // TODO(openlob): Surface duplicate id as a rejection via ExecutionReport.
    return {};
  }

  Order incoming = order;
  std::vector<Trade> trades;

  auto match_against_asks = [&]() {
    while (incoming.quantity.value > 0 && !asks_.empty()) {
      const bool is_marketable = incoming.type == OrderType::Market || asks_.begin()->first <= incoming.price;
      if (!is_marketable) {
        break;
      }

      auto level_it = asks_.begin();
      auto& level = level_it->second;

      while (incoming.quantity.value > 0 && !level.orders.empty()) {
        auto& resting = level.orders.front();
        const Quantity fill_qty{std::min(incoming.quantity.value, resting.quantity.value)};

        trades.push_back(Trade{
            .buy_order_id = incoming.id,
            .sell_order_id = resting.id,
            .price = resting.price,
            .quantity = fill_qty,
            .timestamp = incoming.timestamp,
        });

        incoming.quantity.value -= fill_qty.value;
        resting.quantity.value -= fill_qty.value;

        if (resting.quantity.value == 0) {
          order_index_.erase(resting.id);
          level.orders.pop_front();
        }
      }

      if (level.empty()) {
        asks_.erase(level_it);
      }
    }
  };

  auto match_against_bids = [&]() {
    while (incoming.quantity.value > 0 && !bids_.empty()) {
      const bool is_marketable = incoming.type == OrderType::Market || bids_.begin()->first >= incoming.price;
      if (!is_marketable) {
        break;
      }

      auto level_it = bids_.begin();
      auto& level = level_it->second;

      while (incoming.quantity.value > 0 && !level.orders.empty()) {
        auto& resting = level.orders.front();
        const Quantity fill_qty{std::min(incoming.quantity.value, resting.quantity.value)};

        trades.push_back(Trade{
            .buy_order_id = resting.id,
            .sell_order_id = incoming.id,
            .price = resting.price,
            .quantity = fill_qty,
            .timestamp = incoming.timestamp,
        });

        incoming.quantity.value -= fill_qty.value;
        resting.quantity.value -= fill_qty.value;

        if (resting.quantity.value == 0) {
          order_index_.erase(resting.id);
          level.orders.pop_front();
        }
      }

      if (level.empty()) {
        bids_.erase(level_it);
      }
    }
  };

  if (incoming.side == Side::Buy) {
    match_against_asks();
  } else {
    match_against_bids();
  }

  if (incoming.type == OrderType::Limit && incoming.quantity.value > 0) {
    if (incoming.side == Side::Buy) {
      auto [level_it, inserted] = bids_.try_emplace(incoming.price, PriceLevel{.price = incoming.price});
      (void)inserted;
      level_it->second.orders.push_back(incoming);
      auto order_it = std::prev(level_it->second.orders.end());
      order_index_.emplace(incoming.id, OrderLocation{
                                            .side = Side::Buy,
                                            .price = incoming.price,
                                            .iterator = order_it,
                                        });
    } else {
      auto [level_it, inserted] = asks_.try_emplace(incoming.price, PriceLevel{.price = incoming.price});
      (void)inserted;
      level_it->second.orders.push_back(incoming);
      auto order_it = std::prev(level_it->second.orders.end());
      order_index_.emplace(incoming.id, OrderLocation{
                                            .side = Side::Sell,
                                            .price = incoming.price,
                                            .iterator = order_it,
                                        });
    }
  }

  return trades;
}

bool OrderBook::cancel_order(OrderId order_id) {
  const auto index_it = order_index_.find(order_id);
  if (index_it == order_index_.end()) {
    return false;
  }

  const OrderLocation location = index_it->second;
  if (location.side == Side::Buy) {
    const auto level_it = bids_.find(location.price);
    if (level_it == bids_.end()) {
      return false;
    }
    level_it->second.orders.erase(location.iterator);
    if (level_it->second.empty()) {
      bids_.erase(level_it);
    }
  } else {
    const auto level_it = asks_.find(location.price);
    if (level_it == asks_.end()) {
      return false;
    }
    level_it->second.orders.erase(location.iterator);
    if (level_it->second.empty()) {
      asks_.erase(level_it);
    }
  }

  order_index_.erase(index_it);
  return true;
}

std::size_t OrderBook::order_count() const { return order_index_.size(); }

std::size_t OrderBook::bid_level_count() const { return bids_.size(); }

std::size_t OrderBook::ask_level_count() const { return asks_.size(); }

}  // namespace openlob
