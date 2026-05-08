#pragma once

#include <list>

#include "openlob/core/order.hpp"
#include "openlob/core/types.hpp"

namespace openlob {

struct PriceLevel {
  Price price{};
  std::list<Order> orders{};

  [[nodiscard]] Quantity total_quantity() const {
    Quantity total{};
    for (const auto& order : orders) {
      total.value += order.quantity.value;
    }
    return total;
  }

  [[nodiscard]] bool empty() const { return orders.empty(); }
};

}  // namespace openlob
