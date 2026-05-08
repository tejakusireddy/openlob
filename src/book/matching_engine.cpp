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

  if (const std::optional<OrderReject> reject = validate_order(order); reject.has_value()) {
    ExecutionReport invalid_report{};
    invalid_report.order_id = order.id;
    invalid_report.rejects.push_back(reject.value());
    return invalid_report;
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

std::optional<OrderReject> MatchingEngine::validate_order(const Order& order) const {
  if (order.symbol != order_book_.instrument().symbol) {
    return OrderReject{
        .order_id = order.id,
        .reason = "symbol mismatch",
        .timestamp = order.timestamp,
    };
  }

  if (order.quantity.value == 0) {
    return OrderReject{
        .order_id = order.id,
        .reason = "invalid quantity",
        .timestamp = order.timestamp,
    };
  }

  if (order_book_.instrument().lot_size.value == 0) {
    return OrderReject{
        .order_id = order.id,
        .reason = "invalid lot size",
        .timestamp = order.timestamp,
    };
  }

  if (order.quantity.value % order_book_.instrument().lot_size.value != 0) {
    return OrderReject{
        .order_id = order.id,
        .reason = "quantity not aligned to lot size",
        .timestamp = order.timestamp,
    };
  }

  if (order.type == OrderType::Limit) {
    if (order_book_.instrument().tick_size.ticks <= 0) {
      return OrderReject{
          .order_id = order.id,
          .reason = "invalid tick size",
          .timestamp = order.timestamp,
      };
    }

    if (order.price.ticks <= 0) {
      return OrderReject{
          .order_id = order.id,
          .reason = "invalid price",
          .timestamp = order.timestamp,
      };
    }

    if (order.price.ticks % order_book_.instrument().tick_size.ticks != 0) {
      return OrderReject{
          .order_id = order.id,
          .reason = "price not aligned to tick size",
          .timestamp = order.timestamp,
      };
    }
  }

  return std::nullopt;
}

bool MatchingEngine::cancel_order(OrderId order_id) { return order_book_.cancel_order(order_id); }

const Instrument& MatchingEngine::instrument() const { return order_book_.instrument(); }

OrderBook& MatchingEngine::book() { return order_book_; }

const OrderBook& MatchingEngine::book() const { return order_book_; }

}  // namespace openlob
