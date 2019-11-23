#include "../quote.h"
#include <gtest/gtest.h>

using namespace market::entities;

TEST(Quote, NoOrders) {
  Quote quote(/*price =*/ 1000);
  ASSERT_EQ(0, quote.aggregated_amount());
}

TEST(Quote, AddAndRemoveOneOrder) {
  Quote quote(/*price =*/ 1000);

  quote.push(Order{1000, 10, Order::Type::Limit});
  ASSERT_EQ(10, quote.aggregated_amount());

  ASSERT_EQ(1000, quote.front().price());
  ASSERT_EQ(10, quote.front().amount());

  quote.pop();
  ASSERT_EQ(0, quote.aggregated_amount());
}

TEST(Quote, AddTenOrders) {
  Quote quote(/*price =*/ 1000);

  uint64_t expected_aggregated_amount = 0;
  for (uint64_t i = 1; i < 11; ++i) {
    expected_aggregated_amount += i * 10;
    quote.push(Order{1000, i * 10, Order::Type::Limit});
    ASSERT_EQ(expected_aggregated_amount, quote.aggregated_amount());
  }

  for (uint64_t i = 1; i < 11; ++i) {
    ASSERT_EQ(i * 10, quote.front().amount());
    expected_aggregated_amount -= quote.front().amount();
    quote.pop();
    ASSERT_EQ(expected_aggregated_amount, quote.aggregated_amount());
  }
}

int main(int argc, char** argv) {
   ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
