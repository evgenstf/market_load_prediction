#pragma once
#include "../direction/direction.h"
#include <string>
#include <cassert>

namespace market::matching_engine {

class MatchingEngine;

}  // namespace market::matching_engine

namespace market::entities {

class Order {
public:
  enum Type : uint8_t {
    Limit = 0,
    Ioc = 1
  };

  Order(
      double price,
      uint64_t amount,
      Direction direction,
      Type type,
      size_t id,
      std::string user
  ):
      price_(price),
      amount_(amount),
      direction_(direction),
      type_(type),
      id_(id),
      user_(std::move(user)
  ) {
    assert(price_ > 0);
    assert(amount_ > 0);
  }

  double price() const { return price_; }
  uint64_t amount() const { return amount_; }
  Direction direction() const { return direction_; }
  Type type() const { return type_; }
  size_t id() const { return id_; }
  const std::string& user() const { return user_; }

private:
  void set_amount(uint64_t amount) {
    amount_ = amount;
  }

  double price_;
  uint64_t amount_;
  Direction direction_;
  Type type_;
  size_t id_;
  std::string user_;

  friend class market::matching_engine::MatchingEngine;
};

}  // namespace market::entities
