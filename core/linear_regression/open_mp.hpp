#pragma once

#include <vector>
// #include "core/linear_regression/open_mp.hpp"
#include "core/coordinate.h"

namespace DSPC::LinearRegression::OpenMP
{
  double CalculateGradient(const std::vector<Coordinate> &coordinates);
  double CalculateYIntercept(const std::vector<Coordinate> &coordinates, const double gradient);
  std::tuple<std::pair<double, double>, double> CalculateGradientAndYIntercept(const std::vector<MultivariateCoordinate> &mc);
}