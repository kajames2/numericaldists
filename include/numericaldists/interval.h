#ifndef _BIDDING_INTERVAL_H_
#define _BIDDING_INTERVAL_H_

#include <cassert>
#include <cstddef>
#include <ostream>

namespace bidding {

struct Interval {
  Interval() {}
  Interval(float min, float max) : min(min), max(max) { assert(min <= max); }
  float min;
  float max;
};

bool InInterval(Interval interval, float val);
float GetSpan(Interval interval);
bool operator==(const Interval& g1, const Interval& g2);
std::size_t hash_value(const Interval& s);
std::ostream& operator<<(std::ostream& os, const Interval& r);

}  // namespace bidding

#endif  // _BIDDING_INTERVAL_H_
