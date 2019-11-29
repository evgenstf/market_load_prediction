#include "../matching_engine.h"
#include <gtest/gtest.h>

using namespace market::matching_engine;
using namespace market::entities;

TEST(MatchingEngine, L1SnapshotNoOrders) {
  MatchingEngine matching_engine;
  ASSERT_EQ(L1Snapshot(std::nullopt, std::nullopt), matching_engine.build_l1_snapshot());
}

TEST(MatchingEngine, L1SnapshotOneBidOrder) {
  MatchingEngine matching_engine;
  matching_engine.add_order(Order{50.0, 3, Direction::Bid, Order::Type::Limit});
  ASSERT_EQ(L1Snapshot(L1Snapshot::Quote{50.0, 3}, std::nullopt), matching_engine.build_l1_snapshot());
}

TEST(MatchingEngine, L1SnapshotSeveralBidOrders) {
  MatchingEngine matching_engine;
  matching_engine.add_order(Order{50.0, 3, Direction::Bid, Order::Type::Limit});
  matching_engine.add_order(Order{60.0, 3, Direction::Bid, Order::Type::Limit});
  matching_engine.add_order(Order{50.0, 7, Direction::Bid, Order::Type::Limit});
  ASSERT_EQ(L1Snapshot(L1Snapshot::Quote{50.0, 10}, std::nullopt), matching_engine.build_l1_snapshot());
}

TEST(MatchingEngine, L1SnapshotOneBidOneAskOrders) {
  MatchingEngine matching_engine;
  matching_engine.add_order(Order{50.0, 3, Direction::Bid, Order::Type::Limit});
  matching_engine.add_order(Order{100.0, 2, Direction::Ask, Order::Type::Limit});
  ASSERT_EQ(L1Snapshot(L1Snapshot::Quote{50.0, 3}, L1Snapshot::Quote{100.0, 2}), matching_engine.build_l1_snapshot());
}

int main(int argc, char** argv) {
   ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
