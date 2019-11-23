#pragma once
#include <string>
#include <cassert>

namespace market::entities {

class Order {
public:
  Order(double price, uint64_t amount, std::string uuid = ""):
      price_(price), amount_(amount), uuid_(std::move(uuid)) {
    assert(price_ > 0);
    assert(amount_ > 0);
  }

  double price() const { return price_; }
  uint64_t amount() const { return amount_; }
  const std::string& uuid() const { return uuid_; }

private:
  double price_;
  uint64_t amount_;
  std::string uuid_;
};

}  // market::entities
