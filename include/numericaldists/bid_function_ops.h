#ifndef _NUMERICALDISTS_BID_FUNCTION_OPS_H_
#define _NUMERICALDISTS_BID_FUNCTION_OPS_H_

#include "numericaldists/bounds.h"
#include "numericaldists/distribution.h"
#include "numericaldists/equal_length_piecewise_function.h"
#include "numericaldists/interval.h"
#include "numericaldists/piecewise_linear_function.h"

namespace numericaldists {

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

}  // namespace numericaldists

#endif  // _NUMERICALDISTS_BID_FUNCTION_OPS_H_
