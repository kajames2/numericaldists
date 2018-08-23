#ifndef _BIDDING_EQUAL_LENGTH_PIECEWISE_FUNCTION_
#define _BIDDING_EQUAL_LENGTH_PIECEWISE_FUNCTION_

#include <ostream>
#include <vector>

#include "bidding/piecewise_linear_function.h"

namespace bidding {

class EqualLengthPiecewiseFunction : public PiecewiseLinearFunction {
 public:
  EqualLengthPiecewiseFunction() {}
  EqualLengthPiecewiseFunction(const std::vector<float>& ys, Interval interval)
      : PiecewiseLinearFunction(ys, interval),
        segment_length_((interval.max - interval.min) / (ys.size() - 1)),
        min_(interval.min) {}
  EqualLengthPiecewiseFunction(const std::vector<float>& xs,
                               const std::vector<float>& ys)
      : PiecewiseLinearFunction(xs, ys),
        segment_length_((xs.back() - xs.front()) / (xs.size() - 1)),
        min_(xs.front()) {}
  EqualLengthPiecewiseFunction(std::vector<LineSegment> segments)
      : PiecewiseLinearFunction(segments) {
    min_ = segments_.front().GetXInterval().min;
    float max = segments_.back().GetXInterval().max;
    segment_length_ = (max - min_) / segments_.size();
  }
  LineSegment GetLine(float x) const override {
    return segments_[(x - min_) / segment_length_];
  }

 private:
  float segment_length_;
  float min_;
};

}  // namespace bidding

#endif  // _BIDDING_EQUAL_LENGTH_PIECEWISE_FUNCTION_
