#include "openlob/sim/event_queue.hpp"

namespace openlob {

void EventQueue::push(Event event) {
  if (event.sequence_id == 0) {
    event.sequence_id = ++next_sequence_id_;
  }
  queue_.push(std::move(event));
}

std::optional<Event> EventQueue::pop() {
  if (queue_.empty()) {
    return std::nullopt;
  }

  Event top = queue_.top();
  queue_.pop();
  return top;
}

bool EventQueue::empty() const { return queue_.empty(); }

std::size_t EventQueue::size() const { return queue_.size(); }

}  // namespace openlob
