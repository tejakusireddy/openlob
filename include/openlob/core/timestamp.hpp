#pragma once

#include <compare>
#include <cstdint>

namespace openlob {

struct Timestamp {
  std::int64_t ns{0};
  auto operator<=>(const Timestamp&) const = default;

  [[nodiscard]] static Timestamp zero() { return Timestamp{0}; }

  [[nodiscard]] Timestamp operator+(std::int64_t delta_ns) const { return Timestamp{ns + delta_ns}; }
  [[nodiscard]] Timestamp operator-(std::int64_t delta_ns) const { return Timestamp{ns - delta_ns}; }
};

// Strong Timestamp avoids accidental mixing with price/ids and keeps
// simulation-time reasoning explicit and deterministic.

}  // namespace openlob
