#pragma once

#include <vector>
#include <tuple>

#include "core/coordinate.h"

namespace DSPC::LinearRegression::Serial
{
  double CalculateGradient(const std::vector<Coordinate> &coordinates);
  double CalculateYIntercept(const std::vector<Coordinate> &coordinates, const double gradient);

  std::tuple<double, double> CalculateGradient(const std::vector<MultivariateCoordinate> &mc);
  double CalculateYIntercept(const std::vector<MultivariateCoordinate> &mc, const std::tuple<double, double> gradient);
}
