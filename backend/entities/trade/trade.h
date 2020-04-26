#pragma once
#include "../direction/direction.h"

#include <string>
#include <cassert>

namespace market::entities {

class Trade {
public:
  Trade() = default;
  Trade(
      double price,
      uint64_t amount,
      Direction aggressor_direction,
      std::vector<size_t> order_ids
  ):
      price_(price),
      amount_(amount),
      aggressor_direction_(aggressor_direction),
      order_ids_(std::move(order_ids))
  {}

  auto price() const { return price_; }
  auto amount() const { return amount_; }
  auto aggressor_direction() const { return aggressor_direction_; }
  const auto& order_ids() const { return order_ids_; }

private:
  double price_;
  uint64_t amount_;
  Direction aggressor_direction_;
  std::vector<size_t> order_ids_;
};

}  // namespace market::entities
