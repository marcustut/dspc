#pragma once

#include <vector>
#include <tuple>
#include "core/coordinate.h"

namespace DSPC::LinearRegression::CppStdLib
{
  double CalculateGradient(const std::vector<Coordinate> &coordinates);
  double CalculateYIntercept(const std::vector<Coordinate> &coordinates, const double gradient);

  std::tuple<std::pair<double, double>, double> CalculateGradientAndYIntercept(const std::vector<MultivariateCoordinate> &mc);
}
