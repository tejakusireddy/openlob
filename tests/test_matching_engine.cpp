#include <cassert>
#include <string>

#include "openlob/book/matching_engine.hpp"

int main() {
  auto make_order = [](openlob::OrderId id,
                       openlob::Symbol symbol,
                       openlob::OrderType type,
                       openlob::Price price,
                       openlob::Quantity quantity,
                       openlob::Timestamp timestamp) {
    return openlob::Order{
        .id = id,
        .agent_id = openlob::AgentId{42},
        .symbol = symbol,
        .side = openlob::Side::Buy,
        .type = type,
        .price = price,
        .quantity = quantity,
        .timestamp = timestamp,
    };
  };

  // Test 1: reject wrong symbol.
  {
    const openlob::Instrument instrument{
        .symbol = "TEST",
        .tick_size = openlob::Price{5},
        .lot_size = openlob::Quantity{10},
    };
    openlob::MatchingEngine engine(instrument);
    const openlob::Order wrong_symbol_order = make_order(openlob::OrderId{1},
                                                          "OTHER",
                                                          openlob::OrderType::Limit,
                                                          openlob::Price{100},
                                                          openlob::Quantity{10},
                                                          openlob::Timestamp{1});
    const openlob::ExecutionReport report = engine.submit_order(wrong_symbol_order);
    assert(report.rejects.size() == 1);
    assert(report.acks.empty());
    assert(report.trades.empty());
    assert(report.rejects[0].reason.find("symbol") != std::string::npos);
  }

  // Test 2: reject zero quantity.
  {
    const openlob::Instrument instrument{
        .symbol = "TEST",
        .tick_size = openlob::Price{5},
        .lot_size = openlob::Quantity{10},
    };
    openlob::MatchingEngine engine(instrument);
    const openlob::Order zero_qty_order = make_order(openlob::OrderId{2},
                                                      "TEST",
                                                      openlob::OrderType::Limit,
                                                      openlob::Price{100},
                                                      openlob::Quantity{0},
                                                      openlob::Timestamp{2});
    const openlob::ExecutionReport report = engine.submit_order(zero_qty_order);
    assert(report.rejects.size() == 1);
    assert(report.rejects[0].reason.find("quantity") != std::string::npos ||
           report.rejects[0].reason.find("lot") != std::string::npos);
  }

  // Test 3: reject non-lot-size quantity.
  {
    const openlob::Instrument instrument{
        .symbol = "TEST",
        .tick_size = openlob::Price{5},
        .lot_size = openlob::Quantity{10},
    };
    openlob::MatchingEngine engine(instrument);
    const openlob::Order non_lot_order = make_order(openlob::OrderId{3},
                                                     "TEST",
                                                     openlob::OrderType::Limit,
                                                     openlob::Price{100},
                                                     openlob::Quantity{15},
                                                     openlob::Timestamp{3});
    const openlob::ExecutionReport report = engine.submit_order(non_lot_order);
    assert(report.rejects.size() == 1);
    assert(report.rejects[0].reason.find("lot") != std::string::npos ||
           report.rejects[0].reason.find("quantity") != std::string::npos);
  }

  // Test 4: reject invalid limit price tick.
  {
    const openlob::Instrument instrument{
        .symbol = "TEST",
        .tick_size = openlob::Price{5},
        .lot_size = openlob::Quantity{10},
    };
    openlob::MatchingEngine engine(instrument);
    const openlob::Order bad_tick_order = make_order(openlob::OrderId{4},
                                                      "TEST",
                                                      openlob::OrderType::Limit,
                                                      openlob::Price{103},
                                                      openlob::Quantity{10},
                                                      openlob::Timestamp{4});
    const openlob::ExecutionReport report = engine.submit_order(bad_tick_order);
    assert(report.rejects.size() == 1);
    assert(report.rejects[0].reason.find("tick") != std::string::npos ||
           report.rejects[0].reason.find("price") != std::string::npos);
  }

  // Test 5: reject zero/negative limit price.
  {
    const openlob::Instrument instrument{
        .symbol = "TEST",
        .tick_size = openlob::Price{5},
        .lot_size = openlob::Quantity{10},
    };
    openlob::MatchingEngine engine(instrument);
    const openlob::Order zero_price_order = make_order(openlob::OrderId{5},
                                                        "TEST",
                                                        openlob::OrderType::Limit,
                                                        openlob::Price{0},
                                                        openlob::Quantity{10},
                                                        openlob::Timestamp{5});
    const openlob::ExecutionReport report = engine.submit_order(zero_price_order);
    assert(report.rejects.size() == 1);
    assert(report.rejects[0].reason.find("price") != std::string::npos ||
           report.rejects[0].reason.find("tick") != std::string::npos);
  }

  // Test 6: market order ignores price tick validation.
  {
    const openlob::Instrument instrument{
        .symbol = "TEST",
        .tick_size = openlob::Price{5},
        .lot_size = openlob::Quantity{10},
    };
    openlob::MatchingEngine engine(instrument);
    const openlob::Order market_order = make_order(openlob::OrderId{6},
                                                    "TEST",
                                                    openlob::OrderType::Market,
                                                    openlob::Price{103},
                                                    openlob::Quantity{10},
                                                    openlob::Timestamp{6});
    const openlob::ExecutionReport report = engine.submit_order(market_order);
    assert(report.rejects.empty());
    assert(report.acks.size() == 1);
  }

  // Test 7: valid limit order accepted.
  {
    const openlob::Instrument instrument{
        .symbol = "TEST",
        .tick_size = openlob::Price{5},
        .lot_size = openlob::Quantity{10},
    };
    openlob::MatchingEngine engine(instrument);
    const openlob::Order valid_order = make_order(openlob::OrderId{7},
                                                   "TEST",
                                                   openlob::OrderType::Limit,
                                                   openlob::Price{100},
                                                   openlob::Quantity{20},
                                                   openlob::Timestamp{7});
    const openlob::ExecutionReport report = engine.submit_order(valid_order);
    assert(report.rejects.empty());
    assert(report.acks.size() == 1);
  }

  // Test 8: rejected invalid order does not mutate book.
  {
    const openlob::Instrument instrument{
        .symbol = "TEST",
        .tick_size = openlob::Price{5},
        .lot_size = openlob::Quantity{10},
    };
    openlob::MatchingEngine engine(instrument);
    const openlob::Order invalid_order = make_order(openlob::OrderId{99},
                                                     "OTHER",
                                                     openlob::OrderType::Limit,
                                                     openlob::Price{100},
                                                     openlob::Quantity{10},
                                                     openlob::Timestamp{8});
    const openlob::ExecutionReport report = engine.submit_order(invalid_order);
    assert(report.rejects.size() == 1);
    assert(!engine.book().contains_order(openlob::OrderId{99}));
  }

  // Existing duplicate-live-id behavior remains after validation checks.
  {
    const openlob::Instrument instrument{
        .symbol = "TEST",
        .tick_size = openlob::Price{5},
        .lot_size = openlob::Quantity{10},
    };
    openlob::MatchingEngine engine(instrument);
    const openlob::Order first = make_order(openlob::OrderId{200},
                                             "TEST",
                                             openlob::OrderType::Limit,
                                             openlob::Price{100},
                                             openlob::Quantity{10},
                                             openlob::Timestamp{9});
    const openlob::Order duplicate = make_order(openlob::OrderId{200},
                                                 "TEST",
                                                 openlob::OrderType::Limit,
                                                 openlob::Price{100},
                                                 openlob::Quantity{10},
                                                 openlob::Timestamp{10});
    (void)engine.submit_order(first);
    const openlob::ExecutionReport duplicate_report = engine.submit_order(duplicate);
    assert(duplicate_report.rejects.size() == 1);
    assert(duplicate_report.rejects[0].reason.find("duplicate") != std::string::npos);
    assert(duplicate_report.acks.empty());
    assert(duplicate_report.trades.empty());
  }

  return 0;
}
