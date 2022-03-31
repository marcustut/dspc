#include "serial.h"

#include <numeric>
#include <algorithm>
#include <cmath>
#include <fmt/core.h>

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

    std::tuple<double, double> CalculateGradient(const std::vector<MultivariateCoordinate> &mc)
    {
        // num of coordinate
        int n = mc.size();
        std::vector<double> result;

        double sum_of_x1 = std::accumulate(mc.begin(), mc.end(), 0.0, [](double pv, MultivariateCoordinate c)
                                           { return pv + c.xs[0]; });
        double sum_of_x2 = std::accumulate(mc.begin(), mc.end(), 0.0, [](double pv, MultivariateCoordinate c)
                                           { return pv + c.xs[1]; });
        double sum_of_y = std::accumulate(mc.begin(), mc.end(), 0.0, [](double pv, MultivariateCoordinate c)
                                          { return pv + c.y; });

        double mean_x1 = sum_of_x1 / n;
        double mean_x2 = sum_of_x2 / n;
        double mean_y = sum_of_y / n;

        double sum_of_squares_x1 = std::accumulate(mc.begin(), mc.end(), 0.0, [&](double pv, MultivariateCoordinate c)
                                                   { return pv + std::pow(c.xs[0] - mean_x1, 2); });
        double sum_of_squares_x2 = std::accumulate(mc.begin(), mc.end(), 0.0, [&](double pv, MultivariateCoordinate c)
                                                   { return pv + std::pow(c.xs[1] - mean_x2, 2); });

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

        return std::make_tuple(b1, b2);
    }
    double CalculateYIntercept(const std::vector<MultivariateCoordinate> &mc, const std::tuple<double, double> gradient)
    {
        return 0.0;
    }
}
