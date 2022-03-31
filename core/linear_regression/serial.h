#pragma once

#include <vector>

#include "core/coordinate.h"

namespace DSPC::LinearRegression::Serial
{
  double CalculateGradient(const std::vector<Coordinate> &coordinates);
  double CalculateYIntercept(const std::vector<Coordinate> &coordinates, const double gradient);

  double CalculateGradient(const std::vector<MultivariateCoordinate> &multivariate_coordinates);
  double CalculateYIntercept(const std::vector<MultivariateCoordinate> &multivariate_coordinates, const double gradient);
}
