#include "openlob/sim/latency_model.hpp"

namespace openlob {

Timestamp LatencyModel::order_latency(AgentId /*agent_id*/) const { return Timestamp::zero(); }

Timestamp LatencyModel::market_data_latency(AgentId /*agent_id*/) const {
  return Timestamp::zero();
}

}  // namespace openlob
