#pragma once

#include "openlob/core/types.hpp"

namespace openlob {

struct Instrument {
  Symbol symbol{};
  Price tick_size{};
  Quantity lot_size{};
};

}  // namespace openlob
