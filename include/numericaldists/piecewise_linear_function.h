#ifndef _NUMERICALDISTS_PIECEWISE_LINEAR_FUNCTION_
#define _NUMERICALDISTS_PIECEWISE_LINEAR_FUNCTION_

#include <ostream>
#include <vector>

#include "numericaldists/interval.h"
#include "numericaldists/line_segment.h"

namespace numericaldists {

class PiecewiseLinearFunction {
 public:
  PiecewiseLinearFunction() {}
  PiecewiseLinearFunction(const std::vector<float>& ys, Interval interval);
  PiecewiseLinearFunction(const std::vector<float>& xs, const std::vector<float>& ys);
  virtual LineSegment GetLine(float x) const;
  float GetBid(float x) const;
  friend std::size_t hash_value(const PiecewiseLinearFunction& s);
  friend bool operator==(const PiecewiseLinearFunction& g1,
                         const PiecewiseLinearFunction& g2);
  friend std::ostream& operator<<(std::ostream& os,
                                  const PiecewiseLinearFunction& pl);
  PiecewiseLinearFunction(std::vector<LineSegment> segments);
  float operator()(float x) const { return GetBid(x); }
  std::vector<LineSegment> GetSegments() const { return segments_; }
 protected:
  std::vector<LineSegment> segments_;
  Interval interval_;
};
bool operator==(const PiecewiseLinearFunction& g1,
                const PiecewiseLinearFunction& g2);
std::size_t hash_value(const PiecewiseLinearFunction& s);

}  // namespace numericaldists

#endif  // _NUMERICALDISTS_PIECEWISE_LINEAR_FUNCTION_
