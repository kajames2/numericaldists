#include "bidding/bid_function_ops.h"

#include <boost/math/quadrature/gauss.hpp>
#include "bidding/combination_generation.h"
#include "bidding/distribution.h"
#include "bidding/equal_length_piecewise_function.h"
#include "bidding/line_ops.h"
#include "bidding/piecewise_linear_function.h"

#include <algorithm>
#include <functional>
#include <vector>

namespace bidding {

EqualLengthPiecewiseFunction ResampleFunction(
    const std::function<float(float)>& func, Interval interval, int n_samples) {
  auto x_samples = GetMesh(interval, n_samples);
  std::vector<float> y_samples(n_samples);
  std::transform(x_samples.begin(), x_samples.end(), y_samples.begin(), func);
  auto segments = PointsToLines(x_samples, y_samples);
  return EqualLengthPiecewiseFunction(segments);
}

PiecewiseLinearFunction ApproximateInverse(
    const std::function<float(float)>& func, Interval interval, int n_samples) {
  auto x_samples = GetMesh(interval, n_samples);
  std::vector<float> y_samples(n_samples);
  std::transform(x_samples.begin(), x_samples.end(), y_samples.begin(), func);
  auto segments = PointsToLines(y_samples, x_samples);
  return PiecewiseLinearFunction(segments);
}

EqualLengthPiecewiseFunction ApproximateDerivative(
    const std::function<float(float)>& func, Interval interval, int n_samples) {
  float d = GetSpan(interval) / n_samples;
  auto xd_samples = GetMesh(interval, n_samples + 1);
  auto x_samples =
      GetMesh(Interval{interval.min + d / 2, interval.max - d / 2}, n_samples);
  std::vector<float> yd_samples(n_samples + 1);
  std::transform(xd_samples.begin(), xd_samples.end(), yd_samples.begin(),
                 func);
  std::vector<float> y_samples(n_samples);
  for (int i = 0; i < n_samples; ++i) {
    y_samples[i] = (yd_samples[i + 1] - yd_samples[i]) / d;
  }
  auto segments = PointsToLines(x_samples, y_samples);
  return EqualLengthPiecewiseFunction(segments);
}

PiecewiseLinearFunction ApproximateRandomVariableFunctionCDF(
    const Distribution& dist, const std::function<float(float)>& func,
    int n_samples) {
  auto prob_samples = GetMesh(Interval{0.000001, 0.999999}, n_samples);
  std::vector<float> quant_samples(n_samples);
  std::transform(prob_samples.begin(), prob_samples.end(),
                 quant_samples.begin(),
                 [dist](float x) { return quantile(dist, x); });
  std::vector<float> f_samples(n_samples);
  std::transform(quant_samples.begin(), quant_samples.end(), f_samples.begin(),
                 func);
  std::sort(f_samples.begin(), f_samples.end());
  auto segments = PointsToLines(f_samples, prob_samples);
  return PiecewiseLinearFunction(segments);
}

// PiecewiseLinearFunction ApproximateRandomVariableFunctionCDF(
//     const Distribution& dist, const std::function<float(float, float)>& func, int n_samples) {
//   auto prob_samples = GetMesh(Interval{0.000001, 0.999999}, Interval{0.000001, 0.999999}, n_samples, n_samples);
//   std::vector<float> quant_samples(n_samples);
//   std::transform(prob_samples.begin(), prob_samples.end(),
//                  quant_samples.begin(),
//                  [dist](float x) { return quantile(dist, x); });
//   std::vector<float> f_samples(n_samples);
//   std::transform(quant_samples.begin(), quant_samples.end(), f_samples.begin(),
//                  func);
//   std::sort(f_samples.begin(), f_samples.end());
//   auto segments = PointsToLines(f_samples, prob_samples);
//   return PiecewiseLinearFunction(segments);
// }

std::vector<float> ApproximateKthLowestOrderStatisticCDFHelper(
    const std::vector<std::function<float(float)>>& cdfs, Interval interval,
    int k, int n_samples) {
  int n = cdfs.size();
  assert(n >= 0 && k <= n);
  auto x_samples = GetMesh(interval, n_samples);
  std::vector<float> cdf_samples(n_samples, 0);

  auto comb = bidding::GetFirstCanonicalCombination(k);
  while (comb < (1<<n)) {
    for (int i = 0; i < n_samples; ++i) {
      float prob = 1;
      for (int j = 0; j < n; ++j) {
        float prob_below = cdfs[j](x_samples[i]);
        unsigned int above = (comb >> j) & 1;
        prob *= above * prob_below + (1 - above) * (1 - prob_below);
      }
      cdf_samples[i] += prob;
    }
    comb = bidding::GetNextCanonicalCombination(comb);
  }
  if (k < n) {
    auto rest_cdf = ApproximateKthLowestOrderStatisticCDFHelper(cdfs, interval, k+1, n_samples);
    std::transform(cdf_samples.begin(), cdf_samples.end(), rest_cdf.begin(), cdf_samples.begin(), std::plus<float>());
  }
  return cdf_samples;
}

EqualLengthPiecewiseFunction ApproximateKthLowestOrderStatisticCDF(
    const std::vector<std::function<float(float)>>& cdfs, Interval interval,
    int k, int n_samples)
{
  auto x_samples = GetMesh(interval, n_samples);
  auto cdf_samples = ApproximateKthLowestOrderStatisticCDFHelper(cdfs, interval, k, n_samples);
  auto segments = PointsToLines(x_samples, cdf_samples);
  return EqualLengthPiecewiseFunction(segments);
}

EqualLengthPiecewiseFunction ApproximateHighestOrderStatisticCDF(
    const std::vector<std::function<float(float)>>& cdfs, Interval interval,
    int n_samples) {
  auto x_samples = GetMesh(interval, n_samples);
  std::vector<float> cdf_samples(n_samples);
  std::transform(x_samples.begin(), x_samples.end(), cdf_samples.begin(),
                 [cdfs](float x) {
                   float tot = 1;
                   for (const auto& cdf : cdfs) {
                     tot *= cdf(x);
                   }
                   return tot;
                 });
  auto segments = PointsToLines(x_samples, cdf_samples);
  return EqualLengthPiecewiseFunction(segments);
}

EqualLengthPiecewiseFunction ApproximateHighestOrderStatisticCDF(
    const std::vector<Distribution>& dists,
    const std::vector<std::function<float(float)>>& funcs, Interval interval,
    int n_samples) {
  std::vector<std::function<float(float)>> bid_cdfs;
  bid_cdfs.reserve(funcs.size());
  for (int i = 0; i < funcs.size(); ++i) {
    bid_cdfs.push_back(
        ApproximateRandomVariableFunctionCDF(dists[i], funcs[i], n_samples));
  }
  return ApproximateHighestOrderStatisticCDF(bid_cdfs, interval, n_samples);
}

EqualLengthPiecewiseFunction ApproximateExpectedValueFunction(
    const std::function<float(float)>& cdf, Interval interval, int n_samples) {
  auto x_samples = GetMesh(interval, n_samples);
  auto pdf = ApproximateDerivative(cdf, interval, n_samples);
  std::vector<float> exp_samples(n_samples);
  exp_samples[0] = x_samples[0];
  float tot = x_samples[0] * cdf(x_samples[0]);
  for (int i = 1; i < exp_samples.size(); ++i) {
    tot += boost::math::quadrature::gauss<float, 7>::integrate(
        [pdf](float x) { return x * pdf(x); }, x_samples[i - 1], x_samples[i]);
    exp_samples[i] = std::min(tot / cdf(x_samples[i]), x_samples[i]);
  }
  auto segments = PointsToLines(x_samples, exp_samples);
  return EqualLengthPiecewiseFunction(segments);
}

}  // namespace bidding
