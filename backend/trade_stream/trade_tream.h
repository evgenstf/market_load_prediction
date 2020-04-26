#pragma once

#include "../third_party/server/connection_handler.h"
#include "../third_party/server/server.h"

#include <iostream>
#include <optional>
#include <vector>

namespace market::trade_stream {

struct Command {
  std::string name;
  std::string parameter;
};

bool is_letter(char c) { return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'); }

std::optional<Command> parse_command(char* message, size_t length) {
  Command command;

  size_t index = 0;
  for (; index < length && is_letter(message[index]); ++index) {
    command.name += message[index];
  }
  for (; index < length && !is_letter(message[index]); ++index) {
  }
  for (; index < length && is_letter(message[index]); ++index) {
    command.parameter += message[index];
  }

  return command.name.empty() ? std::nullopt : std::make_optional(command);
}

class ConnectionHandler: public third_party::ConnectionHandler {
public:
  ConnectionHandler(boost::asio::io_service& io_service) :
      third_party::ConnectionHandler(io_service) {}

private:
  void process_message(char* message, size_t length) {
    auto command = parse_command(message, length);
    if (command.has_value()) {
      std::clog << "received command: " << command->name << " parameter: " << command->parameter
                << std::endl;
    } else {
      std::clog << "cannot parse command" << std::endl;
    }
  }

  std::vector<Command> commands_;
};

class TradeStream {
public:
  TradeStream(void (*process_command_internal)(Command), size_t port) :
      server_(port), process_command_internal_(process_command_internal) {
    server_.start();
  }

private:
  third_party::TcpServer<ConnectionHandler> server_;
  void (*process_command_internal_)(Command);
};
} // namespace market::trade_stream
