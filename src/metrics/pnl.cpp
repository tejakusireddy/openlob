#include "openlob/metrics/pnl.hpp"

namespace openlob {

void PnLTracker::on_trade(const Trade& /*trade*/) {
  // TODO(openlob): Replace with real accounting once fill attribution is modeled.
}

std::int64_t PnLTracker::realized_pnl_ticks() const { return realized_pnl_ticks_; }

}  // namespace openlob
