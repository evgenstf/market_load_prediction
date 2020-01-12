#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <iostream>

namespace third_party {

template<class ConnectionHandler>
class TcpServer {
public:
  explicit TcpServer(size_t port):
    acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {}

  void start() {
    io_service_.run();
    wait_new_connection();
  }

private:
  void wait_new_connection() {
    connection_handler_storage_.emplace_back(io_service_);
    auto* connection_handler = &connection_handler_storage_.back();
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

  std::vector<ConnectionHandler> connection_handler_storage_;
  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;
};

}  // namespace third_party
