#include <cassert>

#include "openlob/book/matching_engine.hpp"
#include "openlob/sim/simulator.hpp"

int main() {
  const openlob::Instrument instrument{
      .symbol = "TEST",
      .tick_size = openlob::Price{1},
      .lot_size = openlob::Quantity{1},
  };

  openlob::MatchingEngine engine(instrument);

  const openlob::Order order{
      .id = openlob::OrderId{1},
      .agent_id = openlob::AgentId{10},
      .symbol = "TEST",
      .side = openlob::Side::Buy,
      .type = openlob::OrderType::Limit,
      .price = openlob::Price{100},
      .quantity = openlob::Quantity{5},
      .timestamp = openlob::Timestamp{100},
  };

  const openlob::ExecutionReport report = engine.submit_order(order);
  assert(report.order_id == order.id);
  assert(report.acks.size() == 1);
  assert(report.acks.front().order_id == order.id);
  assert(report.acks.front().timestamp == order.timestamp);
  assert(report.rejects.empty());

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

  assert(sim.pending_events() == 3);
  assert(sim.processed_events() == 0);

  sim.run();

  assert(sim.pending_events() == 0);
  assert(sim.processed_events() == 3);
  assert(sim.current_time() == openlob::Timestamp{300});

  return 0;
}
