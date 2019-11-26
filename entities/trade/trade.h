#pragma once
#include "../direction/direction.h"

#include <string>
#include <cassert>

namespace market::entities {

class Trade {
public:
  Trade(
      double price,
      uint64_t amount,
      Direction aggressor_side,
      std::vector<std::string> order_uuids
  ):
      price_(price),
      amount_(amount),
      aggressor_side_(aggressor_side),
      order_uuids_(std::move(order_uuids))
  {}

  auto price() const { return price_; }
  auto amount() const { return amount_; }
  auto aggressor_side() const { return aggressor_side_; }
  const auto& order_uuids() const { return order_uuids_; }

private:
  double price_;
  uint64_t amount_;
  Direction aggressor_side_;
  std::vector<std::string> order_uuids_;
};

}  // namespace market::entities
