#pragma once

#include <optional>
#include <iostream>

#include "../../third_party/json/json.hpp"
#include "../direction/direction.h"
#include "../trade/trade.h"
#include "../order/order.h"
#include "../snapshot/l1_snapshot.h"

namespace market::entities {

enum class ResponseType : size_t {
  NewSnapshot = 0,
  NewTrade = 1,
  NewOrder = 2
};

struct Response {
  ResponseType type;

  Trade trade;

  Direction direction;
  int price;
  int amount;

  std::string id;
};

std::string response_to_string(const Response& response) {
  if (response.type == ResponseType::NewTrade) {
    const auto& trade = response.trade;
    const tao::json::value json = { {"price", trade.price()}, {"amount", trade.amount()}, {"aggressor_direction", direction_to_string(trade.aggressor_direction()) } };
    return tao::json::to_string(json);
  }
}

} // namespace market::entities
