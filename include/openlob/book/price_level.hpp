#pragma once

#include "openlob/core/types.hpp"

namespace openlob {

struct PriceLevel {
  Price price{};
  Quantity aggregate_quantity{};

  // TODO(openlob): Maintain FIFO queue of orders at this price.
  // TODO(openlob): Keep aggregate quantity synchronized with queue updates.
  // TODO(openlob): Add queue-position tracking utilities.
};

}  // namespace openlob
