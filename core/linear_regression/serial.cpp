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

    double CalculateGradient(const std::vector<MultivariateCoordinate> &multivariate_coordinates)
    {
        // num of coordinate
        int n = multivariate_coordinates.size();

        double sum_of_x1 = std::accumulate(multivariate_coordinates.begin(), multivariate_coordinates.end(), 0.0, [](double pv, MultivariateCoordinate c)
                                           { return pv + c.xs[0]; });
        double sum_of_x2 = std::accumulate(multivariate_coordinates.begin(), multivariate_coordinates.end(), 0.0, [](double pv, MultivariateCoordinate c)
                                           { return pv + c.xs[1]; });
        double sum_of_y = std::accumulate(multivariate_coordinates.begin(), multivariate_coordinates.end(), 0.0, [](double pv, MultivariateCoordinate c)
                                          { return pv + c.y; });

        double mean_x1 = sum_of_x1 / n;
        double mean_x2 = sum_of_x2 / n;
        double mean_y = sum_of_y / n;

        double sum_of_squares_x1 = std::accumulate(multivariate_coordinates.begin(), multivariate_coordinates.end(), 0.0, [&](double pv, MultivariateCoordinate c)
                                                   { return pv + std::pow(c.xs[0] - mean_x1, 2); });
        double sum_of_squares_x2 = std::accumulate(multivariate_coordinates.begin(), multivariate_coordinates.end(), 0.0, [&](double pv, MultivariateCoordinate c)
                                                   { return pv + std::pow(c.xs[1] - mean_x2, 2); });

        fmt::print("Sum of X1: {}\n", sum_of_x1);
        fmt::print("Sum of X2: {}\n", sum_of_x2);
        fmt::print("Sum of Y: {}\n", sum_of_y);
        fmt::print("Mean X1: {}\n", mean_x1);
        fmt::print("Mean X2: {}\n", mean_x2);
        fmt::print("Mean Y: {}\n", mean_y);
        fmt::print("Sum of Squares X1: {}\n", sum_of_squares_x1);
        fmt::print("Sum of Squares X2: {}\n", sum_of_squares_x2);

        return 0.0;
    }
    double CalculateYIntercept(const std::vector<MultivariateCoordinate> &multivariate_coordinates, const double gradient)
    {
        // num of coordinates
        int n = multivariate_coordinates.size();

        // sum of y
        std::vector<double> vec_y;
        std::transform(
            multivariate_coordinates.begin(),
            multivariate_coordinates.end(),
            std::back_inserter(vec_y),
            [](MultivariateCoordinate c)
            { return c.y; });
        double sum_y = std::accumulate(vec_y.begin(), vec_y.end(), 0.0);

        // sum of x
        std::vector<double> vec_x;
        std::transform(
            multivariate_coordinates.begin(),
            multivariate_coordinates.end(),
            std::back_inserter(vec_x),
            [](MultivariateCoordinate c)
            { return c.y; });
        double sum_x = std::accumulate(vec_x.begin(), vec_x.end(), 0.0);

        return (1 / (double)n) * (sum_y - gradient * sum_x);
    }
}
