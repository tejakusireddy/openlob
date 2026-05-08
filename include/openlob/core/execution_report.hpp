#pragma once

#include <string>
#include <vector>

#include "openlob/core/timestamp.hpp"
#include "openlob/core/trade.hpp"
#include "openlob/core/types.hpp"

namespace openlob {

struct OrderAck {
  OrderId order_id{};
  Timestamp timestamp{Timestamp::zero()};
};

struct OrderReject {
  OrderId order_id{};
  std::string reason{};
  Timestamp timestamp{Timestamp::zero()};
};

struct ExecutionReport {
  OrderId order_id{};
  std::vector<Trade> trades{};
  std::vector<OrderAck> acks{};
  std::vector<OrderReject> rejects{};
};

// MatchingEngine will later return ExecutionReport synchronously for
// deterministic direct unit testing.
// Simulator will later convert ExecutionReport into asynchronous events
// with latency-aware delivery.

}  // namespace openlob
