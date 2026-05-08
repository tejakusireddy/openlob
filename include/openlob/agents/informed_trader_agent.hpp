#pragma once

#include "openlob/agents/agent.hpp"

namespace openlob {

// Future: toxic/adverse selection flow simulation behavior.
class InformedTraderAgent : public Agent {
 public:
  explicit InformedTraderAgent(AgentId id);
  void on_event(const Event& event) override;
};

}  // namespace openlob
