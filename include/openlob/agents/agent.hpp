#pragma once

#include "openlob/core/event.hpp"
#include "openlob/core/types.hpp"

namespace openlob {

class Agent {
 public:
  explicit Agent(AgentId id) : id_(id) {}
  virtual ~Agent() = default;

  [[nodiscard]] AgentId id() const { return id_; }
  virtual void on_event(const Event& event) = 0;
  // Agents must never call MatchingEngine directly; they interact only
  // through Simulator-managed events.

 private:
  AgentId id_{};
};

}  // namespace openlob
