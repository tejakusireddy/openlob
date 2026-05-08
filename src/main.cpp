#include <iostream>

#include "openlob/book/matching_engine.hpp"

int main() {
  std::cout << "OpenLOB: Open Market Microstructure Lab\n";
  std::cout << "C++ L3 limit order book and market-making research simulator\n";

  openlob::MatchingEngine engine("DEMO");
  std::cout << "MatchingEngine initialized for symbol: " << engine.book().symbol() << '\n';

  return 0;
}
