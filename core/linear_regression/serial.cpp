#include "serial.h"

#include <numeric>
#include <algorithm>

namespace DSPC::LinearRegression::Serial
{
  double CalculateGradient(const std::vector<Coordinate> &coordinates)
  {
    // num of coordinate
    int n = coordinates.size();

    // sum of x*y
    std::vector<double> vec_xy;
    std::transform(
        coordinates.begin(),
        coordinates.end(),
        std::back_inserter(vec_xy),
        [](Coordinate c)
        { return c.x * c.y; });
    double sum_xy = std::accumulate(vec_xy.begin(), vec_xy.end(), 0.0);

    // sum of x
    std::vector<double> vec_x;
    std::transform(
        coordinates.begin(),
        coordinates.end(),
        std::back_inserter(vec_x),
        [](Coordinate c)
        { return c.x; });
    double sum_x = std::accumulate(vec_x.begin(), vec_x.end(), 0.0);

    // sum of y
    std::vector<double> vec_y;
    std::transform(
        coordinates.begin(),
        coordinates.end(),
        std::back_inserter(vec_y),
        [](Coordinate c)
        { return c.y; });
    double sum_y = std::accumulate(vec_y.begin(), vec_y.end(), 0.0);

    // sum of x*x
    std::vector<double> vec_xx;
    std::transform(
        coordinates.begin(),
        coordinates.end(),
        std::back_inserter(vec_xx),
        [](Coordinate c)
        { return c.x * c.x; });
    double sum_xx = std::accumulate(vec_xx.begin(), vec_xx.end(), 0.0);

    return (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
  }

  double CalculateYIntercept(const std::vector<Coordinate> &coordinates, const double gradient)
  {
    // num of coordinates
    int n = coordinates.size();

    // sum of y
    std::vector<double> vec_y;
    std::transform(
        coordinates.begin(),
        coordinates.end(),
        std::back_inserter(vec_y),
        [](Coordinate c)
        { return c.y; });
    double sum_y = std::accumulate(vec_y.begin(), vec_y.end(), 0.0);

    // sum of x
    std::vector<double> vec_x;
    std::transform(
        coordinates.begin(),
        coordinates.end(),
        std::back_inserter(vec_x),
        [](Coordinate c)
        { return c.x; });
    double sum_x = std::accumulate(vec_x.begin(), vec_x.end(), 0.0);

    return (1 / (double)n) * (sum_y - gradient * sum_x);
  }
}
