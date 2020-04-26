#include <iostream>

#include "../entities/ring_buffer/ring_buffer.h"
#include "../entities/command/command.h"
#include "../command_receiver/command_receiver.h"
#include "../matching_engine/matching_engine.h"

using namespace std;
using namespace market::entities;
using namespace market::matching_engine;
using namespace market::command_receiver;

void execute_main_loop() {
  std::cerr << "start executing main loop" << std::endl;

  RingBuffer<Command, 1000> ring_buffer;

  CommandReceiver command_receiver(1234, &ring_buffer);
  command_receiver.start();

  MatchingEngine matching_engine;

  while (1) {
    auto command = ring_buffer.pop_when_exists();
    if (command.type == CommandType::NewOrder) {
      Order order(command.price, command.amount, command.direction, Order::Type::Limit);
      matching_engine.add_order(std::move(order));
      auto snapshot = matching_engine.build_l1_snapshot();
      auto best_ask_quote = snapshot.best_quote(Direction::Ask);
      auto best_bid_quote = snapshot.best_quote(Direction::Bid);

      if (best_ask_quote.has_value()) {
        std::clog << "best ask: " << best_ask_quote->price << ' ' << best_ask_quote->amount << std::endl;
      } else {
        std::clog << "best ask: " << "None" << std::endl;
      }

      if (best_bid_quote.has_value()) {
        std::clog << "best bid: " << best_bid_quote->price << ' ' << best_bid_quote->amount << std::endl;
      } else {
        std::clog << "best bid: " << "None" << std::endl;
      }

    }
  }
}

int main() {
  execute_main_loop();
}
