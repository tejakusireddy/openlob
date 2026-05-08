#include "openlob/book/order_book.hpp"

#include <iterator>

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

  if (order.side == Side::Buy) {
    auto [level_it, inserted] = bids_.try_emplace(order.price, PriceLevel{.price = order.price});
    (void)inserted;
    level_it->second.orders.push_back(order);
    auto order_it = std::prev(level_it->second.orders.end());
    order_index_.emplace(order.id, OrderLocation{
                                       .side = Side::Buy,
                                       .price = order.price,
                                       .iterator = order_it,
                                   });
  } else {
    auto [level_it, inserted] = asks_.try_emplace(order.price, PriceLevel{.price = order.price});
    (void)inserted;
    level_it->second.orders.push_back(order);
    auto order_it = std::prev(level_it->second.orders.end());
    order_index_.emplace(order.id, OrderLocation{
                                       .side = Side::Sell,
                                       .price = order.price,
                                       .iterator = order_it,
                                   });
  }

  return {};
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
