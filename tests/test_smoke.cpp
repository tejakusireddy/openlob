#include <cassert>

#include "openlob/core/execution_report.hpp"
#include "openlob/core/instrument.hpp"
#include "openlob/core/order.hpp"
#include "openlob/core/timestamp.hpp"
#include "openlob/core/trade.hpp"
#include "openlob/core/types.hpp"

int main() {
  const openlob::Price price{10025};
  const openlob::Quantity quantity{42};
  const openlob::Timestamp t0 = openlob::Timestamp::zero();
  const openlob::Timestamp t1{150};
  const openlob::OrderId order_id{123};
  const openlob::AgentId agent_id{7};

  assert(price.ticks == 10025);
  assert(quantity.value == 42);
  assert(t0.ns == 0);
  assert(order_id.value == 123);
  assert(agent_id.value == 7);

  const openlob::Timestamp t2 = t1 + 50;
  const openlob::Timestamp t3 = t2 - 20;
  assert(t2.ns == 200);
  assert(t3.ns == 180);

  const openlob::Instrument instrument{
      .symbol = "AAPL",
      .tick_size = openlob::Price{1},
      .lot_size = openlob::Quantity{100},
  };
  assert(instrument.symbol == "AAPL");
  assert(instrument.tick_size.ticks == 1);
  assert(instrument.lot_size.value == 100);

  const openlob::Order order{
      .id = order_id,
      .agent_id = agent_id,
      .symbol = instrument.symbol,
      .side = openlob::Side::Buy,
      .type = openlob::OrderType::Limit,
      .price = price,
      .quantity = quantity,
      .timestamp = t1,
  };
  assert(order.id.value == 123);
  assert(order.agent_id.value == 7);
  assert(order.price.ticks == 10025);

  const openlob::Trade trade{
      .buy_order_id = openlob::OrderId{1},
      .sell_order_id = openlob::OrderId{2},
      .price = openlob::Price{10030},
      .quantity = openlob::Quantity{5},
      .timestamp = openlob::Timestamp{200},
  };
  assert(trade.price.ticks == 10030);
  assert(trade.quantity.value == 5);

  openlob::ExecutionReport report{.order_id = order_id};
  assert(report.order_id.value == 123);
  assert(report.trades.empty());
  assert(report.acks.empty());
  assert(report.rejects.empty());

  return 0;
}
