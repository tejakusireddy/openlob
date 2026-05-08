#include <cassert>

#include "openlob/sim/event_queue.hpp"

int main() {
  // Test 1: timestamp ordering (earlier first).
  {
    openlob::EventQueue queue;
    queue.push(openlob::Event{
        .timestamp = openlob::Timestamp{200},
        .type = openlob::EventType::Wakeup,
        .agent_id = openlob::AgentId{1},
    });
    queue.push(openlob::Event{
        .timestamp = openlob::Timestamp{100},
        .type = openlob::EventType::Wakeup,
        .agent_id = openlob::AgentId{2},
    });

    const auto first = queue.pop();
    const auto second = queue.pop();
    assert(first.has_value());
    assert(second.has_value());
    assert(first->timestamp.ns == 100);
    assert(second->timestamp.ns == 200);
  }

  // Test 2: same timestamp FIFO by queue-assigned sequence_id.
  {
    openlob::EventQueue queue;
    queue.push(openlob::Event{
        .timestamp = openlob::Timestamp{100},
        .type = openlob::EventType::Wakeup,
        .agent_id = openlob::AgentId{11},
    });
    queue.push(openlob::Event{
        .timestamp = openlob::Timestamp{100},
        .type = openlob::EventType::Wakeup,
        .agent_id = openlob::AgentId{22},
    });
    queue.push(openlob::Event{
        .timestamp = openlob::Timestamp{100},
        .type = openlob::EventType::Wakeup,
        .agent_id = openlob::AgentId{33},
    });

    const auto first = queue.pop();
    const auto second = queue.pop();
    const auto third = queue.pop();
    assert(first.has_value());
    assert(second.has_value());
    assert(third.has_value());
    assert(first->sequence_id < second->sequence_id);
    assert(second->sequence_id < third->sequence_id);
    assert(first->agent_id.value == 11);
    assert(second->agent_id.value == 22);
    assert(third->agent_id.value == 33);
  }

  // Test 3: caller-provided sequence_id is overwritten by queue assignment.
  {
    openlob::EventQueue queue;
    queue.push(openlob::Event{
        .timestamp = openlob::Timestamp{100},
        .type = openlob::EventType::Wakeup,
        .agent_id = openlob::AgentId{7},
        .sequence_id = 9999,
    });
    const auto popped = queue.pop();
    assert(popped.has_value());
    assert(popped->sequence_id != 9999);
    assert(popped->sequence_id == 1);
  }

  // Test 4: size/empty behavior.
  {
    openlob::EventQueue queue;
    assert(queue.empty());
    assert(queue.size() == 0);

    queue.push(openlob::Event{
        .timestamp = openlob::Timestamp{123},
        .type = openlob::EventType::Wakeup,
        .agent_id = openlob::AgentId{99},
    });
    assert(!queue.empty());
    assert(queue.size() == 1);

    const auto popped = queue.pop();
    assert(popped.has_value());
    assert(queue.empty());
    assert(queue.size() == 0);
  }

  return 0;
}
