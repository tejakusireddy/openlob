#pragma once

#include "openlob/core/timestamp.hpp"
#include "openlob/core/types.hpp"

namespace openlob {

struct Trade {
  OrderId buy_order_id{};
  OrderId sell_order_id{};
  Price price{};
  Quantity quantity{};
  Timestamp timestamp{Timestamp::zero()};
};

}  // namespace openlob
