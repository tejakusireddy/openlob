#include <iostream>

#include "openlob/book/matching_engine.hpp"

int main() {
  std::cout << "OpenLOB: Open Market Microstructure Lab\n";
  std::cout << "C++ L3 limit order book and market-making research simulator\n";

  const openlob::Instrument instrument{
      .symbol = "DEMO",
      .tick_size = openlob::Price{1},
      .lot_size = openlob::Quantity{1},
  };
  openlob::MatchingEngine engine(instrument);
  std::cout << "MatchingEngine initialized for symbol: " << engine.book().symbol() << '\n';

  return 0;
}
