#include "command_receiver.h"
#include "../entities/ring_buffer/ring_buffer.h"

#include <iostream>

using namespace std;
using namespace market::command_receiver;
using namespace market::entities;

int main() {
  std::clog << "Open receiver on port 1234" << std::endl;

  RingBuffer<Command, 1000> ring_buffer;

  CommandReceiver receiver(1234, &ring_buffer);
  receiver.start();

  while (1) {
    std::clog << "wait for next command" << std::endl;
    Command command = ring_buffer.pop_when_exists();
    std::clog << "popped from ring buffer: " << command.name << std::endl;
  }
}
