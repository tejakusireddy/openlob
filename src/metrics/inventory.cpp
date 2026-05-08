#include "openlob/metrics/inventory.hpp"

namespace openlob {

void InventoryTracker::on_fill(Side side, Quantity quantity) {
  if (side == Side::Buy) {
    net_position_ += static_cast<std::int64_t>(quantity.value);
  } else {
    net_position_ -= static_cast<std::int64_t>(quantity.value);
  }
}

std::int64_t InventoryTracker::net_position() const { return net_position_; }

}  // namespace openlob
