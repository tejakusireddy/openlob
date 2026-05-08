#pragma once

#include <cstdint>

#include "openlob/core/types.hpp"

namespace openlob {

class InventoryTracker {
 public:
  void on_fill(Side side, Quantity quantity);
  [[nodiscard]] std::int64_t net_position() const;

 private:
  std::int64_t net_position_{0};

  // TODO(openlob): Add per-symbol and per-agent inventory accounting.
  // TODO(openlob): Add exposure and risk-limit checks.
};

}  // namespace openlob
