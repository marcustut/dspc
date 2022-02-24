#pragma once

#include <vector>

#include "core/coordinate.h"

namespace DSPC::LinearRegression
{
  class LeastSquare
  {
  private:
    // coordinates is the set of (x, y) coordinates
    std::vector<Coordinate> coordinates;
    // m is the gradient, c is the y-intercept
    double m, c;

    // CalculateGradient calculates gradient of straight line
    // based on given coordinates and update the 'm' variable.
    void CalculateGradient();

    // CalculateYIntercept calculates y-intercept of straight line
    // based on given coordinates and update the 'c' variable.
    // NOTE: This should be called only after 'm' is initialized else,
    //       it will throw and exception.
    void CalculateYIntercept();

  public:
    LeastSquare(std::vector<Coordinate> coordinates);
    ~LeastSquare();

    Coordinate PredictX(double Y);
    Coordinate PredictY(double X);
    std::string Formula();
  };
} // namespace DSPC
