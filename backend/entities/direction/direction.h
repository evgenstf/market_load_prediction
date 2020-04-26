#pragma once
#include <cstdint>
#include <string>

namespace market::entities {

enum Direction : uint8_t {
  Bid = 0,
  Ask = 1
};

std::string direction_to_string(Direction direction) {
  return direction == Direction::Ask ? "ask" : "bid";
}

}  // namespace market::entities
