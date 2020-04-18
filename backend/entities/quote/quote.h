#pragma once
#include "../order/order.h"
#include "../direction/direction.h"
#include <queue>

namespace market::entities {

class Quote {
public:
  Quote(double price, Direction direction):
    price_(price), direction_(direction) {}

  double price() const { return price_; }
  Direction direction() const { return direction_; }
  uint64_t aggregated_amount() const { return aggregated_amount_; }
  bool empty() const { return orders_.empty(); }

  void push(Order&& order);
  Order& front();
  const Order& front() const;
  void pop();

private:
  const double price_;
  Direction direction_;
  uint64_t aggregated_amount_ = 0;
  std::queue<Order> orders_;
};

}  // namespace market::entities
