#include "numericaldists/piecewise_linear_function.h"

#include <algorithm>
#include <boost/container_hash/hash.hpp>
#include <cassert>
#include <cstddef>
#include <iostream>

#include "numericaldists/bounds.h"
#include "numericaldists/interval.h"
#include "numericaldists/line_segment.h"

namespace numericaldists {

PiecewiseLinearFunction::PiecewiseLinearFunction(const std::vector<float>& ys,
                                                 Interval interval)
    : interval_(interval) {
  assert(ys.size() > 1);
  int n_lines = ys.size() - 1;
  float interval_size = (interval.max - interval.min) / n_lines;
  std::vector<float> xs;
  xs.reserve(n_lines + 1);
  for (int i = 0; i <= n_lines; ++i) {
    xs.emplace_back(i * interval_size + interval.min);
  }
  segments_.reserve(n_lines);
  for (int i = 0; i < n_lines; ++i) {
    segments_.emplace_back(Interval{xs[i], xs[i + 1]},
                           Bounds{ys[i], ys[i + 1]});
  }
}

LineSegment PiecewiseLinearFunction::GetLine(float x) const {
  return *std::find_if(
      segments_.begin(), segments_.end(),
      [x](const LineSegment& line) { return line.IsInInterval(x); });
}

float PiecewiseLinearFunction::GetBid(float x) const {
  if (x <= interval_.min) {
    return segments_[0].GetYBounds().a;
  } else if (x >= interval_.max) {
    return segments_[segments_.size() - 1].GetYBounds().b;
  }
  auto line = GetLine(x);
  return line.GetY(x);
}

PiecewiseLinearFunction::PiecewiseLinearFunction(
    std::vector<LineSegment> segments) {
  std::stable_sort(segments.begin(), segments.end(),
                   [](const LineSegment& l1, const LineSegment& l2) {
                     if (l1.GetXInterval().min != l2.GetXInterval().min) {
                       return l1.GetXInterval().min < l2.GetXInterval().min;
                     } else {
                       return l1.GetXInterval().max < l2.GetXInterval().max;
                     }
                   });
  interval_ = Interval{segments.front().GetXInterval().min,
                       segments.back().GetXInterval().max};
  segments_ = segments;
}

bool operator==(const PiecewiseLinearFunction& g1,
                const PiecewiseLinearFunction& g2) {
  return g1.segments_ == g2.segments_;
}

std::size_t hash_value(const PiecewiseLinearFunction& s) {
  boost::hash<std::vector<LineSegment>> hasher;
  return hasher(s.segments_);
}

std::ostream& operator<<(std::ostream& os, const PiecewiseLinearFunction& pl) {
  const auto separator = "\n";
  const auto* sep = "";
  for (auto ls : pl.segments_) {
    os << sep << ls;
    sep = separator;
  }
  return os;
}

}  // namespace numericaldists
