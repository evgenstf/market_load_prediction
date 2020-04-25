#pragma once

#include "../entities/ring_buffer/ring_buffer.h"
#include "../entities/command/command.h"

#include "../third_party/server/connection_handler.h"
#include "../third_party/server/server.h"

#include <iostream>
#include <optional>
#include <vector>
#include <thread>

namespace market::command_receiver {

class ConnectionHandler: public third_party::ConnectionHandler {
public:
  ConnectionHandler(
      boost::asio::io_service& io_service,
      const std::function<void(char*, size_t)>& process_message_implementation
  ):
      third_party::ConnectionHandler(io_service),
      process_message_implementation_(process_message_implementation)
  {}

private:
  void process_message(char* message, size_t length) {
    process_message_implementation_(message, length);
  }

  std::function<void(char*, size_t)> process_message_implementation_;
};

class CommandReceiver {
public:
  CommandReceiver(size_t port, entities::RingBuffer<entities::Command, 1000>* ring_buffer) :
      server_(port, [this](char* message, size_t length){
          process_message(message, length);
      }),
      ring_buffer_(ring_buffer)
  { }

  void start() {
    auto start_server_lambda = [](third_party::TcpServer<ConnectionHandler>* server){
      std::clog << "call server start" << std::endl;
      server->start();
    };
    server_thread_ = std::thread(start_server_lambda, &server_);
  }

private:
  void process_message(char* message, size_t length) {
    auto command = entities::command_from_string(std::string(message, message + length));
    if (command.has_value()) {
      std::clog << "received command: ";
      entities::print_command(*command);
      ring_buffer_->push(std::make_unique<entities::Command>(std::move(*command)));
    } else {
      std::clog << "cannot parse command" << std::endl;
    }
  }

  third_party::TcpServer<ConnectionHandler> server_;
  std::thread server_thread_;
  entities::RingBuffer<entities::Command, 1000>* ring_buffer_;
};

} // namespace market::command_receiver
