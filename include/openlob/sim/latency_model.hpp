#pragma once

#include "openlob/core/timestamp.hpp"
#include "openlob/core/types.hpp"

namespace openlob {

class LatencyModel {
 public:
  [[nodiscard]] Timestamp order_latency(AgentId agent_id) const;
  [[nodiscard]] Timestamp market_data_latency(AgentId agent_id) const;

  // TODO(openlob): Add configurable per-agent network and exchange delays.
  // TODO(openlob): Support asymmetric uplink/downlink latency and jitter.
};

}  // namespace openlob
