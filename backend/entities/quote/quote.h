#pragma once
#include "../order/order.h"
#include "../direction/direction.h"
#include <deque>

namespace market::entities {

class Quote {
public:
  Quote(double price, Direction direction):
    price_(price), direction_(direction) {}

  double price() const { return price_; }
  Direction direction() const { return direction_; }
  uint64_t aggregated_amount() const { return aggregated_amount_; }
  bool empty() const { return orders_.empty(); }

void push(Order&& order) {
  assert(order.price() == price());
  assert(order.type() == Order::Type::Limit);
  assert(order.direction() == direction());
  aggregated_amount_ += order.amount();
  orders_.push_back(order);
}

void update_aggregated_amount() {
  aggregated_amount_ = 0;
  for (const auto& order : orders_) {
    aggregated_amount_ += order.amount();
  }
}

Order& front() {
  assert(!empty());
  return orders_.front();
}

const Order& front() const {
  assert(!empty());
  return front();
}

void pop() {
  assert(!empty());
  aggregated_amount_ -= orders_.front().amount();
  orders_.pop_front();
}

private:
  const double price_;
  Direction direction_;
  uint64_t aggregated_amount_ = 0;
  std::deque<Order> orders_;
};

}  // namespace market::entities
