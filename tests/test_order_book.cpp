#include <cassert>

#include "openlob/book/order_book.hpp"

int main() {
  auto make_order = [](openlob::OrderId id,
                       openlob::Side side,
                       openlob::Price price,
                       openlob::Quantity quantity = openlob::Quantity{5},
                       openlob::OrderType type = openlob::OrderType::Limit) {
    return openlob::Order{
        .id = id,
        .agent_id = openlob::AgentId{10},
        .symbol = "AAPL",
        .side = side,
        .type = type,
        .price = price,
        .quantity = quantity,
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

  // Test 4: buy crosses resting sell.
  {
    const openlob::Instrument instrument{
        .symbol = "AAPL",
        .tick_size = openlob::Price{1},
        .lot_size = openlob::Quantity{100},
    };
    openlob::OrderBook book(instrument);
    (void)book.add_order(make_order(openlob::OrderId{1},
                                    openlob::Side::Sell,
                                    openlob::Price{100},
                                    openlob::Quantity{5}));
    const auto trades = book.add_order(make_order(openlob::OrderId{2},
                                                   openlob::Side::Buy,
                                                   openlob::Price{101},
                                                   openlob::Quantity{5}));
    assert(trades.size() == 1);
    assert(trades[0].price == openlob::Price{100});
    assert(trades[0].quantity == openlob::Quantity{5});
    assert(book.empty());
  }

  // Test 5: sell crosses resting buy.
  {
    const openlob::Instrument instrument{
        .symbol = "AAPL",
        .tick_size = openlob::Price{1},
        .lot_size = openlob::Quantity{100},
    };
    openlob::OrderBook book(instrument);
    (void)book.add_order(make_order(openlob::OrderId{1},
                                    openlob::Side::Buy,
                                    openlob::Price{100},
                                    openlob::Quantity{5}));
    const auto trades = book.add_order(make_order(openlob::OrderId{2},
                                                   openlob::Side::Sell,
                                                   openlob::Price{99},
                                                   openlob::Quantity{5}));
    assert(trades.size() == 1);
    assert(trades[0].price == openlob::Price{100});
    assert(trades[0].quantity == openlob::Quantity{5});
    assert(book.empty());
  }

  // Test 6: FIFO within same price.
  {
    const openlob::Instrument instrument{
        .symbol = "AAPL",
        .tick_size = openlob::Price{1},
        .lot_size = openlob::Quantity{100},
    };
    openlob::OrderBook book(instrument);
    (void)book.add_order(make_order(openlob::OrderId{1},
                                    openlob::Side::Sell,
                                    openlob::Price{100},
                                    openlob::Quantity{5}));
    (void)book.add_order(make_order(openlob::OrderId{2},
                                    openlob::Side::Sell,
                                    openlob::Price{100},
                                    openlob::Quantity{5}));
    const auto trades = book.add_order(make_order(openlob::OrderId{3},
                                                   openlob::Side::Buy,
                                                   openlob::Price{100},
                                                   openlob::Quantity{7}));
    assert(trades.size() == 2);
    assert(trades[0].sell_order_id == openlob::OrderId{1});
    assert(trades[0].quantity == openlob::Quantity{5});
    assert(trades[1].sell_order_id == openlob::OrderId{2});
    assert(trades[1].quantity == openlob::Quantity{2});
    assert(book.best_ask() == openlob::Price{100});
    assert(book.order_count() == 1);
  }

  // Test 7: partial fill leaves incoming limit remainder.
  {
    const openlob::Instrument instrument{
        .symbol = "AAPL",
        .tick_size = openlob::Price{1},
        .lot_size = openlob::Quantity{100},
    };
    openlob::OrderBook book(instrument);
    (void)book.add_order(make_order(openlob::OrderId{1},
                                    openlob::Side::Sell,
                                    openlob::Price{100},
                                    openlob::Quantity{3}));
    const auto trades = book.add_order(make_order(openlob::OrderId{2},
                                                   openlob::Side::Buy,
                                                   openlob::Price{100},
                                                   openlob::Quantity{10}));
    assert(trades.size() == 1);
    assert(trades[0].quantity == openlob::Quantity{3});
    assert(book.best_bid() == openlob::Price{100});
    assert(book.order_count() == 1);
  }

  // Test 8: market order does not store remainder.
  {
    const openlob::Instrument instrument{
        .symbol = "AAPL",
        .tick_size = openlob::Price{1},
        .lot_size = openlob::Quantity{100},
    };
    openlob::OrderBook book(instrument);
    (void)book.add_order(make_order(openlob::OrderId{1},
                                    openlob::Side::Sell,
                                    openlob::Price{100},
                                    openlob::Quantity{3}));
    const auto trades = book.add_order(make_order(openlob::OrderId{2},
                                                   openlob::Side::Buy,
                                                   openlob::Price{0},
                                                   openlob::Quantity{10},
                                                   openlob::OrderType::Market));
    assert(trades.size() == 1);
    assert(trades[0].quantity == openlob::Quantity{3});
    assert(book.empty());
    assert(!book.best_bid().has_value());
  }

  // Test 9: best price after level cleanup.
  {
    const openlob::Instrument instrument{
        .symbol = "AAPL",
        .tick_size = openlob::Price{1},
        .lot_size = openlob::Quantity{100},
    };
    openlob::OrderBook book(instrument);
    (void)book.add_order(make_order(openlob::OrderId{1},
                                    openlob::Side::Sell,
                                    openlob::Price{100},
                                    openlob::Quantity{3}));
    (void)book.add_order(make_order(openlob::OrderId{2},
                                    openlob::Side::Sell,
                                    openlob::Price{101},
                                    openlob::Quantity{5}));
    (void)book.add_order(make_order(openlob::OrderId{3},
                                    openlob::Side::Buy,
                                    openlob::Price{100},
                                    openlob::Quantity{3}));
    assert(book.best_ask() == openlob::Price{101});
  }

  // Test 10: cancel existing order.
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

  // Test 11: cancel missing order.
  {
    const openlob::Instrument instrument{
        .symbol = "AAPL",
        .tick_size = openlob::Price{1},
        .lot_size = openlob::Quantity{100},
    };
    openlob::OrderBook book(instrument);
    assert(!book.cancel_order(openlob::OrderId{999}));
  }

  // Test 12: cancel filled order returns false.
  {
    const openlob::Instrument instrument{
        .symbol = "AAPL",
        .tick_size = openlob::Price{1},
        .lot_size = openlob::Quantity{100},
    };
    openlob::OrderBook book(instrument);
    (void)book.add_order(make_order(openlob::OrderId{1},
                                    openlob::Side::Sell,
                                    openlob::Price{100},
                                    openlob::Quantity{5}));
    (void)book.add_order(make_order(openlob::OrderId{2},
                                    openlob::Side::Buy,
                                    openlob::Price{101},
                                    openlob::Quantity{5}));
    assert(!book.cancel_order(openlob::OrderId{1}));
  }

  // Test 13: contains_order false on empty book.
  {
    const openlob::Instrument instrument{
        .symbol = "AAPL",
        .tick_size = openlob::Price{1},
        .lot_size = openlob::Quantity{100},
    };
    openlob::OrderBook book(instrument);
    assert(!book.contains_order(openlob::OrderId{1}));
  }

  // Test 14: contains_order true after storing non-crossing limit order.
  {
    const openlob::Instrument instrument{
        .symbol = "AAPL",
        .tick_size = openlob::Price{1},
        .lot_size = openlob::Quantity{100},
    };
    openlob::OrderBook book(instrument);
    (void)book.add_order(make_order(openlob::OrderId{7},
                                    openlob::Side::Buy,
                                    openlob::Price{100},
                                    openlob::Quantity{5}));
    assert(book.contains_order(openlob::OrderId{7}));
  }

  // Test 15: contains_order false after cancel_order.
  {
    const openlob::Instrument instrument{
        .symbol = "AAPL",
        .tick_size = openlob::Price{1},
        .lot_size = openlob::Quantity{100},
    };
    openlob::OrderBook book(instrument);
    (void)book.add_order(make_order(openlob::OrderId{8},
                                    openlob::Side::Buy,
                                    openlob::Price{100},
                                    openlob::Quantity{5}));
    assert(book.cancel_order(openlob::OrderId{8}));
    assert(!book.contains_order(openlob::OrderId{8}));
  }

  // Test 16: contains_order false after full fill removal.
  {
    const openlob::Instrument instrument{
        .symbol = "AAPL",
        .tick_size = openlob::Price{1},
        .lot_size = openlob::Quantity{100},
    };
    openlob::OrderBook book(instrument);
    (void)book.add_order(make_order(openlob::OrderId{9},
                                    openlob::Side::Sell,
                                    openlob::Price{100},
                                    openlob::Quantity{5}));
    (void)book.add_order(make_order(openlob::OrderId{10},
                                    openlob::Side::Buy,
                                    openlob::Price{100},
                                    openlob::Quantity{5}));
    assert(!book.contains_order(openlob::OrderId{9}));
  }

  return 0;
}
