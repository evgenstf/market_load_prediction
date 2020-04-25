#pragma once
#include "../entities/snapshot/l1_snapshot.h"
#include "../entities/quote/quote.h"
#include "../entities/trade/trade.h"
#include "../entities/order/order.h"

#include <map>

namespace market::matching_engine {

class MatchingEngine {
public:
  /*
    struct Parameters {
      size_t l2_snapshot_depth;
    };
  */

  using DirectionContainer = std::map<double, entities::Quote>;
  using OrderBook = DirectionContainer[2];

  MatchingEngine();

  std::vector<entities::Trade> add_order(entities::Order&& order);

  entities::L1Snapshot build_l1_snapshot() const;

private:
  OrderBook order_book_;
};

}  // namespace market::matching_engine
