#include "matching_engine.h"
#include <optional>

namespace market::matching_engine {

namespace {

using namespace entities;

bool closer_to_spread(double left_price, double right_price, Direction direction) {
  return direction == Direction::Bid ? left_price > right_price : left_price < right_price;
}

Direction opposite_direction(Direction direction) {
  return direction == Bid ? Ask : Bid;
}

}  // namespace

MatchingEngine::MatchingEngine() {}

std::vector<Trade> MatchingEngine::add_order(Order&& order) {
  std::vector<Trade> trades;
  auto trade_direction = opposite_direction(order.direction());
  auto rest_amount = order.amount();

  while (
      !order_book_[trade_direction].empty() &&
      rest_amount > 0 &&
      !closer_to_spread(
        order.price(),
        order_book_[trade_direction].begin()->first,
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

    if (trade_order.amount() == 0) {
      trade_quote.pop();
      if (trade_quote.empty()) {
        order_book_[trade_direction].erase(order_book_[trade_direction].begin());
      }
    }
  }

  if (order.type() == Order::Limit) {
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

L1Snapshot MatchingEngine::build_l1_snapshot() const {
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

} // namespace market::matching_engine
