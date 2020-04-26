#pragma once

#include <optional>
#include <iostream>

#include "../../third_party/json/json.hpp"
#include "../direction/direction.h"
#include "../trade/trade.h"
#include "../order/order.h"
#include "../snapshot/l1_snapshot.h"

namespace market::entities {

enum class UpdateType : size_t {
  NewSnapshot = 0,
  NewTrade = 1,
  NewOrder = 2
};

struct Update {
  UpdateType type;

  Trade trade;

  Direction direction;
  int price;
  int amount;

  std::string id;
};

std::optional<Command> command_from_string(std::string string) {
  Command command;
  try {
    auto json = tao::json::from_string(string);
    auto type = json.at("type").as<std::string>();
    if (type == "new_order") {
      command.type = CommandType::NewOrder;
      auto direction = json.at("direction").as<std::string>();
      if (direction == "bid") {
        command.direction = Direction::Bid;
      } else if (direction == "ask") {
        command.direction = Direction::Ask;
      } else {
        throw "unknown direction: " + direction;
      }
      command.price = json.at("price").as<int>();
      command.amount = json.at("amount").as<int>();
    } else if (type == "cancel_order") {
      command.type = CommandType::CancelOrder;
      command.id = json.at("id").as<std::string>();
    } else {
      throw "unknown type: " + type;
    }
  } catch (std::exception& ex) {
      std::clog << "cannot parse command from string: " << string << " exception: " << ex.what() << std::endl;
      return std::nullopt;
  }
  return command;
}

void print_command(const Command& command) {
  std::clog << "command: type: ";
  if (command.type == CommandType::NewOrder) {
    std::clog << "new_order price: " << command.price << " amount: " << command.amount << " direction: ";
    if (command.direction == Direction::Bid) {
      std::clog << "bid" << std::endl;
    } else if (command.direction == Direction::Ask) {
      std::clog << "ask" << std::endl;
    }
  } else if (command.type == CommandType::CancelOrder) {
    std::clog << "cancel_order id: " << command.id << std::endl;
  }
}

} // namespace market::entities
