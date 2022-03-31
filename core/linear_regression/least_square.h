#pragma once

#include <vector>
#include <string>
#include <variant>

#include "core/coordinate.h"
#include "core/technique.h"
#include "core/type.h"

namespace DSPC::LinearRegression
{
  class LeastSquare
  {
  private:
    // coordinates is the set of (x, y) coordinates
    std::vector<Coordinate> coordinates;
    // multivariate_coordinates is the set of (x, y) coordinates
    std::vector<MultivariateCoordinate> multivariate_coordinates;
    // technique is the method used for executing the least square algorithm
    Technique technique;
    // type is the type of linear regression
    Type type;
    // m is the gradient, c is the y-intercept (for single linear regression)
    double m, c;
    // b1 is the gradient for x1, b2 is the gradient for x2, a is the y-intercept (for multivariate linear regression)
    double b1, b2, a;

    // CalculateGradient calculates gradient of straight line
    // based on given coordinates and update the 'm' variable.
    void CalculateGradient();

    // CalculateYIntercept calculates y-intercept of straight line
    // based on given coordinates and update the 'c' variable.
    // NOTE: This should be called only after 'm' is initialized else,
    //       it will throw and exception.
    void CalculateYIntercept();

  public:
    LeastSquare(Technique technique, Type type);
    LeastSquare(Technique technique, Type type, std::vector<Coordinate> coordinates);
    LeastSquare(Technique technique, Type type, std::vector<MultivariateCoordinate> multivariate_coordinates);
    ~LeastSquare();

    void SetCoordinates(std::vector<Coordinate> coordinates);
    void SetCoordinates(std::vector<MultivariateCoordinate> multivariate_coordinates);
    void InitModel();
    std::variant<Coordinate, MultivariateCoordinate> PredictX(double Y);
    Coordinate PredictY(double X);
    MultivariateCoordinate PredictY(std::vector<double> Xs);
    std::string Formula();
  };
}
