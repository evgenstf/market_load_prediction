#pragma once
#include <atomic>
#include <cstddef>
#include <iostream>
#include <memory>

namespace market::entities {

template <class Item, size_t kBufferSize>
class RingBuffer {
public:
  void push(std::unique_ptr<Item> item) {
    while (lock_.test_and_set());
    items_[end_] = std::move(item);
    end_ = next_index(end_);
    if (end_ == begin_) {
      std::clog << "ring buffer overflowed. drop next begin value\n";
      begin_ = next_index(begin_);
    }
    lock_.clear();
  }

  void push(Item&& item) {
    push(std::make_unique<Item>(std::move(item)));
  }

  bool pop_if_exists(std::unique_ptr<Item>& item) {
    bool popped = false;
    while (lock_.test_and_set());
    if (begin_ != end_) {
      item = std::move(items_[begin_]);
      begin_ = next_index(begin_);
      popped = true;
    }
    lock_.clear();
    return popped;
  }

  Item pop_when_exists() {
    std::unique_ptr<Item> item;
    while (!pop_if_exists(item));
    return *item;
  }

  size_t size() {
    size_t result;
    while (lock_.test_and_set());
    if (begin_ <= end_) {
      result = end_ - begin_;
    } else {
      result = end_ + kBufferSize - begin_;
    }
    lock_.clear();
    return result;
  }

private:
  size_t next_index(size_t index) {
    index += 1;
    index %= kBufferSize;
    return index;
  }

  std::unique_ptr<Item> items_[kBufferSize];
  size_t begin_ = 0;
  size_t end_ = 0;
  std::atomic_flag lock_ = false;
};

}  // namespace market::entities
