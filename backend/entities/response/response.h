#pragma once

#include <optional>
#include <iostream>

#include "../../third_party/json/json.hpp"
#include "../direction/direction.h"
#include "../trade/trade.h"
#include "../order/order.h"
#include "../snapshot/l2_snapshot.h"
#include "../user/user.h"

namespace market::entities {

enum class ResponseType : size_t {
  NewSnapshot = 0,
  NewTrade = 1,
  NewOrder = 2,
  UserInfo = 2,
  Error = 3
};

struct Response {
  ResponseType type;

  Trade trade;

  Direction direction;
  int price;
  int amount;

  std::string id;

  L2Snapshot l2_snapshot;

  std::string error_message;

  User user_info;

  Response() = default;
  Response(Trade trade_):
    type(ResponseType::NewTrade), trade(trade_) {}
  Response(L2Snapshot l2_snapshot_):
    type(ResponseType::NewSnapshot), l2_snapshot(l2_snapshot_) {}
  explicit Response(std::string error_message_):
    type(ResponseType::Error), error_message(error_message_) {}
  Response(User user_info_):
    type(ResponseType::UserInfo), user_info(user_info_) {}
};

std::string response_to_string(const Response& response) {
  if (response.type == ResponseType::NewTrade) {
    const auto& trade = response.trade;
    const tao::json::value json = {
      {"type", "trade"},
      {"value", {
          {"price", trade.price()},
          {"amount", trade.amount()},
          {"aggressor_direction", direction_to_string(trade.aggressor_direction())}
      }}};
    return tao::json::to_string(json);
  } else if (response.type == ResponseType::NewSnapshot) {
    return R"({"type": "l2_snapshot", "value": )" +response.l2_snapshot.to_string() + "}";
  } else if (response.type == ResponseType::UserInfo) {
    const auto& user_info = response.user_info;
    const tao::json::value json = {
      {"type", "user_info"},
      {"value", {
          {"name", user_info.name()},
          {"balance", user_info.balance()},
          {"blocked_balance", user_info.blocked_balance()},
          {"position", user_info.position()},
          {"blocked_position", user_info.blocked_position()}
      }}};
    auto json_string = tao::json::to_string(json);
    json_string.resize(json_string.size() - 2);

    json_string += ", \"orders\": [";
    for (const auto& [id, order] : user_info.order_by_id()) {
      const tao::json::value order_json = {
          {"id", order.id()},
          {"price", order.price()},
          {"amount", order.amount()},
          {"direction", direction_to_string(order.direction())}
      };
      json_string += tao::json::to_string(order_json) + ", ";
    }
    if (user_info.order_by_id().size()) {
      json_string.resize(json_string.size() - 2);
    }
    json_string += "]}}";
    return json_string;
  } else if (response.type == ResponseType::Error) {
    return R"({"error": ")" + response.error_message + "\"}";
  }
}

} // namespace market::entities
