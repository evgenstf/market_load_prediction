#pragma once
#include <string>
#include <cassert>

namespace market::entities {

class Order {
public:
  enum Type : uint8_t {
    Limit = 0,
    Ioc = 1
  };

  Order(double price, uint64_t amount, Type type, std::string uuid = ""):
      price_(price), amount_(amount), type_(type), uuid_(std::move(uuid)) {
    assert(price_ > 0);
    assert(amount_ > 0);
  }

  double price() const { return price_; }
  uint64_t amount() const { return amount_; }
  Type type() const { return type_; }
  const std::string& uuid() const { return uuid_; }

private:
  double price_;
  uint64_t amount_;
  Type type_;
  std::string uuid_;
};

}  // market::entities
