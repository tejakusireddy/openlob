#include <cassert>

#include "openlob/book/order_book.hpp"

int main() {
  auto make_order = [](openlob::OrderId id, openlob::Side side, openlob::Price price) {
    return openlob::Order{
        .id = id,
        .agent_id = openlob::AgentId{10},
        .symbol = "AAPL",
        .side = side,
        .type = openlob::OrderType::Limit,
        .price = price,
        .quantity = openlob::Quantity{5},
        .timestamp = openlob::Timestamp{100},
    };
  };

  // Test 1: initial state.
  {
    const openlob::Instrument instrument{
        .symbol = "AAPL",
        .tick_size = openlob::Price{1},
        .lot_size = openlob::Quantity{100},
    };
    openlob::OrderBook book(instrument);
    assert(book.empty());
    assert(!book.best_bid().has_value());
    assert(!book.best_ask().has_value());
    assert(book.order_count() == 0);
  }

  // Test 2: add buy order.
  {
    const openlob::Instrument instrument{
        .symbol = "AAPL",
        .tick_size = openlob::Price{1},
        .lot_size = openlob::Quantity{100},
    };
    openlob::OrderBook book(instrument);
    (void)book.add_order(make_order(openlob::OrderId{1}, openlob::Side::Buy, openlob::Price{100}));
    assert(!book.empty());
    assert(book.best_bid() == openlob::Price{100});
    assert(!book.best_ask().has_value());
    assert(book.order_count() == 1);
  }

  // Test 3: add sell order.
  {
    const openlob::Instrument instrument{
        .symbol = "AAPL",
        .tick_size = openlob::Price{1},
        .lot_size = openlob::Quantity{100},
    };
    openlob::OrderBook book(instrument);
    (void)book.add_order(make_order(openlob::OrderId{1}, openlob::Side::Buy, openlob::Price{100}));
    (void)book.add_order(make_order(openlob::OrderId{2}, openlob::Side::Sell, openlob::Price{105}));
    assert(book.best_ask() == openlob::Price{105});
    assert(book.order_count() == 2);
  }

  // Test 4: best bid/ask ordering.
  {
    const openlob::Instrument instrument{
        .symbol = "AAPL",
        .tick_size = openlob::Price{1},
        .lot_size = openlob::Quantity{100},
    };
    openlob::OrderBook book(instrument);
    (void)book.add_order(make_order(openlob::OrderId{1}, openlob::Side::Buy, openlob::Price{100}));
    (void)book.add_order(make_order(openlob::OrderId{2}, openlob::Side::Buy, openlob::Price{101}));
    (void)book.add_order(make_order(openlob::OrderId{3}, openlob::Side::Buy, openlob::Price{99}));
    assert(book.best_bid() == openlob::Price{101});

    (void)book.add_order(make_order(openlob::OrderId{4}, openlob::Side::Sell, openlob::Price{105}));
    (void)book.add_order(make_order(openlob::OrderId{5}, openlob::Side::Sell, openlob::Price{103}));
    (void)book.add_order(make_order(openlob::OrderId{6}, openlob::Side::Sell, openlob::Price{107}));
    assert(book.best_ask() == openlob::Price{103});
  }

  // Test 5: cancel existing order.
  {
    const openlob::Instrument instrument{
        .symbol = "AAPL",
        .tick_size = openlob::Price{1},
        .lot_size = openlob::Quantity{100},
    };
    openlob::OrderBook book(instrument);
    (void)book.add_order(make_order(openlob::OrderId{1}, openlob::Side::Buy, openlob::Price{100}));
    assert(book.cancel_order(openlob::OrderId{1}));
    assert(book.order_count() == 0);
    assert(book.empty());
  }

  // Test 6: cancel missing order.
  {
    const openlob::Instrument instrument{
        .symbol = "AAPL",
        .tick_size = openlob::Price{1},
        .lot_size = openlob::Quantity{100},
    };
    openlob::OrderBook book(instrument);
    assert(!book.cancel_order(openlob::OrderId{999}));
  }

  // Test 7: price level cleanup.
  const openlob::Instrument instrument{
      .symbol = "AAPL",
      .tick_size = openlob::Price{1},
      .lot_size = openlob::Quantity{100},
  };
  openlob::OrderBook book(instrument);
  (void)book.add_order(make_order(openlob::OrderId{42}, openlob::Side::Buy, openlob::Price{100}));
  assert(book.best_bid() == openlob::Price{100});
  assert(book.cancel_order(openlob::OrderId{42}));
  assert(!book.best_bid().has_value());

  return 0;
}
