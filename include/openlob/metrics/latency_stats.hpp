#pragma once

#include <cstddef>
#include <cstdint>

#include "openlob/core/timestamp.hpp"

namespace openlob {

class LatencyStats {
 public:
  void record(Timestamp latency);
  [[nodiscard]] std::size_t samples() const;
  [[nodiscard]] std::int64_t total_nanoseconds() const;

 private:
  std::size_t samples_{0};
  std::int64_t total_nanoseconds_{0};

  // TODO(openlob): Add percentile metrics (p50/p95/p99).
  // TODO(openlob): Segment latency stats by event type and agent.
};

}  // namespace openlob
