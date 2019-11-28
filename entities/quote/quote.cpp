#include "quote.h"

#include <cassert>

namespace market::entities {

void Quote::push(Order&& order) {
  assert(order.price() == price());
  assert(order.type() == Order::Type::Limit);
  assert(order.direction() == direction());
  aggregated_amount_ += order.amount();
  orders_.push(order);
}

Order& Quote::front() {
  assert(!empty());
  return orders_.front();
}

const Order& Quote::front() const {
  assert(!empty());
  return front();
}

void Quote::pop() {
  assert(!empty());
  aggregated_amount_ -= orders_.front().amount();
  orders_.pop();
}

}  // namespace market::entities
