#include "openlob/book/order_book.hpp"

namespace openlob {

OrderBook::OrderBook(Instrument instrument) : instrument_(std::move(instrument)) {}

const Instrument& OrderBook::instrument() const { return instrument_; }

const Symbol& OrderBook::symbol() const { return instrument_.symbol; }

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
