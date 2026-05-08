#pragma once

#include <cstddef>
#include <optional>
#include <queue>
#include <vector>

#include "openlob/core/event.hpp"

namespace openlob {

class EventQueue {
 public:
  void push(Event event);
  std::optional<Event> pop();

  [[nodiscard]] bool empty() const;
  [[nodiscard]] std::size_t size() const;

 private:
  // EventQueue stamps events at insertion time to prevent agents from lying,
  // colliding, or breaking deterministic replay.
  std::uint64_t next_sequence_id_{1};
  // Deterministic simulation scheduler: reproducibility matters more than raw speed.
  std::priority_queue<Event, std::vector<Event>, EventComparator> queue_;
};

}  // namespace openlob
