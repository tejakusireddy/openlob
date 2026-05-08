#pragma once

#include "openlob/agents/agent.hpp"

namespace openlob {

// Future: bid/ask quoting, spread capture, and inventory control logic.
class MarketMakerAgent : public Agent {
 public:
  explicit MarketMakerAgent(AgentId id);
  void on_event(const Event& event) override;
};

}  // namespace openlob
