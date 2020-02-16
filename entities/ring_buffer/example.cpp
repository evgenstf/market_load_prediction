#include "ring_buffer.h"
#include <string>
#include <thread>

using namespace std;
using namespace market::entities;

int main() {
  RingBuffer<std::string, 1000000> buffer;
  for (int i = 0; i < 999999; ++i) {
    buffer.push(make_unique<string>("string"));
  }

  cout << "size: " << buffer.size() << endl;

  auto process = [](size_t thread_index, RingBuffer<std::string, 1000000>* buffer) {
    cout << "start thread: " << thread_index << endl;
    size_t cnt = 0;
    while (buffer->size()) {
      unique_ptr<string> item;
      if (buffer->pop_if_exists(item)) {
        ++cnt;
      }
    }
    cout << "processed items: " << cnt << endl;
  };


  thread process_threads[10];

  for (int i = 0; i < 10; ++i) {
    process_threads[i] = thread(process, i, &buffer);
  }

  for (int i = 0; i < 10; ++i) {
    process_threads[i].join();
  }

  return 0;
}
