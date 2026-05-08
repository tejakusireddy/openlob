#include "openlob/sim/simulator.hpp"

namespace openlob {

Simulator::Simulator() = default;

void Simulator::schedule_event(Event event) { event_queue_.push(std::move(event)); }

void Simulator::run() {
  while (!event_queue_.empty()) {
    auto event = event_queue_.pop();
    if (!event.has_value()) {
      break;
    }
    current_time_ = event->timestamp;

    // TODO(openlob): Dispatch events to exchange and agent handlers.
  }
}

Timestamp Simulator::current_time() const { return current_time_; }

}  // namespace openlob
