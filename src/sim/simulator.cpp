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
    ++processed_events_;

    // TODO(openlob): Dispatch events to exchange and agent handlers.
  }
}

Timestamp Simulator::current_time() const { return current_time_; }

std::size_t Simulator::pending_events() const { return event_queue_.size(); }

std::size_t Simulator::processed_events() const { return processed_events_; }

}  // namespace openlob
