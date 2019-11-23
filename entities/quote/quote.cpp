#include "quote.h"

#include <cassert>

namespace market::entities {

void Quote::push(Order&& order) {
  assert(order.price() == price());
  aggregated_amount_ += order.amount();
  orders_.push(order);
}

const Order& Quote::front() const {
  assert(!empty());
  return orders_.front();
}

void Quote::pop() {
  assert(!empty());
  aggregated_amount_ -= orders_.front().amount();
  orders_.pop();
}

}  // market::entities
