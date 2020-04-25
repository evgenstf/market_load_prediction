#include <iostream>

#include "../entities/ring_buffer/ring_buffer.h"
#include "../command_receiver/command_receiver.h"
#include "../matching_engine/matching_engine.h"

using namespace std;
using namespace market::entities;
using namespace market::matching_engine;
using namespace market::command_receiver;

void execute_main_loop() {
  std::cerr << "start executing main loop" << std::endl;
  MatchingEngine matching_engine();

  RingBuffer<Command, 1000> ring_buffer;

  CommandReceiver command_receiver(1234, &ring_buffer);
  command_receiver.start();

  while (1) {
    std::clog << "wait for next command" << std::endl;
    auto command = ring_buffer.pop_when_exists();
    std::clog << "process command: ";
    print_command(command);
  }
}

int main() {
  execute_main_loop();
}
