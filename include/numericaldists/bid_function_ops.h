#ifndef _BIDDING_BID_FUNCTION_OPS_H_
#define _BIDDING_BID_FUNCTION_OPS_H_

#include "bidding/bounds.h"
#include "bidding/distribution.h"
#include "bidding/equal_length_piecewise_function.h"
#include "bidding/interval.h"
#include "bidding/piecewise_linear_function.h"

namespace bidding {

EqualLengthPiecewiseFunction ResampleFunction(
    const std::function<float(float)>& func, Interval interval,
    int n_samples = 10001);

PiecewiseLinearFunction ApproximateInverse(
    const std::function<float(float)>& func, Interval interval,
    int n_samples = 1001);

EqualLengthPiecewiseFunction ApproximateDerivative(
    const std::function<float(float)>& func, Interval interval,
    int n_samples = 1001);

PiecewiseLinearFunction ApproximateRandomVariableFunctionCDF(
    const Distribution& dist, const std::function<float(float)>& func,
    int n_samples = 1001);

PiecewiseLinearFunction ApproximateRandomVariableFunctionCDF(
    const Distribution& dist, const std::function<float(float, float)>& func,
    int n_samples = 1001);

EqualLengthPiecewiseFunction ApproximateHighestOrderStatisticCDF(
    const std::vector<std::function<float(float)>>& cdfs, Interval interval,
    int n_samples = 1001);

EqualLengthPiecewiseFunction ApproximateHighestOrderStatisticCDF(
    const std::vector<Distribution>& dists,
    const std::vector<std::function<float(float)>>& funcs, Interval interval,
    int n_samples = 1001);

EqualLengthPiecewiseFunction ApproximateKthLowestOrderStatisticCDF(
    const std::vector<std::function<float(float)>>& cdfs, Interval interval,
    int n, int n_samples = 1001);

EqualLengthPiecewiseFunction ApproximateExpectedValueFunction(
    const std::function<float(float)>& cdf, Interval interval,
    int n_samples = 1001);

}  // namespace bidding

#endif  // _BIDDING_BID_FUNCTION_OPS_H_
