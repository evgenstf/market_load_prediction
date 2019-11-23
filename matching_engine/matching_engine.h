#pragma once

#include <map>

class MatchingEngine {
public:
  struct Parameters {
    size_t l2_snapshot_depth;
  };

  using DirectionContainer = std::map<double, Quote>;
  using OrderBook = DirectionContainer[2];

  MatchingEngine(const MatchingEngineParameters& parameters)

  std::vector<Trade> add_order(Order&& order);

  L1Snapshot build_l1_snapshot() const;
  L2Snapshot build_l2_snapshot() const;

private:
  const Parameters parameters_;

  OrderBook order_book_;
};
