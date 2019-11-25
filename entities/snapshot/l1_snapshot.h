#pragma once
#include "../direction/direction.h"
#include <utility>

namespace market::entities {

class L1Snapshot {
public:
  struct Quote {
    double price;
    uint64_t amount;
  };

  L1Snapshot(Quote best_bid, Quote best_ask):
    best_bid_(std::move(best_bid)), best_ask_(std::move(best_ask)) {}

  const Quote& best_quote(Direction direction) {
    return (direction == Direction::Bid) ? best_bid_ : best_ask_;
  }

private:
  Quote best_bid_;
  Quote best_ask_;
};

}  // namespace market::entities
