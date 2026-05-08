#include <cassert>

#include "openlob/book/order_book.hpp"

int main() {
  openlob::OrderBook book("AAPL");
  assert(book.symbol() == "AAPL");
  assert(book.empty());
  return 0;
}
