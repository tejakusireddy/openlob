#pragma once

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

 private:
  EventQueue event_queue_;
  Timestamp current_time_{Timestamp::zero()};

  // Simulator is the async boundary that will transform synchronous
  // matching outcomes into latency-aware event delivery.
  // TODO(openlob): Integrate agent wakeups and callbacks.
  // TODO(openlob): Handle exchange-level events and order acknowledgments.
  // TODO(openlob): Publish market data updates to subscribed agents.
  // TODO(openlob): Integrate configurable latency model into event scheduling.
  // TODO(openlob): Add deterministic replay facilities and audit traces.
};

}  // namespace openlob
