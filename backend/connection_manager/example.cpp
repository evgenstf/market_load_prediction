#include "connection_manager.h"
#include "../entities/ring_buffer/ring_buffer.h"

#include <iostream>

using namespace std;
using namespace market::connection_manager;
using namespace market::entities;

int main() {
  std::clog << "Open receiver on port 1234" << std::endl;

  RequestRingBuffer request_ring_buffer;

  ConnectionManager receiver(1234, &request_ring_buffer);
  receiver.start();

  while (1) {
    std::clog << "wait for next request" << std::endl;
    auto [request, response_ring_buffer] = request_ring_buffer.pop_when_exists();
    std::clog << "popped from ring buffer: " << request_to_string(request) << std::endl;
    Response response;
    response.type = ResponseType::NewTrade;
    response.trade = Trade(10, 10, Direction::Ask, {});
    response_ring_buffer->push(std::move(response));
  }
}
