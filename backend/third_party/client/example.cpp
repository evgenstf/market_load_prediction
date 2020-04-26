#include "client.h"
#include <iostream>

int main() {
  Client client("127.0.0.1", 1235);
  client.send_message("test_message");
  std::clog << "sent test_message" << std::endl;
  return 0;
}
