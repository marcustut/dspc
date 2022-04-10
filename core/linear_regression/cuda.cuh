#pragma once

#include <vector>

#include "core/coordinate.h"

namespace DSPC::LinearRegression::CUDA
{
    double CalculateGradient(const std::vector<Coordinate> &coordinates);
    double CalculateYIntercept(const std::vector<Coordinate> &coordinates, const double gradient);
    std::tuple<std::pair<double, double>, double> CalculateGradientAndYIntercept(const std::vector<MultivariateCoordinate> &mc);
    void EndCuda();
}