#include "connection_handler.h"
#include "server.h"

#include <iostream>

using namespace std;
using namespace third_party;

int main() {
  TcpServer<ConnectionHandler> server(1234);
  server.start();
}
