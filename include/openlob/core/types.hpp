#pragma once

#include <compare>
#include <cstdint>
#include <string>

namespace openlob {

enum class Side { Buy, Sell };

enum class OrderType { Limit, Market };

enum class EventType { AddOrder, CancelOrder, MarketOrder, Trade, Wakeup, Ack, Reject };

// Price is stored in integer ticks to avoid floating-point precision drift.
// This keeps matching, PnL, and replay behavior deterministic.
struct Price {
  std::int64_t ticks{0};
  auto operator<=>(const Price&) const = default;
};

struct Quantity {
  std::uint64_t value{0};
  auto operator<=>(const Quantity&) const = default;
};

struct OrderId {
  std::uint64_t value{0};
  auto operator<=>(const OrderId&) const = default;
};

struct AgentId {
  std::uint64_t value{0};
  auto operator<=>(const AgentId&) const = default;
};

// Strong wrappers prevent accidental mixing of market-domain values
// such as Price, Timestamp, OrderId, and AgentId.
using Symbol = std::string;

}  // namespace openlob
