#include "request.h"

#include <iostream>

using namespace std;
using namespace market::entities;


int main() {
  auto request = request_from_string(R"({"type":"new_order", "price": 10, "amount": 5, "direction": "bid"})");
  print_request(*request);
}
