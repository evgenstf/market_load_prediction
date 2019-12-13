#pragma once

namespace third_party {

class TcpServer {
public:
  TcpServer(size_t port, MessageProcessor* message_processor);

  void send_message(const std::string& message);

private:
  MessageProcessor* message_processor_;
  tcp::acceptor acceptor_;
};

}  // namespace third_party
