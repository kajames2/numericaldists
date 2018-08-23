#include <gtest/gtest.h>

#include <vector>

#include "numericaldists/piecewise_linear_function.h"

namespace gatests {

using namespace numericaldists;

class PiecewiseLinearFunctionTest : public ::testing::Test {
 public:
  PiecewiseLinearFunctionTest() {}

 protected:
  virtual void SetUp() {}
  PiecewiseLinearFunction func = PiecewiseLinearFunction(
      std::vector<float>{10, 20, 20, 50}, Interval{0, 30});
  PiecewiseLinearFunction horizontal_func =
      PiecewiseLinearFunction(std::vector<float>{10, 10}, Interval{0, 30});
  PiecewiseLinearFunction vertical_func =
      PiecewiseLinearFunction(std::vector<float>{10, 20}, Interval{30, 30});
  PiecewiseLinearFunction decreasing_func =
      PiecewiseLinearFunction(std::vector<float>{30, 10}, Interval{0, 30});
};

typedef PiecewiseLinearFunctionTest PiecewiseLinearFunctionDeathTest;

TEST_F(PiecewiseLinearFunctionTest, GetBidInterior) {
  EXPECT_EQ(15, func.GetBid(5));
  EXPECT_EQ(20, func.GetBid(10));
  EXPECT_EQ(20, func.GetBid(15));
  EXPECT_EQ(35, func.GetBid(25));
}

TEST_F(PiecewiseLinearFunctionTest, GetBidExtrapolation) {
  EXPECT_EQ(10, func.GetBid(0));
  EXPECT_EQ(50, func.GetBid(30));
  EXPECT_EQ(50, func.GetBid(50));
  EXPECT_EQ(10, func.GetBid(-120));
}

TEST_F(PiecewiseLinearFunctionTest, GetBidVertical) {
  EXPECT_EQ(10, vertical_func.GetBid(0));
  EXPECT_EQ(20, vertical_func.GetBid(50));
}

TEST_F(PiecewiseLinearFunctionTest, GetBidDecreasing) {
  EXPECT_FLOAT_EQ(30, decreasing_func.GetBid(-10));
  EXPECT_FLOAT_EQ(26, decreasing_func.GetBid(6));
  EXPECT_FLOAT_EQ(10, decreasing_func.GetBid(40));
}

TEST_F(PiecewiseLinearFunctionDeathTest, SinglePointFunctionDeathTest) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  EXPECT_DEATH(PiecewiseLinearFunction(std::vector<float>{10}, Interval(0, 30)),
               "");
}

// TEST_F(PiecewiseLinearFunctionTest, InverseFunctionTest) {
//   auto func_inv = func.GetInverseFunction();
//   EXPECT_EQ(0, func_inv.GetBid(10));
//   EXPECT_EQ(5, func_inv.GetBid(15));
//   EXPECT_EQ(20, func_inv.GetBid(20));
//   EXPECT_EQ(25, func_inv.GetBid(35));
//   EXPECT_EQ(30, func_inv.GetBid(50));
//   EXPECT_EQ(func, func_inv.GetInverseFunction());
// }

// TEST_F(PiecewiseLinearFunctionTest, InverseVerticalFunctionTest) {
//   auto func_inv = vertical_func.GetInverseFunction();
//   EXPECT_EQ(30, func_inv.GetBid(-10));
//   EXPECT_EQ(30, func_inv.GetBid(15));
//   EXPECT_EQ(30, func_inv.GetBid(30));
//   EXPECT_EQ(vertical_func, func_inv.GetInverseFunction());
// }

// TEST_F(PiecewiseLinearFunctionTest, InverseHorizontalFunctionTest) {
//   auto func_inv = horizontal_func.GetInverseFunction();
//   EXPECT_EQ(0, func_inv.GetBid(0));
//   EXPECT_EQ(30, func_inv.GetBid(20));
//   EXPECT_EQ(horizontal_func, func_inv.GetInverseFunction());
// }

// TEST_F(PiecewiseLinearFunctionTest, InverseDecreasingFunctionTest) {
//   auto func_inv = decreasing_func.GetInverseFunction();
//   EXPECT_EQ(0, func_inv.GetBid(30));
//   EXPECT_DOUBLE_EQ(6, func_inv.GetBid(26));
//   EXPECT_EQ(30, func_inv.GetBid(10));
//   EXPECT_EQ(decreasing_func, func_inv.GetInverseFunction());
// }

}  // namespace gatests
