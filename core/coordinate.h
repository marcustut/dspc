#pragma once

#include <array>

namespace DSPC
{
  // Coordinate represents the (x,y) value
  // of a specific coordinate in 2D space.
  struct Coordinate
  {
    double x, y;
  };

  // MultivariateCoordinate represents the ((x1, x2),y) value
  // of a specific coordinate in 2D space.
  struct MultivariateCoordinate
  {
    std::array<double, 2> xs;
    double y;
  };
} // namespace DSPC
