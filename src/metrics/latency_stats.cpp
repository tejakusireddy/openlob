#include "openlob/metrics/latency_stats.hpp"

namespace openlob {

void LatencyStats::record(Timestamp latency) {
  ++samples_;
  total_nanoseconds_ += latency.ns;
}

std::size_t LatencyStats::samples() const { return samples_; }

std::int64_t LatencyStats::total_nanoseconds() const { return total_nanoseconds_; }

}  // namespace openlob
