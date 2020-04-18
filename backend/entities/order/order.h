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

  Order(double price, uint64_t amount, Direction direction, Type type, std::string uuid = ""):
      price_(price), amount_(amount), direction_(direction), type_(type), uuid_(std::move(uuid)) {
    assert(price_ > 0);
    assert(amount_ > 0);
  }

  double price() const { return price_; }
  uint64_t amount() const { return amount_; }
  Direction direction() const { return direction_; }
  Type type() const { return type_; }
  const std::string& uuid() const { return uuid_; }

private:
  void set_amount(uint64_t amount) {
    amount_ = amount;
  }

  double price_;
  uint64_t amount_;
  Direction direction_;
  Type type_;
  std::string uuid_;

  friend class market::matching_engine::MatchingEngine;
};

}  // namespace market::entities
