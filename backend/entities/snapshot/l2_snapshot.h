#pragma once
#include "../direction/direction.h"
#include "../../third_party/json/json.hpp"
#include <utility>
#include <optional>
#include <vector>
#include <tuple>

namespace market::entities {

class L2Snapshot {
public:
  struct Quote {
    double price;
    uint64_t amount;
  };

  L2Snapshot() = default;
  L2Snapshot(std::vector<Quote> bid_quotes, std::vector<Quote> ask_quotes):
      bid_quotes_(std::move(bid_quotes)), ask_quotes_(std::move(ask_quotes)) {}

  const auto& bid_quotes() const { return bid_quotes_; }
  const auto& ask_quotes() const { return ask_quotes_; }

  std::string to_string() const {
    std::string bid_string = "[";
    for (size_t i = 0; i < bid_quotes_.size(); ++i) {
      const auto& quote = bid_quotes_[i];
      bid_string += tao::json::to_string(tao::json::value{{"price", quote.price}, {"amount", quote.amount}});
      if (i != bid_quotes_.size() - 1) {
        bid_string += ", ";
      }
    }
    bid_string += "]";

    std::string ask_string = "[";
    for (size_t i = 0; i < ask_quotes_.size(); ++i) {
      const auto& quote = ask_quotes_[i];
      ask_string += tao::json::to_string(tao::json::value{{"price", quote.price}, {"amount", quote.amount}});
      if (i != ask_quotes_.size() - 1) {
        ask_string += ", ";
      }
    }
    ask_string += "]";

    return "{\"bids\":" + bid_string + ", \"asks\":" + ask_string + "}";
  };

private:
  std::vector<Quote> bid_quotes_;
  std::vector<Quote> ask_quotes_;
};

}  // namespace market::entities
