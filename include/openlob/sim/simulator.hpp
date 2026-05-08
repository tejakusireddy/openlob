#pragma once

#include <cstddef>

#include "openlob/core/event.hpp"
#include "openlob/core/timestamp.hpp"
#include "openlob/sim/event_queue.hpp"

namespace openlob {

class Simulator {
 public:
  Simulator();

  void schedule_event(Event event);
  void run();

  [[nodiscard]] Timestamp current_time() const;
  [[nodiscard]] std::size_t pending_events() const;
  [[nodiscard]] std::size_t processed_events() const;

 private:
  // Simulator is the deterministic orchestration layer.
  EventQueue event_queue_;
  Timestamp current_time_{Timestamp::zero()};
  std::size_t processed_events_{0};

  // Phase 1 run() only advances time and counts processed events.
  // Later it will route exchange events, agent wakeups, latency-delayed
  // acks/fills/rejects, and market data events.
};

}  // namespace openlob
