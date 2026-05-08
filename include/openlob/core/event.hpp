#pragma once

#include <cstdint>
#include <optional>

#include "openlob/core/order.hpp"
#include "openlob/core/timestamp.hpp"
#include "openlob/core/types.hpp"

namespace openlob {

struct Event {
  Timestamp timestamp{Timestamp::zero()};
  EventType type{EventType::Wakeup};
  AgentId agent_id{};
  std::optional<OrderId> order_id{};
  std::optional<Order> order{};
  std::uint64_t sequence_id{};
};

struct EventComparator {
  bool operator()(const Event& lhs, const Event& rhs) const {
    if (lhs.timestamp != rhs.timestamp) {
      return lhs.timestamp > rhs.timestamp;
    }
    return lhs.sequence_id > rhs.sequence_id;
  }
};

}  // namespace openlob
