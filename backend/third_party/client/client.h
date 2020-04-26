#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;

class Client {
public:
  Client(std::string host, size_t port):
    host_(std::move(host)), port_(port) {}

  void send_message(const std::string& message) {
     boost::asio::io_service io_service;
     ip::tcp::socket socket(io_service);
     socket.connect(ip::tcp::endpoint(boost::asio::ip::address::from_string(host_), port_));

     boost::system::error_code error;
     boost::asio::write(socket, boost::asio::buffer(message), error);
     if (error) {
       std::clog << "error while sending message from client: " << error.message() << std::endl;
     }
  }

private:
  const std::string host_;
  const size_t port_;
};
