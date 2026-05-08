#pragma once

#include <cstdint>

#include "openlob/core/trade.hpp"

namespace openlob {

class PnLTracker {
 public:
  void on_trade(const Trade& trade);
  [[nodiscard]] std::int64_t realized_pnl_ticks() const;

 private:
  std::int64_t realized_pnl_ticks_{0};

  // TODO(openlob): Add mark-to-market and unrealized PnL components.
  // TODO(openlob): Attribute PnL to spread capture vs adverse selection.
};

}  // namespace openlob
