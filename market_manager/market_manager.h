#pragma once

#include "../matching_engine/matching_engine.h"
#include "../trade_stream/trade_tream.h"

#include <cstdint>
#include <utility>

namespace market::market_manager {

struct Config {
  uint32_t port;
};

class MarketManager {
public:
  MarketManager(Config config) : config_(std::move(config)) {}

private:
  matching_engine::MatchingEngine matching_engine_;
  trade_stream::TradeStream trade_stream_;
  Config config_;
};
} // namespace market::market_manager
