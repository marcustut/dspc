#include "least_square.h"

#include <string>
#include <numeric>
#include <cstdio>

namespace DSPC::LinearRegression
{
  LeastSquare::LeastSquare(
      std::vector<Coordinate> coordinates)
  {
    this->coordinates = coordinates;
    this->CalculateGradient();
    this->CalculateYIntercept();
  }

  LeastSquare::~LeastSquare()
  {
  }

  void LeastSquare::CalculateGradient()
  {
    if (coordinates.empty())
      throw std::logic_error(std::string("coordinates are empty"));

    // num of coordinate
    int n = coordinates.size();

    // sum of x*y
    std::vector<double> vec_xy;
    // map through each element of coordinates and calculate the sum
    std::transform(coordinates.begin(), coordinates.end(), std::back_inserter(vec_xy), [](Coordinate c)
                   { return c.x * c.y; });
    double sum_xy = std::accumulate(vec_xy.begin(), vec_xy.end(), 0.0);

    // sum of x
    std::vector<double> vec_x;
    std::transform(coordinates.begin(), coordinates.end(), std::back_inserter(vec_x), [](Coordinate c)
                   { return c.x; });
    double sum_x = std::accumulate(vec_x.begin(), vec_x.end(), 0.0);

    // sum of y
    std::vector<double> vec_y;
    std::transform(coordinates.begin(), coordinates.end(), std::back_inserter(vec_y), [](Coordinate c)
                   { return c.y; });
    double sum_y = std::accumulate(vec_y.begin(), vec_y.end(), 0.0);

    // sum of x*x
    std::vector<double> vec_xx;
    std::transform(coordinates.begin(), coordinates.end(), std::back_inserter(vec_xx), [](Coordinate c)
                   { return c.x * c.x; });
    double sum_xx = std::accumulate(vec_xx.begin(), vec_xx.end(), 0.0);

    this->m = (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
  }

  void LeastSquare::CalculateYIntercept()
  {
    // num of coordinates
    int n = coordinates.size();

    // sum of y
    std::vector<double> vec_y;
    std::transform(coordinates.begin(), coordinates.end(), std::back_inserter(vec_y), [](Coordinate c)
                   { return c.y; });
    double sum_y = std::accumulate(vec_y.begin(), vec_y.end(), 0.0);

    // sum of x
    std::vector<double> vec_x;
    std::transform(coordinates.begin(), coordinates.end(), std::back_inserter(vec_x), [](Coordinate c)
                   { return c.x; });
    double sum_x = std::accumulate(vec_x.begin(), vec_x.end(), 0.0);

    this->c = (1 / (double)n) * (sum_y - this->m * sum_x);
  }

  Coordinate LeastSquare::PredictX(double Y)
  {
    // x = (y - c) / m
    return (Coordinate){(Y - this->c) / this->m, Y};
  }

  Coordinate LeastSquare::PredictY(double X)
  {
    // y = mx + c
    return (Coordinate){X, (this->m * X) + this->c};
  }

  std::string LeastSquare::Formula()
  {
    return "y = " + std::to_string(this->m) + "x + " + std::to_string(this->c);
  }
} // namespace DSPC
