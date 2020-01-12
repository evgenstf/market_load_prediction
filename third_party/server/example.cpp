#include "connection_handler.h"
#include "server.h"

#include <iostream>

using namespace std;
using namespace third_party;

int main() {
  TcpServer<ConnectionHandler> server(1234);
  std::clog << "Open localhost server on port 1234" << std::endl;
  server.start();
}
