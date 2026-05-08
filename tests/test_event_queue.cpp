#include <cassert>

#include "openlob/sim/event_queue.hpp"

int main() {
  openlob::EventQueue queue;

  queue.push(openlob::Event{
      .timestamp = openlob::Timestamp{20},
      .type = openlob::EventType::Wakeup,
      .agent_id = openlob::AgentId{1},
  });
  queue.push(openlob::Event{
      .timestamp = openlob::Timestamp{10},
      .type = openlob::EventType::Wakeup,
      .agent_id = openlob::AgentId{2},
  });

  auto first = queue.pop();
  auto second = queue.pop();

  assert(first.has_value());
  assert(second.has_value());
  assert(first->timestamp.ns <= second->timestamp.ns);

  return 0;
}
