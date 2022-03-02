#pragma once

#include <vector>

#include "core/coordinate.h"

namespace DSPC::LinearRegression::Serial
{
  double CalculateGradient(const std::vector<Coordinate>& coordinates);
  double CalculateYIntercept(const std::vector<Coordinate>& coordinates, const double gradient);
} 
