#pragma once

#include <optional>
#include <iostream>

#include "../../third_party/json/json.hpp"
#include "../direction/direction.h"
#include "../trade/trade.h"
#include "../order/order.h"
#include "../snapshot/l2_snapshot.h"

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

  L2Snapshot l2_snapshot;

  Response() = default;
  Response(Trade trade_):
    type(ResponseType::NewTrade), trade(trade_) {}
  Response(L2Snapshot l2_snapshot_):
    type(ResponseType::NewSnapshot), l2_snapshot(l2_snapshot_) {}
};

std::string response_to_string(const Response& response) {
  if (response.type == ResponseType::NewTrade) {
    const auto& trade = response.trade;
    const tao::json::value json = {{"type", "trade"},  {"value", {{"price", trade.price()}, {"amount", trade.amount()}, {"aggressor_direction", direction_to_string(trade.aggressor_direction()) }}} };
    return tao::json::to_string(json);
  } else if (response.type == ResponseType::NewSnapshot) {
    return R"({"type": "l2_snapshot", "value": )" +response.l2_snapshot.to_string() + "}";
  }
}

} // namespace market::entities
