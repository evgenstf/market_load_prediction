#include "../matching_engine.h"
#include <gtest/gtest.h>

using namespace market::matching_engine;

TEST(MatchingEngine, NoOrders) {
  ASSERT_EQ(0, 0);
}

int main(int argc, char** argv) {
   ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
