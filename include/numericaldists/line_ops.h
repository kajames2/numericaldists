#ifndef _NUMERICALDISTS_LINE_OPS_H_
#define _NUMERICALDISTS_LINE_OPS_H_

#include "numericaldists/bounds.h"
#include "numericaldists/interval.h"
#include "numericaldists/distribution.h"
#include "numericaldists/line_segment.h"

namespace numericaldists {
std::vector<float> GetMesh(Interval interval, int n_lines = 100);
std::vector<LineSegment> PointsToLines(const std::vector<float>& xs, const std::vector<float>& ys);
}  // namespace numericaldists

#endif  // _NUMERICALDISTS_LINE_OPS_H_
