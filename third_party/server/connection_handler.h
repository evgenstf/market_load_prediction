#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <iostream>

namespace third_party {

class ConnectionHandler {
public:
  ConnectionHandler(boost::asio::io_service& io_service):
      socket_(io_service) { }

  boost::asio::ip::tcp::socket& socket() {
    return socket_;
  }

  void start_session() {
    wait_message();
  }

protected:
  void process_message(char* message, size_t length) {
    std::clog << "received message: " << message << " length: " << length << std::endl;
  }

private:
  void wait_message() {
    socket_.async_read_some(
        boost::asio::buffer(buffer_, kBufferSize),
        boost::bind(
          &ConnectionHandler::process_message_internal,
          this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred
        )
    );
  }

  void send_message(const std::string& message) {
    boost::asio::async_write(
        socket_,
        boost::asio::buffer(message.data(), message.size()),
        boost::bind(
          &ConnectionHandler::process_send_message_responce,
          this,
          boost::asio::placeholders::error
        )
    );
  }

  void process_message_internal(const boost::system::error_code& error, size_t received_bytes) {
    if (error) {
      std::clog << "ConnectionHandler::process_message_internal received error: " << error << std::endl;
    }
    process_message(buffer_, received_bytes);
    wait_message();
  }

  void process_send_message_responce(const boost::system::error_code& error) {
    if (error) {
      std::clog << "received error while sending message: " << error << std::endl;
    }
  }

  static const size_t kBufferSize = 2048;

  boost::asio::ip::tcp::socket socket_;
  char buffer_[kBufferSize];
};

}  // namespace third_party
