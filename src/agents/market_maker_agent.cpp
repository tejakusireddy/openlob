#include "openlob/agents/market_maker_agent.hpp"

namespace openlob {

MarketMakerAgent::MarketMakerAgent(AgentId id) : Agent(id) {}

void MarketMakerAgent::on_event(const Event& /*event*/) {
  // TODO(openlob): Implement market maker quoting and inventory controls.
}

}  // namespace openlob
