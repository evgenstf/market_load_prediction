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
      auto trades = matching_engine.add_order(std::move(order));

      std::vector<Response> responses;
      for (const auto& trade : trades) {
        responses.emplace_back(trade);
      }
      responses.emplace_back(matching_engine.build_l2_snapshot());

      /*
      for (const auto& response : responses) {
        std::clog << "response: " << response_to_string(response) << std::endl;
      }
      */

      response_ring_buffer->push(std::move(responses));
    }
  }
}

int main() {
  execute_main_loop();
}
