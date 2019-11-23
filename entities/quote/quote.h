#pragma once
#include "../order/order.h"
#include <queue>

namespace market::entities {

class Quote {
public:
  Quote(double price):
    price_(price) {}

  double price() const { return price_; }
  double aggregated_amount() const { return aggregated_amount_; }
  bool empty() const { return orders_.empty(); }

  void push(Order&& order);
  const Order& front() const;
  void pop();

private:
  std::queue<Order> orders_;
  const double price_;
  double aggregated_amount_ = 0;
};

}  // namespace market::entities
