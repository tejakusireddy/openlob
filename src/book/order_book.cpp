#include "openlob/book/order_book.hpp"

namespace openlob {

OrderBook::OrderBook(Symbol symbol) : symbol_(std::move(symbol)) {}

const Symbol& OrderBook::symbol() const { return symbol_; }

bool OrderBook::empty() const { return live_order_count_ == 0; }

std::optional<Price> OrderBook::best_bid() const { return std::nullopt; }

std::optional<Price> OrderBook::best_ask() const { return std::nullopt; }

std::vector<Trade> OrderBook::add_order(const Order& /*order*/) {
  ++live_order_count_;
  return {};
}

bool OrderBook::cancel_order(OrderId /*order_id*/) {
  if (live_order_count_ == 0) {
    return false;
  }
  --live_order_count_;
  return true;
}

}  // namespace openlob
