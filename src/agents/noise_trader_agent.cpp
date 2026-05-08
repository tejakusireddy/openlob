#include "openlob/agents/noise_trader_agent.hpp"

namespace openlob {

NoiseTraderAgent::NoiseTraderAgent(AgentId id) : Agent(id) {}

void NoiseTraderAgent::on_event(const Event& /*event*/) {
  // TODO(openlob): Implement stochastic noise trading behavior.
}

}  // namespace openlob
