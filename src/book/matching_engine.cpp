#include "openlob/book/matching_engine.hpp"

namespace openlob {

MatchingEngine::MatchingEngine(Instrument instrument) : order_book_(std::move(instrument)) {}

ExecutionReport MatchingEngine::submit_order(const Order& order) {
  // Phase 1 duplicate detection checks only currently live order IDs in-book.
  // Cross-session/global uniqueness is deferred to future gateway/session layers.
  if (order_book_.contains_order(order.id)) {
    ExecutionReport duplicate_report{};
    duplicate_report.order_id = order.id;
    duplicate_report.rejects.push_back(OrderReject{
        .order_id = order.id,
        .reason = "duplicate order id",
        .timestamp = order.timestamp,
    });
    return duplicate_report;
  }

  ExecutionReport report{};
  report.order_id = order.id;
  report.trades = order_book_.add_order(order);
  report.acks.push_back(OrderAck{
      .order_id = order.id,
      .timestamp = order.timestamp,
  });
  return report;
}

bool MatchingEngine::cancel_order(OrderId order_id) { return order_book_.cancel_order(order_id); }

const Instrument& MatchingEngine::instrument() const { return order_book_.instrument(); }

OrderBook& MatchingEngine::book() { return order_book_; }

const OrderBook& MatchingEngine::book() const { return order_book_; }

}  // namespace openlob
