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

std::atomic<int> open_sockets_ = 0;

using RequestRingBuffer =
    entities::RingBuffer<std::pair<entities::Request, std::shared_ptr<ResponseRingBuffer>>, 1000>;


class ConnectionHandler: public third_party::ConnectionHandler {
public:
  ConnectionHandler(
      boost::asio::io_service& io_service,
      const std::function<std::shared_ptr<ResponseRingBuffer>(char*, size_t)>& process_message_implementation
  ):
      third_party::ConnectionHandler(io_service),
      process_message_implementation_(process_message_implementation)
  {}

private:
  void process_message(char* message, size_t length) {
    try {
      auto response_ring_buffer = process_message_implementation_(message, length);

      auto* response_thread = new std::thread([this](std::shared_ptr<ResponseRingBuffer> response_ring_buffer) {
          if (open_sockets_ > 10) {
            response_ring_buffer->push({entities::Response(R"({"error": "server overloaded: too many connections"})")});
            return;
          }
          auto start = std::chrono::high_resolution_clock::now();
          open_sockets_++;
          auto socket = socket_;
          std::vector<entities::Response> responses = response_ring_buffer->pop_when_exists();

          std::string result_json = "[";
          for (size_t i = 0; i < responses.size(); ++i) {
            result_json += entities::response_to_string(responses[i]);
            if (i != responses.size() - 1) {
              result_json += ", ";
            }
          }
          result_json += "]";

          std::clog << "send message " << std::endl;
          try {
            send_message(result_json, *socket);
          } catch (...) {}
          std::clog << "sended message " << std::endl;
          socket_->close();
          open_sockets_--;
          std::clog << "open sockets: " << open_sockets_ << std::endl;
          std::clog << "request processed in "  << (std::chrono::high_resolution_clock::now() - start).count() / 1000 << " ms" << std::endl;
        },
        std::move(response_ring_buffer)
      );
    } catch (...) {}
  }

  std::function<std::shared_ptr<ResponseRingBuffer>(char*, size_t)> process_message_implementation_;

};

class ConnectionManager {
public:
  ConnectionManager(
      size_t port,
      RequestRingBuffer* request_ring_buffer
  ):
      server_(port, [this](char* message, size_t length) -> std::shared_ptr<ResponseRingBuffer> {
          return process_message(message, length);
      }),
      request_ring_buffer_(request_ring_buffer)
  { }

  void start() {
    auto start_server_lambda = [](third_party::TcpServer<ConnectionHandler>* server){
      server->start();
    };
    server_thread_ = std::thread(start_server_lambda, &server_);
  }

private:
  std::shared_ptr<ResponseRingBuffer> process_message(char* message, size_t length) {
    std::clog << "received message: ";
    auto request = entities::request_from_string(std::string(message, message + length));
    std::shared_ptr<ResponseRingBuffer> response_ring_buffer = std::make_shared<ResponseRingBuffer>();

    if (request.has_value()) {
      entities::print_request(*request);
      request_ring_buffer_->push(std::make_pair(std::move(*request), response_ring_buffer));
    } else {
      std::clog << "cannot parse request" << std::endl;
      response_ring_buffer->push({entities::Response(R"({"error": "cannot parse request"})")});
    }
    return response_ring_buffer;
  }

  third_party::TcpServer<ConnectionHandler> server_;
  std::thread server_thread_;

  RequestRingBuffer* request_ring_buffer_;
};

} // namespace market::connection_manager
