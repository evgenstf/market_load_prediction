#include "trade_tream.h"

#include <iostream>

using namespace std;
using namespace market::trade_stream;

void process_command_internal(market::trade_stream::Command command) {
  std::clog << "process command: " << command.name << std::endl;
}

int main() {
  std::clog << "Open trade_stream on port 1234" << std::endl;
  TradeStream trade_stream(&process_command_internal, 1234);
}
