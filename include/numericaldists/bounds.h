#ifndef _BIDDING_BOUNDS_H_
#define _BIDDING_BOUNDS_H_

#include <cstddef>
#include <ostream>

#include "bidding/interval.h"

namespace bidding {

struct Bounds {
  Bounds() {}
  Bounds(float a, float b) : a(a), b(b) {}
  explicit Bounds(Interval r) : a(r.min), b(r.max) {}
  float a;
  float b;
};

bool operator==(const Bounds& g1, const Bounds& g2);
std::size_t hash_value(const Bounds& s);
std::ostream& operator<<(std::ostream& os, const Bounds& r);

}  // namespace bidding

#endif  // _BIDDING_BOUNDS_H_
