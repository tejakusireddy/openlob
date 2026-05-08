#include <chrono>
#include <iostream>

#include "openlob/book/order_book.hpp"

int main() {
  using clock = std::chrono::steady_clock;

  openlob::OrderBook book("BENCH");
  constexpr int iterations = 100000;

  const auto start = clock::now();
  for (int i = 0; i < iterations; ++i) {
    openlob::Order order{
        .id = openlob::OrderId{static_cast<std::uint64_t>(i + 1)},
        .agent_id = openlob::AgentId{1},
        .symbol = "BENCH",
        .side = (i % 2 == 0) ? openlob::Side::Buy : openlob::Side::Sell,
        .type = openlob::OrderType::Limit,
        .price = openlob::Price{10000 + (i % 10)},
        .quantity = openlob::Quantity{1},
        .timestamp = openlob::Timestamp{i},
    };
    (void)book.add_order(order);
  }
  const auto end = clock::now();

  const auto elapsed_ns =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  std::cout << "Placeholder order book benchmark\n";
  std::cout << "Iterations: " << iterations << ", elapsed(ns): " << elapsed_ns << '\n';
  std::cout << "TODO(openlob): replace with representative workload and profiling hooks.\n";

  return 0;
}
