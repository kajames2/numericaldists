#ifndef _BIDDING_LINE_OPS_H_
#define _BIDDING_LINE_OPS_H_

#include "bidding/bounds.h"
#include "bidding/interval.h"
#include "bidding/distribution.h"
#include "bidding/line_segment.h"

namespace bidding {
std::vector<float> GetMesh(Interval interval, int n_lines = 100);
std::vector<LineSegment> PointsToLines(const std::vector<float>& xs, const std::vector<float>& ys);
}  // namespace bidding

#endif  // _BIDDING_LINE_OPS_H_
