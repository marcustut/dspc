#include "open_mp.h"

#include <iostream>
#include <numeric>
#include <algorithm>
#include <functional>
#include <math.h>
#include <omp.h>

namespace DSPC::LinearRegression::OpenMP
{
  double CalculateGradient(const std::vector<Coordinate> &coordinates)
  {
    // num of coordinate
    int n = coordinates.size();

    double sum_xy = std::accumulate(coordinates.begin(), coordinates.end(), 0.0, [](double pv, Coordinate c)
                                    { return pv + c.x * c.y; });
    double sum_x = std::accumulate(coordinates.begin(), coordinates.end(), 0.0, [](double pv, Coordinate c)
                                   { return pv + c.x; });
    double sum_y = std::accumulate(coordinates.begin(), coordinates.end(), 0.0, [](double pv, Coordinate c)
                                   { return pv + c.y; });
    double sum_xx = std::accumulate(coordinates.begin(), coordinates.end(), 0.0, [](double pv, Coordinate c)
                                    { return pv + std::pow(c.x, 2); });

    return (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
    // return 0.0;
  }

  double CalculateYIntercept(const std::vector<Coordinate> &coordinates, const double gradient)
  {
    // num of coordinates
    int n = coordinates.size();

    double sum_y = std::accumulate(coordinates.begin(), coordinates.end(), 0.0, [](double pv, Coordinate c)
                                   { return pv + c.y; });
    double sum_x = std::accumulate(coordinates.begin(), coordinates.end(), 0.0, [](double pv, Coordinate c)
                                   { return pv + c.x; });

    return (1 / (double)n) * (sum_y - gradient * sum_x);
    // return 1.1;
  }

  std::tuple<std::pair<double, double>, double> CalculateGradientAndYIntercept(const std::vector<MultivariateCoordinate> &mc)
  {
    // num of coordinate
    int n = mc.size();
    std::vector<double> result;
    double sum_of_x1, sum_of_x2, sum_of_y, sum_of_squares_x1, sum_of_squares_x2;
    omp_set_nested(2);
#pragma omp parallel num_threads(16)
    {
      if (omp_get_thread_num() < 5)
      {
        //  Task 0
        sum_of_x1 = std::accumulate(mc.begin(), mc.end(), 0.0, [](double pv, MultivariateCoordinate c)
                                    { return pv + c.xs[0]; });
      }
      else if (omp_get_thread_num() >= 5 && omp_get_thread_num() < 11)
      {
        //  Task 1
        sum_of_x2 = std::accumulate(mc.begin(), mc.end(), 0.0, [](double pv, MultivariateCoordinate c)
                                    { return pv + c.xs[1]; });
      }
      else if (omp_get_thread_num() >= 11 && omp_get_thread_num() < 16)
      {
        //  Task 2
        sum_of_y = std::accumulate(mc.begin(), mc.end(), 0.0, [](double pv, MultivariateCoordinate c)
                                   { return pv + c.y; });
      }
    }
    // sum_of_x1 = std::accumulate(mc.begin(), mc.end(), 0.0, [](double pv, MultivariateCoordinate c)
    //                             { return pv + c.xs[0]; });
    // sum_of_x2 = std::accumulate(mc.begin(), mc.end(), 0.0, [](double pv, MultivariateCoordinate c)
    //                             { return pv + c.xs[1]; });
    // sum_of_y = std::accumulate(mc.begin(), mc.end(), 0.0, [](double pv, MultivariateCoordinate c)
    //                            { return pv + c.y; });
    double mean_x1 = sum_of_x1 / n;
    double mean_x2 = sum_of_x2 / n;
    double mean_y = sum_of_y / n;

#pragma omp parallel num_threads(16)
    {
      if (omp_get_thread_num() < 8)
      {
        //  Task 3
        sum_of_squares_x1 = std::accumulate(mc.begin(), mc.end(), 0.0, [&](double pv, MultivariateCoordinate c)
                                            { return pv + std::pow(c.xs[0] - mean_x1, 2); });
      }
      else
      {
        //  Task 4
        sum_of_squares_x2 = std::accumulate(mc.begin(), mc.end(), 0.0, [&](double pv, MultivariateCoordinate c)
                                            { return pv + std::pow(c.xs[1] - mean_x2, 2); });
      }
    }

    std::transform(mc.begin(), mc.end(), mc.begin(), std::back_inserter(result), [&](MultivariateCoordinate a, MultivariateCoordinate b)
                   { return (a.xs[0] - mean_x1) * (b.y - mean_y); });
    double sum_of_products_x1_y = std::accumulate(result.begin(), result.end(), 0.0, [&](double pv, double val)
                                                  { return pv + val; });
    result.clear();

    std::transform(mc.begin(), mc.end(), mc.begin(), std::back_inserter(result), [&](MultivariateCoordinate a, MultivariateCoordinate b)
                   { return (a.xs[1] - mean_x2) * (b.y - mean_y); });
    double sum_of_products_x2_y = std::accumulate(result.begin(), result.end(), 0.0, [&](double pv, double val)
                                                  { return pv + val; });
    result.clear();

    std::transform(mc.begin(), mc.end(), mc.begin(), std::back_inserter(result), [&](MultivariateCoordinate a, MultivariateCoordinate b)
                   { return (a.xs[0] - mean_x1) * (b.xs[1] - mean_x2); });
    double sum_of_products_x1_x2 = std::accumulate(result.begin(), result.end(), 0.0, [&](double pv, double val)
                                                   { return pv + val; });
    result.clear();

    double b1 = (sum_of_products_x1_y * sum_of_squares_x2 - sum_of_products_x1_x2 * sum_of_products_x2_y) / (sum_of_squares_x1 * sum_of_squares_x2 - sum_of_products_x1_x2 * sum_of_products_x1_x2);

    double b2 = (sum_of_products_x2_y * sum_of_squares_x1 - sum_of_products_x1_x2 * sum_of_products_x1_y) / (sum_of_squares_x1 * sum_of_squares_x2 - sum_of_products_x1_x2 * sum_of_products_x1_x2);

    double a = mean_y - (b1 * mean_x1) - (b2 * mean_x2);

    return std::make_tuple(std::make_pair(b1, b2), a);
    // return std::make_tuple(std::make_pair(0, 0), 0);
  }
}