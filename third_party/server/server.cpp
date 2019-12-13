namespace third_party {

namespace {

const size_t kBufferSize = 4096;

class ConnectionHandler : public boost::enable_shared_from_this<ConnectionHandler> {
public:
  using Pointer = boost::shared_ptr<ConnectionHandler>;

  ConnectionHandler(boost::asio::io_service& io_service):
    socket_(io_service) {}

  static pointer create(boost::asio::io_service& io_service) {
    return pointer(new ConnectionHandler(io_service));
  }

  tcp::socket& socket() { return socket_; }

  void send_message(const std::string& message) {
    socket_.async_write_some(
        boost::asio::buffer(message, kBufferSize),
        boost::bind(
          &ConnectionHandler::handle_write,
          shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred)
    );
  }

  void start() {
      socket_.async_read_some(
          boost::asio::buffer(buffer_, kBufferSize),
          boost::bind(&ConnectionHandler::handle_read,
                      shared_from_this(),
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred));
  }

  void handle_read(const boost::system::error_code& err, size_t bytes_transferred) {
    if (!err) {
        socket_.async_read_some(
            boost::asio::buffer(buffer_, kBufferSize),
            boost::bind(&ConnectionHandler::handle_read,
                        shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    } else {
         std::cerr << "error: " << err.message() << std::endl;
         socket_.close();
    }
  }

  void handle_write(const boost::system::error_code& err, size_t bytes_transferred) {
    if (!err) {
       cout << "Server sent Hello message!"<< endl;
    } else {
       std::cerr << "error: " << err.message() << endl;
       socket_.close();
    }
  }

private:
  tcp::socket socket_;
  std::string message="Hello From Server!";
  char buffer_[kBufferSize];
};

}  // namespace

TcpServer::TcpServer(size_t port, MessageProcessor* message_processor) {
  try {
    ConnectionHandler::pointer connection = ConnectionHandler::create(acceptor_.get_io_service());

    // asynchronous accept operation and wait for a new connection.
    acceptor_.async_accept(connection->socket(),
        boost::bind(&Server::handle_accept, this, connection,
          boost::asio::placeholders::error));

    io_service.run();
  } catch(std::exception& e) {
    std::cerr << e.what() << endl;
  }
}

TcpServer::send_message(const std::string& message) {

}

}  // namespace third_party
