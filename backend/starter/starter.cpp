#include <iostream>

#include "../entities/ring_buffer/ring_buffer.h"
#include "../entities/request/request.h"
#include "../entities/response/response.h"
#include "../connection_manager/connection_manager.h"
#include "../matching_engine/matching_engine.h"

using namespace std;
using namespace market::entities;
using namespace market::matching_engine;
using namespace market::connection_manager;

void execute_main_loop() {
  std::cerr << "start executing main loop" << std::endl;

  RequestRingBuffer request_ring_buffer;

  ConnectionManager connection_manager(1234, &request_ring_buffer);
  connection_manager.start();

  MatchingEngine matching_engine;

  while (1) {
    auto [request, response_ring_buffer] = request_ring_buffer.pop_when_exists();
    if (request.type == RequestType::NewOrder) {
      Order order(request.price, request.amount, request.direction, Order::Type::Limit);
      matching_engine.add_order(std::move(order));
      auto snapshot = matching_engine.build_l1_snapshot();
      auto best_ask_quote = snapshot.best_quote(Direction::Ask);
      auto best_bid_quote = snapshot.best_quote(Direction::Bid);

      if (best_ask_quote.has_value()) {
        std::clog << "best ask: " << best_ask_quote->price << ' ' << best_ask_quote->amount << std::endl;
      } else {
        std::clog << "best ask: " << "None" << std::endl;
      }

      if (best_bid_quote.has_value()) {
        std::clog << "best bid: " << best_bid_quote->price << ' ' << best_bid_quote->amount << std::endl;
      } else {
        std::clog << "best bid: " << "None" << std::endl;
      }

      Response response;
      response.type = ResponseType::NewTrade;
      response.trade = Trade(10, 10, Direction::Ask, {});
      response_ring_buffer->push(std::move(response));
    }
  }
}

int main() {
  execute_main_loop();
}
