#include "openlob/agents/informed_trader_agent.hpp"

namespace openlob {

InformedTraderAgent::InformedTraderAgent(AgentId id) : Agent(id) {}

void InformedTraderAgent::on_event(const Event& /*event*/) {
  // TODO(openlob): Implement informed flow and adverse-selection pressure.
}

}  // namespace openlob
