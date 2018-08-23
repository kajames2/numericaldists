#include <gtest/gtest.h>

#include <vector>

#include "bidding/interval.h"

namespace gatests {

class IntervalTest : public ::testing::Test {
 public:
  IntervalTest() {}

 protected:
  virtual void SetUp() {}
};

TEST_F(IntervalTest, InIntervalTest) {
  auto interval = bidding::Interval{5, 10};
  EXPECT_FALSE(bidding::InInterval(interval, 0));
  EXPECT_TRUE(bidding::InInterval(interval, 5));
  EXPECT_TRUE(bidding::InInterval(interval, 8));
  EXPECT_FALSE(bidding::InInterval(interval, 10));
  EXPECT_FALSE(bidding::InInterval(interval, 15));
}

}  // namespace gatests
