#pragma once
#include "../direction/direction.h"
#include <utility>
#include <optional>
#include <tuple>

namespace market::entities {

class L1Snapshot {
public:
  struct Quote {
    double price;
    uint64_t amount;

    bool operator==(const Quote& right) const {
      return std::make_tuple(price, amount) == std::make_tuple(right.price, right.amount);
    }
  };

  L1Snapshot(std::optional<Quote> best_bid, std::optional<Quote> best_ask):
      best_bid_(std::move(best_bid)), best_ask_(std::move(best_ask)) {}

  const auto& best_quote(Direction direction) const {
    return (direction == Direction::Bid) ? best_bid_ : best_ask_;
  }

  bool operator==(const L1Snapshot& right) const {
    return
        best_quote(Direction::Bid) == right.best_quote(Direction::Bid) &&
        best_quote(Direction::Ask) == right.best_quote(Direction::Ask);
  }

private:
  std::optional<Quote> best_bid_;
  std::optional<Quote> best_ask_;
};

}  // namespace market::entities
