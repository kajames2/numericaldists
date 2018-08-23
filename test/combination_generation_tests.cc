#include <gtest/gtest.h>

#include <vector>

#include "bidding/combination_generation.h"

namespace gatests {

class CombinationGenerationTest : public ::testing::Test {
 public:
  CombinationGenerationTest() {}

 protected:
  virtual void SetUp() {}
};

TEST_F(CombinationGenerationTest, GenerateFirstsTest) {
  EXPECT_EQ(0, bidding::GetFirstCanonicalCombination(0));
  EXPECT_EQ(7, bidding::GetFirstCanonicalCombination(3));
  EXPECT_EQ(31, bidding::GetFirstCanonicalCombination(5));
}

TEST_F(CombinationGenerationTest, GetNextTest) {
  unsigned int val = bidding::GetFirstCanonicalCombination(2);
  EXPECT_EQ(3, val);
  val = bidding::GetNextCanonicalCombination(val);
  EXPECT_EQ(5, val);
  val = bidding::GetNextCanonicalCombination(val);
  EXPECT_EQ(6, val);
}
}  // namespace gatests
