#pragma once

#include <iostream>
#include <unordered_map>

#include "../order/order.h"

namespace market::entities {

class User {
public:
  User() = default;

  User(std::string name):
    name_(name) { }

  User(std::string name, double balance, size_t position):
    name_(name), balance_(balance), position_(position) { }

  void set_balance(double balance) {
    balance_ = balance;
  }

  void set_position(size_t position) {
    position_ = position;
  }

  size_t position() const { return position_;}
  size_t blocked_position() const { return blocked_position_;}

  double balance() const { return balance_; }
  double blocked_balance() const { return blocked_balance_; }

  void add_order(Order order, double blocked_balance, size_t blocked_position) {
    auto id = order.id();
    blocked_balance_ += blocked_balance;
    blocked_position_ += blocked_position;
    blocked_balance_by_order_[id] = blocked_balance;
    blocked_position_by_order_[id] = blocked_position;
    order_by_id_.emplace(id, std::move(order));
  }

  void remove_order(size_t id) {
    blocked_balance_ -= blocked_balance_by_order_[id];
    blocked_position_ -= blocked_position_by_order_[id];
    blocked_balance_by_order_.erase(id);
    blocked_position_by_order_.erase(id);
    order_by_id_.erase(id);
  }

  const auto& order_by_id() const { return order_by_id_; }
  const std::string& name() const { return name_; }

private:
  const std::string name_;
  double balance_ = 0;
  size_t position_ = 0;

  double blocked_balance_ = 0;
  size_t blocked_position_ = 0;

  std::unordered_map<size_t, double> blocked_balance_by_order_;
  std::unordered_map<size_t, size_t> blocked_position_by_order_;

  std::unordered_map<size_t, Order> order_by_id_;
};

} // namespace market::entities
