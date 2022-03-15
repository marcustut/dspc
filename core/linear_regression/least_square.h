#pragma once

#include <vector>
#include <string>

#include "core/coordinate.h"
#include "core/technique.h"

namespace DSPC::LinearRegression
{
  class LeastSquare
  {
  private:
    // coordinates is the set of (x, y) coordinates
    std::vector<Coordinate> coordinates;
    // technique is the method used for executing the least square algorithm
    Technique technique;
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
    LeastSquare(Technique technique);
    LeastSquare(Technique technique, std::vector<Coordinate> coordinates);
    ~LeastSquare();

    void SetCoordinates(std::vector<Coordinate> coordinates);
    void InitModel();
    Coordinate PredictX(double Y);
    Coordinate PredictY(double X);
    std::string Formula();
  };
}
