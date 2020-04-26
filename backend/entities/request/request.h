#pragma once

#include <optional>
#include <iostream>

#include "../../third_party/json/json.hpp"
#include "../direction/direction.h"

namespace market::entities {

enum class RequestType : size_t {
  NewOrder = 0,
  CancelOrder = 1,
  GetInfo = 2
};

struct Request {
  RequestType type;

  std::string user;

  Direction direction;
  double price;
  int amount;

  std::string id;
};

std::optional<Request> request_from_string(std::string string) {
  Request request;
  try {
    auto json = tao::json::from_string(string);
    request.user = json.at("user").as<std::string>();
    auto type = json.at("type").as<std::string>();
    if (type == "new_order") {
      request.type = RequestType::NewOrder;
      auto direction = json.at("direction").as<std::string>();
      if (direction == "bid") {
        request.direction = Direction::Bid;
      } else if (direction == "ask") {
        request.direction = Direction::Ask;
      } else {
        throw "unknown direction: " + direction;
      }
      request.price = json.at("price").as<int>();
      request.amount = json.at("amount").as<int>();
    } else if (type == "cancel_order") {
      request.type = RequestType::CancelOrder;
      request.id = json.at("id").as<std::string>();
    } else if (type == "get_info") {
      request.type = RequestType::GetInfo;
    } else {
      throw "unknown type: " + type;
    }
  } catch (std::exception& ex) {
      std::clog << "cannot parse request from string: " << string << " exception: " << ex.what() << std::endl;
      return std::nullopt;
  }
  return request;
}

std::string request_to_string(const Request& request) {
  if (request.type == RequestType::NewOrder) {
    const tao::json::value json = { {"price", request.price}, {"amount", request.amount}, {"direction", direction_to_string(request.direction) } };
    return tao::json::to_string(json);
  } else {
    const tao::json::value json = { {"cancel_order_id", request.id} };
    return tao::json::to_string(json);
  }
}

void print_request(const Request& request) {
  std::clog << "request: " << request_to_string(request) << std::endl;
}

} // namespace market::entities
