#pragma once
#include "../entities/snapshot/l1_snapshot.h"
#include "../entities/snapshot/l2_snapshot.h"
#include "../entities/quote/quote.h"
#include "../entities/trade/trade.h"
#include "../entities/order/order.h"

#include <map>

namespace market::matching_engine {

using namespace entities;

bool closer_to_spread(double left_price, double right_price, Direction direction) {
  return direction == Direction::Bid ? left_price > right_price : left_price < right_price;
}

Direction opposite_direction(Direction direction) {
  return direction == Bid ? Ask : Bid;
}

class MatchingEngine {
public:
  /*
    struct Parameters {
      size_t l2_snapshot_depth;
    };
  */
  struct DirectionContainer {
    DirectionContainer(Direction direction_):
      direction(direction_) {}

    bool empty() const { return quotes.empty(); }
    auto begin() { return quotes.begin(); }
    auto begin() const { return quotes.begin(); }

    void erase(std::map<double, entities::Quote>::iterator iterator) {
      quotes.erase(iterator);
    }

    auto find(double price) {
      if (direction == Direction::Bid) {
        return quotes.find(-price);
      } else {
        return quotes.find(price);
      }
    }

    auto emplace(double price, entities::Quote quote) {
      if (direction == Direction::Bid) {
        return quotes.emplace(-price, quote);
      } else {
        return quotes.emplace(price, quote);
      }
    }

    auto end() const {
      return quotes.end();
    }

    std::map<double, entities::Quote> quotes;
    Direction direction;
  };

  using OrderBook = DirectionContainer[2];

  std::vector<Trade> add_order(Order&& order) {
    std::vector<Trade> trades;
    auto trade_direction = opposite_direction(order.direction());
    auto rest_amount = order.amount();

    while (
        !order_book_[trade_direction].empty() &&
        rest_amount > 0 &&
        !closer_to_spread(
          order.price(),
          order_book_[trade_direction].begin()->second.price(),
          trade_direction
        )
    ) {
      Quote& trade_quote = order_book_[trade_direction].begin()->second;
      Order& trade_order = trade_quote.front();

      auto trade_price = trade_order.price();
      auto trade_amount = std::min(trade_order.amount(), rest_amount);

      trades.push_back(
          Trade{
            trade_price,
            trade_amount,
            order.direction(),
            std::vector<std::string>{order.uuid(), trade_order.uuid()}
          }
      );

      trade_order.set_amount(trade_order.amount() - trade_amount);
      rest_amount -= trade_amount;

      trade_quote.update_aggregated_amount();
      if (trade_order.amount() == 0) {
        trade_quote.pop();
        if (trade_quote.empty()) {
          order_book_[trade_direction].erase(order_book_[trade_direction].begin());
        }
      }
    }

    if (order.type() == Order::Limit && rest_amount > 0) {
      order.set_amount(rest_amount);
      if (
          order_book_[order.direction()].find(order.price()) == order_book_[order.direction()].end()
      ) {
        order_book_[order.direction()].emplace(order.price(), Quote{order.price(), order.direction()});
      }
      order_book_[order.direction()].find(order.price())->second.push(std::move(order));
    }

    return trades;
  }

  L1Snapshot build_l1_snapshot() const {
    return
        L1Snapshot{
            !order_book_[Direction::Bid].empty() ?
                std::optional<L1Snapshot::Quote>(
                    L1Snapshot::Quote{
                        order_book_[Direction::Bid].begin()->second.price(),
                        order_book_[Direction::Bid].begin()->second.aggregated_amount()
                    }
                ) :
                std::nullopt,
            !order_book_[Direction::Ask].empty() ?
                std::optional<L1Snapshot::Quote>(
                    L1Snapshot::Quote{
                      order_book_[Direction::Ask].begin()->second.price(),
                      order_book_[Direction::Ask].begin()->second.aggregated_amount()
                    }
                ) :
                std::nullopt
        };
  }

  L2Snapshot build_l2_snapshot() const {
    std::vector<L2Snapshot::Quote> bid_quotes;
    for (const auto& [price, quote] : order_book_[Direction::Bid]) {
      bid_quotes.emplace_back(L2Snapshot::Quote{quote.price(), quote.aggregated_amount()});
    }
    std::vector<L2Snapshot::Quote> ask_quotes;
    for (const auto& [price, quote] : order_book_[Direction::Ask]) {
      ask_quotes.emplace_back(L2Snapshot::Quote{quote.price(), quote.aggregated_amount()});
    }
    return L2Snapshot{bid_quotes, ask_quotes};
  }

private:
  OrderBook order_book_ = {DirectionContainer(Direction::Bid), DirectionContainer(Direction::Ask)};
};

}  // namespace market::matching_engine
