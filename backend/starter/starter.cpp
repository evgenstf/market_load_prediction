#include <iostream>

#include "../entities/ring_buffer/ring_buffer.h"
#include "../entities/request/request.h"
#include "../entities/response/response.h"
#include "../connection_manager/connection_manager.h"
#include "../matching_engine/matching_engine.h"
#include "../user_storage/user_storage.h"

using namespace std;
using namespace market::entities;
using namespace market::matching_engine;
using namespace market::connection_manager;
using namespace market::user_storage;

void execute_main_loop() {
  std::cerr << "start executing main loop" << std::endl;

  RequestRingBuffer request_ring_buffer;

  ConnectionManager connection_manager(1234, &request_ring_buffer);
  connection_manager.start();

  MatchingEngine matching_engine;
  UserStorage user_storage;
  user_storage.add_user(User{"evgenstf", 1e18, 1000000});
  user_storage.add_user(User{"test_1", 5000.0, 100});
  user_storage.add_user(User{"test_2", 5000.0, 100});

  size_t next_order_id = 0;

  while (1) {
    auto [request, response_ring_buffer] = request_ring_buffer.pop_when_exists();


    std::vector<Response> responses;
    if (request.type == RequestType::NewOrder) {
      Order order(
          request.price,
          request.amount,
          request.direction,
          Order::Type::Limit,
          next_order_id++,
          request.user
      );

      auto error = user_storage.process_order(order);
      if (error.has_value()) {
        response_ring_buffer->push({Response(*error)});
        continue;
      }

      auto trades = matching_engine.add_order(std::move(order));
      user_storage.process_trades(trades);

      for (const auto& trade : trades) {
        responses.emplace_back(trade);
      }

      /*
      for (const auto& response : responses) {
        std::clog << "response: " << response_to_string(response) << std::endl;
      }
      */

    } else if (request.type == RequestType::CancelOrder) {
      matching_engine.cancel_order(request.cancel_order_id);
      user_storage.process_cancel_order(request.cancel_order_id);
    }

    responses.emplace_back(matching_engine.build_l2_snapshot());
    const auto& user = user_storage.user(request.user);
    responses.emplace_back(user);
    response_ring_buffer->push(std::move(responses));
  }
}

int main() {
  execute_main_loop();
}
