#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <iostream>

namespace third_party {

template<class ConnectionHandler>
class TcpServer {
public:
  explicit TcpServer(size_t port, const std::function<std::string(char*, size_t)>& process_message_implementation):
    acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    process_message_implementation_(process_message_implementation) {}

  void start() {
    wait_new_connection();
    io_service_.run();
  }

private:
  void wait_new_connection() {
    auto* connection_handler = new ConnectionHandler(io_service_, process_message_implementation_);
    acceptor_.async_accept(
        connection_handler->socket(),
        boost::bind(
          &TcpServer::handle_accept_connection,
          this,
          connection_handler,
          boost::asio::placeholders::error
        )
    );
  }

  void handle_accept_connection(
      ConnectionHandler* connection_handler,
      const boost::system::error_code& error
  ) {
    if (error) {
      std::clog << "TcpServer::handle_accept_connection error received: " << error << std::endl;
    }
    connection_handler->start_session();
    wait_new_connection();
  }

  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;

  std::function<std::string(char*, size_t)> process_message_implementation_;
};

}  // namespace third_party
