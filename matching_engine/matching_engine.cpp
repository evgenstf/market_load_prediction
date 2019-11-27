#include "matching_engine.h"

namespace market::matching_engine {

MatchingEngine::MatchingEngine(Parameters parameters):
    parameters_(std::move(parameters)) { }

std::vector<entities::Trade> MatchingEngine::add_order(entities::Order&& order) {
  return {};
}

entities::L1Snapshot MatchingEngine::build_l1_snapshot() const {
  return
      entities::L1Snapshot{
          entities::L1Snapshot::Quote{0.0, 10},
          entities::L1Snapshot::Quote{5.0, 10}
      };
}

} // namespace market::matching_engine
