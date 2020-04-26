#include "response.h"

#include <iostream>

using namespace std;
using namespace market::entities;


int main() {
  Response response;
  response.type = ResponseType::NewTrade;
  response.trade = Trade(10, 10, Direction::Ask, {});
  std::clog << "response: " << response_to_string(response) << std::endl;
}
