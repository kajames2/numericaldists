#ifndef _BIDDING_COMBINATION_GENERATION_H_
#define _BIDDING_COMBINATION_GENERATION_H_

#include <vector>

namespace bidding {

unsigned int GetFirstCanonicalCombination(int r);
unsigned int GetNextCanonicalCombination(unsigned int v);

}  // namespace bidding

#endif  // _BIDDING_COMBINATION_GENERATION_H_
