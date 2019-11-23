#pragma once
#include <string>

namespace market {

class Order {
public:
  Order(double price, uint64_t amount, std::string uuid):
    price_(price), amount_(amount), uuid_(std::move(uuid) {}

  double price() const { return price_; }
  double amount() const { return amount_; }
  double uuid() const { return uuid_; }

private:
  double price_;
  uint64_t amount_;
  std::string uuid_;
};

}  // market
