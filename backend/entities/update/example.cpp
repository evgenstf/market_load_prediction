#include "command.h"

#include <iostream>

using namespace std;
using namespace market::entities;


int main() {
  auto command = command_from_string(R"({"type":"new_order", "price": 10, "amount": 5, "direction": "bid"})");
  print_command(*command);
}
