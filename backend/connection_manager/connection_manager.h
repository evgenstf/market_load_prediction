#pragma once

#include "../entities/ring_buffer/ring_buffer.h"
#include "../entities/request/request.h"
#include "../entities/response/response.h"

#include "../third_party/server/connection_handler.h"
#include "../third_party/server/server.h"
#include "../third_party/json/json.hpp"

#include <iostream>
#include <optional>
#include <vector>
#include <thread>

namespace market::connection_manager {

using ResponseRingBuffer = entities::RingBuffer<std::vector<entities::Response>, 1000>;
using RequestRingBuffer =
    entities::RingBuffer<std::pair<entities::Request, ResponseRingBuffer*>, 1000>;


class ConnectionHandler: public third_party::ConnectionHandler {
public:
  ConnectionHandler(
      boost::asio::io_service& io_service,
      const std::function<std::string(char*, size_t)>& process_message_implementation
  ):
      third_party::ConnectionHandler(io_service),
      process_message_implementation_(process_message_implementation)
  {}

private:
  void process_message(char* message, size_t length) {
    send_message(process_message_implementation_(message, length));
  }

  std::function<std::string(char*, size_t)> process_message_implementation_;
};

class ConnectionManager {
public:
  ConnectionManager(
      size_t port,
      RequestRingBuffer* request_ring_buffer
  ):
      server_(port, [this](char* message, size_t length) -> std::string {
          return process_message(message, length);
      }),
      request_ring_buffer_(request_ring_buffer)
  { }

  void start() {
    auto start_server_lambda = [](third_party::TcpServer<ConnectionHandler>* server){
      std::clog << "call server start" << std::endl;
      server->start();
    };
    server_thread_ = std::thread(start_server_lambda, &server_);
  }

private:
  std::string process_message(char* message, size_t length) {
    std::clog << "received message: ";
    auto request = entities::request_from_string(std::string(message, message + length));
    if (request.has_value()) {
      entities::print_request(*request);
      ResponseRingBuffer response_ring_buffer;
      request_ring_buffer_->push(std::make_pair(std::move(*request), &response_ring_buffer));

      std::vector<entities::Response> responses = response_ring_buffer.pop_when_exists();

      std::string result_json = "[";
      for (size_t i = 0; i < responses.size(); ++i) {
        result_json += entities::response_to_string(responses[i]);
        if (i != responses.size() - 1) {
          result_json += ", ";
        }
      }
      result_json += "]";

      return result_json;
    } else {
      std::clog << "cannot parse request" << std::endl;
      return R"({"error": "cannot parse request"})";
    }
  }

  third_party::TcpServer<ConnectionHandler> server_;
  std::thread server_thread_;

  RequestRingBuffer* request_ring_buffer_;
};

} // namespace market::connection_manager
