#pragma once

#include "openlob/core/timestamp.hpp"
#include "openlob/core/types.hpp"

namespace openlob {

struct Order {
  OrderId id{};
  AgentId agent_id{};
  Symbol symbol{};
  Side side{Side::Buy};
  OrderType type{OrderType::Limit};
  Price price{};
  Quantity quantity{};
  Timestamp timestamp{Timestamp::zero()};

  // TODO(openlob): Add remaining quantity for partial-fill support.
  // TODO(openlob): Add order status lifecycle (active, filled, canceled, rejected).
  // TODO(openlob): Track queue position metadata for microstructure analysis.
  // TODO(openlob): Add time-in-force semantics (GTC/IOC/FOK/etc.).
};

}  // namespace openlob
