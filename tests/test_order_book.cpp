#include <cassert>

#include "openlob/book/order_book.hpp"

int main() {
  const openlob::Instrument instrument{
      .symbol = "AAPL",
      .tick_size = openlob::Price{1},
      .lot_size = openlob::Quantity{100},
  };

  openlob::OrderBook book(instrument);
  assert(book.instrument().symbol == instrument.symbol);
  assert(book.symbol() == instrument.symbol);
  assert(book.empty());
  return 0;
}
