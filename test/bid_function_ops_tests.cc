#include <gtest/gtest.h>

#include <vector>

#include "numericaldists/bid_function_ops.h"
#include "numericaldists/piecewise_linear_function.h"

namespace gatests {

using namespace numericaldists;

class BidFunctionOpsTest : public ::testing::Test {
 public:
  BidFunctionOpsTest() {}

 protected:
  virtual void SetUp() {}
};

TEST_F(BidFunctionOpsTest, SampleFunctionTest) {
  auto func =
      ResampleFunction([](float x) { return x * x; }, Interval{0, 10}, 101);
  float epsilon = 0.005;
  EXPECT_NEAR(0.01, func(0.1), epsilon);
  EXPECT_NEAR(1, func(1), epsilon);
  EXPECT_NEAR(2, func(1.414), epsilon);
  EXPECT_NEAR(80, func(8.944), epsilon);
}

TEST_F(BidFunctionOpsTest, ApproximateInverseTest) {
  auto inv = ApproximateInverse([](float x) { return x * x; }, Interval{0, 10});
  float epsilon = 0.001;
  EXPECT_NEAR(0.1, inv(0.01), epsilon);
  EXPECT_NEAR(1, inv(1), epsilon);
  EXPECT_NEAR(1.414, inv(2), epsilon);
  EXPECT_NEAR(8.944, inv(80), epsilon);
}

TEST_F(BidFunctionOpsTest, InverseVerticalFunctionTest) {
  float epsilon = 0.001;
  auto vertical_func =
      PiecewiseLinearFunction(std::vector<float>{10, 20}, Interval{30, 30});
  auto func_inv = ApproximateInverse(vertical_func, Interval{30, 30});
  EXPECT_NEAR(30, func_inv.GetBid(-10), epsilon);
  EXPECT_NEAR(30, func_inv.GetBid(15), epsilon);
  EXPECT_NEAR(30, func_inv.GetBid(30), epsilon);
}

TEST_F(BidFunctionOpsTest, InverseHorizontalFunctionTest) {
  float epsilon = 0.001;
  auto horizontal_func = [](float x) { return 10; };
  auto func_inv = ApproximateInverse(horizontal_func, Interval{0, 30});
  EXPECT_NEAR(0, func_inv.GetBid(0), epsilon);
  EXPECT_NEAR(30, func_inv.GetBid(20), epsilon);
}

TEST_F(BidFunctionOpsTest, InverseDecreasingFunctionTest) {
  float epsilon = 0.001;
  auto decreasing_func =
      PiecewiseLinearFunction(std::vector<float>{30, 10}, Interval{0, 30});
  auto func_inv = ApproximateInverse(decreasing_func, Interval{0, 30});
  EXPECT_NEAR(0, func_inv.GetBid(30), epsilon);
  EXPECT_NEAR(6, func_inv.GetBid(26), epsilon);
  EXPECT_NEAR(30, func_inv.GetBid(10), epsilon);
}

// TEST_F(BidFunctionOpsTest, GetOrderStatistic) {
//   std::vector<std::function<float(float)>> funcs(3, [](float x) { return x; });
//   EXPECT_EQ(1, 1);
// }

}  // namespace gatests
