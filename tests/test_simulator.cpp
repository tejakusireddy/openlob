#include <cassert>

#include "openlob/sim/simulator.hpp"

int main() {
  // Test 1: initial state.
  {
    openlob::Simulator sim;
    assert(sim.current_time() == openlob::Timestamp::zero());
    assert(sim.pending_events() == 0);
    assert(sim.processed_events() == 0);
  }

  // Test 2: schedule_event changes pending count.
  {
    openlob::Simulator sim;
    sim.schedule_event(openlob::Event{
        .timestamp = openlob::Timestamp{100},
        .type = openlob::EventType::Wakeup,
        .agent_id = openlob::AgentId{1},
    });
    assert(sim.pending_events() == 1);
    assert(sim.processed_events() == 0);
  }

  // Test 3: run processes all events.
  {
    openlob::Simulator sim;
    sim.schedule_event(openlob::Event{
        .timestamp = openlob::Timestamp{300},
        .type = openlob::EventType::Wakeup,
        .agent_id = openlob::AgentId{1},
    });
    sim.schedule_event(openlob::Event{
        .timestamp = openlob::Timestamp{100},
        .type = openlob::EventType::Wakeup,
        .agent_id = openlob::AgentId{2},
    });
    sim.schedule_event(openlob::Event{
        .timestamp = openlob::Timestamp{200},
        .type = openlob::EventType::Wakeup,
        .agent_id = openlob::AgentId{3},
    });
    sim.run();
    assert(sim.pending_events() == 0);
    assert(sim.processed_events() == 3);
  }

  // Test 4: current_time advances to last processed timestamp.
  {
    openlob::Simulator sim;
    sim.schedule_event(openlob::Event{
        .timestamp = openlob::Timestamp{300},
        .type = openlob::EventType::Wakeup,
        .agent_id = openlob::AgentId{1},
    });
    sim.schedule_event(openlob::Event{
        .timestamp = openlob::Timestamp{100},
        .type = openlob::EventType::Wakeup,
        .agent_id = openlob::AgentId{2},
    });
    sim.schedule_event(openlob::Event{
        .timestamp = openlob::Timestamp{200},
        .type = openlob::EventType::Wakeup,
        .agent_id = openlob::AgentId{3},
    });
    sim.run();
    assert(sim.current_time() == openlob::Timestamp{300});
  }

  // Test 5: empty run is safe.
  {
    openlob::Simulator sim;
    sim.run();
    assert(sim.current_time() == openlob::Timestamp::zero());
    assert(sim.processed_events() == 0);
  }

  return 0;
}
