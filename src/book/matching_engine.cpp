#include "openlob/book/matching_engine.hpp"

namespace openlob {

MatchingEngine::MatchingEngine(Symbol symbol) : order_book_(std::move(symbol)) {}

std::vector<Trade> MatchingEngine::submit_order(const Order& order) {
  return order_book_.add_order(order);
}

bool MatchingEngine::cancel_order(OrderId order_id) { return order_book_.cancel_order(order_id); }

OrderBook& MatchingEngine::book() { return order_book_; }

const OrderBook& MatchingEngine::book() const { return order_book_; }

}  // namespace openlob
