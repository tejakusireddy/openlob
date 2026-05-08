#pragma once

#include "openlob/agents/agent.hpp"

namespace openlob {

// Future: random market/limit order flow generation.
class NoiseTraderAgent : public Agent {
 public:
  explicit NoiseTraderAgent(AgentId id);
  void on_event(const Event& event) override;
};

}  // namespace openlob
