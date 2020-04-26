#pragma once

#include "../entities/order/order.h"
#include "../entities/user/user.h"
#include "../entities/trade/trade.h"

#include <iostream>
#include <unordered_map>

namespace market::user_storage {

class UserStorage {
public:
  UserStorage() {
  }

  void add_user(const entities::User& user) {
    users_.emplace(user.name(), user);
  }

  const entities::User& user(const std::string& name) const {
    return users_.at(name);
  }

  bool contains(const std::string& name) const {
    return users_.find(name) != users_.end();
  }

  std::optional<std::string> process_order(const entities::Order& order) {
    if (!contains(order.user())) {
      return "not exist user: " + order.user();
    }

    auto& user = users_.at(order.user());
    if (order.direction() == entities::Direction::Bid) {
      double price = order.price();
      int amount = order.amount();
      double volume = price * amount;

      auto free_balance = user.balance() - user.blocked_balance();
      if (free_balance < volume) {
        return std::string("not enough free balance: " + std::to_string(free_balance) +
            " for user: " + order.user() +
            " to place order with price: " + std::to_string(price) +
            " amount: " + std::to_string(amount));
      }

      user.add_order(order, volume, 0);
    } else if (order.direction() == entities::Direction::Ask) {
      double price = order.price();
      int amount = order.amount();

      auto free_position = user.position() - user.blocked_position();
      if (free_position < amount) {
        return std::string("not enough free position: " + std::to_string(free_position) +
            " for user: " + order.user() +
            " to place order with price: " + std::to_string(price) +
            " amount: " + std::to_string(amount));
      }

      user.add_order(order, 0, amount);
    } else {
      std::clog << "WARNING: unknown direction: " << static_cast<int>(order.direction()) << std::endl;
    }
    user_by_order_id_[order.id()] = order.user();
    direction_by_order_id_[order.id()] = order.direction();
    return std::nullopt;
  }

  void process_trades(const std::vector<entities::Trade>& trades) {
    for (const auto& trade : trades) {
      for (const auto& order_id : trade.order_ids()) {
        auto& user = users_[user_by_order_id_[order_id]];
        user.remove_order(order_id);
        if (direction_by_order_id_[order_id] == entities::Direction::Bid) {
          auto volume = trade.price() * trade.amount();
          user.set_balance(user.balance() - volume);
          user.set_position(user.position() + trade.amount());
        } else {
          auto volume = trade.price() * trade.amount();
          user.set_balance(user.balance() + volume);
          user.set_position(user.position() - trade.amount());
        }
      }
    }
  }

private:
  std::unordered_map<std::string, entities::User> users_;
  std::unordered_map<size_t, std::string> user_by_order_id_;
  std::unordered_map<size_t, entities::Direction> direction_by_order_id_;
};

} // namespace market::user_storage
