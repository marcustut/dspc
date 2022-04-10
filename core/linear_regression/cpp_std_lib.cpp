#include "pthread.h"

#include <thread>
#include <future>
#include <numeric>
#include <algorithm>
#include <math.h>
#include "cpp_std_lib.h"

void calculate_wrapper(std::promise<double> &&p, const std::vector<DSPC::Coordinate> &c, std::function<double(const std::vector<DSPC::Coordinate> &)> callback)
{
  p.set_value(callback(c));
}

void calculate_multivariate_wrapper(std::promise<double> &&p, const std::vector<DSPC::MultivariateCoordinate> &mc, std::function<double(const std::vector<DSPC::MultivariateCoordinate> &)> callback)
{
  p.set_value(callback(mc));
}

namespace DSPC::LinearRegression::CppStdLib
{
  double CalculateGradient(const std::vector<Coordinate> &coordinates)
  {
    // num of coordinate
    int n = coordinates.size();

    std::promise<double> promise_sum_of_xy;
    std::promise<double> promise_sum_of_x;
    std::promise<double> promise_sum_of_y;
    std::promise<double> promise_sum_of_xx;
    auto future_sum_of_xy = promise_sum_of_xy.get_future();
    auto future_sum_of_x = promise_sum_of_x.get_future();
    auto future_sum_of_y = promise_sum_of_y.get_future();
    auto future_sum_of_xx = promise_sum_of_xx.get_future();

    auto calculate_sum_of_xy_callback = [](const std::vector<Coordinate> &coord)
    { return std::accumulate(coord.begin(), coord.end(), 0.0, [](double pv, Coordinate c)
                             { return pv + c.x * c.y; }); };
    auto calculate_sum_of_x_callback = [](const std::vector<Coordinate> &coord)
    { return std::accumulate(coord.begin(), coord.end(), 0.0, [](double pv, Coordinate c)
                             { return pv + c.x; }); };
    auto calculate_sum_of_y_callback = [](const std::vector<Coordinate> &coord)
    { return std::accumulate(coord.begin(), coord.end(), 0.0, [](double pv, Coordinate c)
                             { return pv + c.y; }); };
    auto calculate_sum_of_xx_callback = [](const std::vector<Coordinate> &coord)
    { return std::accumulate(coord.begin(), coord.end(), 0.0, [](double pv, Coordinate c)
                             { return pv + c.x * c.x; }); };

    std::thread t1(&calculate_wrapper, std::move(promise_sum_of_xy), coordinates, calculate_sum_of_xy_callback);
    std::thread t2(&calculate_wrapper, std::move(promise_sum_of_x), coordinates, calculate_sum_of_x_callback);
    std::thread t3(&calculate_wrapper, std::move(promise_sum_of_y), coordinates, calculate_sum_of_y_callback);
    std::thread t4(&calculate_wrapper, std::move(promise_sum_of_xx), coordinates, calculate_sum_of_xx_callback);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    double sum_of_xy = future_sum_of_xy.get();
    double sum_of_x = future_sum_of_x.get();
    double sum_of_y = future_sum_of_y.get();
    double sum_of_xx = future_sum_of_xx.get();

    return (n * sum_of_xy - sum_of_x * sum_of_y) / (n * sum_of_xx - sum_of_x * sum_of_x);
  }
  double CalculateYIntercept(const std::vector<Coordinate> &coordinates, const double gradient)
  {
    // num of coordinates
    int n = coordinates.size();

    std::promise<double> promise_sum_of_y;
    std::promise<double> promise_sum_of_x;
    auto future_sum_of_y = promise_sum_of_y.get_future();
    auto future_sum_of_x = promise_sum_of_x.get_future();

    auto calculate_sum_of_y_callback = [](const std::vector<Coordinate> &c)
    { return std::accumulate(c.begin(), c.end(), 0.0, [](double pv, Coordinate c)
                             { return pv + c.y; }); };
    auto calculate_sum_of_x_callback = [](const std::vector<Coordinate> &c)
    { return std::accumulate(c.begin(), c.end(), 0.0, [](double pv, Coordinate c)
                             { return pv + c.x; }); };

    std::thread t1(&calculate_wrapper, std::move(promise_sum_of_y), coordinates, calculate_sum_of_y_callback);
    std::thread t2(&calculate_wrapper, std::move(promise_sum_of_x), coordinates, calculate_sum_of_x_callback);

    t1.join();
    t2.join();

    double sum_of_y = future_sum_of_y.get();
    double sum_of_x = future_sum_of_x.get();

    return (1 / (double)n) * (sum_of_y - gradient * sum_of_x);
  }

  std::tuple<std::pair<double, double>, double> CalculateGradientAndYIntercept(const std::vector<MultivariateCoordinate> &mc)
  {
    // num of coordinate
    int n = mc.size();

    std::promise<double> promise_sum_of_x1;
    std::promise<double> promise_sum_of_x2;
    std::promise<double> promise_sum_of_y;
    auto future_sum_of_x1 = promise_sum_of_x1.get_future();
    auto future_sum_of_x2 = promise_sum_of_x2.get_future();
    auto future_sum_of_y = promise_sum_of_y.get_future();

    auto calculate_sum_of_x1_callback = [](const std::vector<MultivariateCoordinate> &mc)
    { return std::accumulate(mc.begin(), mc.end(), 0.0, [](double pv, MultivariateCoordinate c)
                             { return pv + c.xs[0]; }); };
    auto calculate_sum_of_x2_callback = [](const std::vector<MultivariateCoordinate> &mc)
    { return std::accumulate(mc.begin(), mc.end(), 0.0, [](double pv, MultivariateCoordinate c)
                             { return pv + c.xs[1]; }); };
    auto calculate_sum_of_y_callback = [](const std::vector<MultivariateCoordinate> &mc)
    { return std::accumulate(mc.begin(), mc.end(), 0.0, [](double pv, MultivariateCoordinate c)
                             { return pv + c.y; }); };

    std::thread t1(&calculate_multivariate_wrapper, std::move(promise_sum_of_x1), mc, calculate_sum_of_x1_callback);
    std::thread t2(&calculate_multivariate_wrapper, std::move(promise_sum_of_x2), mc, calculate_sum_of_x2_callback);
    std::thread t3(&calculate_multivariate_wrapper, std::move(promise_sum_of_y), mc, calculate_sum_of_y_callback);

    t1.join();
    t2.join();
    t3.join();

    double sum_of_x1 = future_sum_of_x1.get();
    double sum_of_x2 = future_sum_of_x2.get();
    double sum_of_y = future_sum_of_y.get();

    double mean_x1 = sum_of_x1 / n;
    double mean_x2 = sum_of_x2 / n;
    double mean_y = sum_of_y / n;

    std::promise<double> promise_sum_of_squares_x1;
    std::promise<double> promise_sum_of_squares_x2;
    std::promise<double> promise_sum_of_products_x1_y;
    std::promise<double> promise_sum_of_products_x2_y;
    std::promise<double> promise_sum_of_products_x1_x2;
    auto future_sum_of_squares_x1 = promise_sum_of_squares_x1.get_future();
    auto future_sum_of_squares_x2 = promise_sum_of_squares_x2.get_future();
    auto future_sum_of_products_x1_y = promise_sum_of_products_x1_y.get_future();
    auto future_sum_of_products_x2_y = promise_sum_of_products_x2_y.get_future();
    auto future_sum_of_products_x1_x2 = promise_sum_of_products_x1_x2.get_future();

    auto calculate_sum_of_squares_x1_callback = [&](const std::vector<MultivariateCoordinate> &mc)
    { return std::accumulate(mc.begin(), mc.end(), 0.0, [&](double pv, MultivariateCoordinate c)
                             { return pv + std::pow(c.xs[0] - mean_x1, 2); }); };
    auto calculate_sum_of_squares_x2_callback = [&](const std::vector<MultivariateCoordinate> &mc)
    { return std::accumulate(mc.begin(), mc.end(), 0.0, [&](double pv, MultivariateCoordinate c)
                             { return pv + std::pow(c.xs[1] - mean_x2, 2); }); };
    auto calculate_sum_of_products_x1_y = [&](const std::vector<MultivariateCoordinate> &mc)
    {
      std::vector<double> result;
      std::transform(mc.begin(), mc.end(), mc.begin(), std::back_inserter(result), [&](MultivariateCoordinate a, MultivariateCoordinate b)
                     { return (a.xs[0] - mean_x1) * (b.y - mean_y); });
      return std::accumulate(result.begin(), result.end(), 0.0, [&](double pv, double val)
                             { return pv + val; });
    };
    auto calculate_sum_of_products_x2_y = [&](const std::vector<MultivariateCoordinate> &mc)
    {
      std::vector<double> result;
      std::transform(mc.begin(), mc.end(), mc.begin(), std::back_inserter(result), [&](MultivariateCoordinate a, MultivariateCoordinate b)
                     { return (a.xs[1] - mean_x2) * (b.y - mean_y); });
      return std::accumulate(result.begin(), result.end(), 0.0, [&](double pv, double val)
                             { return pv + val; });
    };
    auto calculate_sum_of_products_x1_x2 = [&](const std::vector<MultivariateCoordinate> &mc)
    {
      std::vector<double> result;
      std::transform(mc.begin(), mc.end(), mc.begin(), std::back_inserter(result), [&](MultivariateCoordinate a, MultivariateCoordinate b)
                     { return (a.xs[0] - mean_x1) * (b.xs[1] - mean_x2); });
      return std::accumulate(result.begin(), result.end(), 0.0, [&](double pv, double val)
                             { return pv + val; });
    };

    std::thread t4(&calculate_multivariate_wrapper, std::move(promise_sum_of_squares_x1), mc, calculate_sum_of_squares_x1_callback);
    std::thread t5(&calculate_multivariate_wrapper, std::move(promise_sum_of_squares_x2), mc, calculate_sum_of_squares_x2_callback);
    std::thread t6(&calculate_multivariate_wrapper, std::move(promise_sum_of_products_x1_y), mc, calculate_sum_of_products_x1_y);
    std::thread t7(&calculate_multivariate_wrapper, std::move(promise_sum_of_products_x2_y), mc, calculate_sum_of_products_x2_y);
    std::thread t8(&calculate_multivariate_wrapper, std::move(promise_sum_of_products_x1_x2), mc, calculate_sum_of_products_x1_x2);

    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();

    double sum_of_squares_x1 = future_sum_of_squares_x1.get();
    double sum_of_squares_x2 = future_sum_of_squares_x2.get();
    double sum_of_products_x1_y = future_sum_of_products_x1_y.get();
    double sum_of_products_x2_y = future_sum_of_products_x2_y.get();
    double sum_of_products_x1_x2 = future_sum_of_products_x1_x2.get();

    double b1 = (sum_of_products_x1_y * sum_of_squares_x2 - sum_of_products_x1_x2 * sum_of_products_x2_y) / (sum_of_squares_x1 * sum_of_squares_x2 - sum_of_products_x1_x2 * sum_of_products_x1_x2);
    double b2 = (sum_of_products_x2_y * sum_of_squares_x1 - sum_of_products_x1_x2 * sum_of_products_x1_y) / (sum_of_squares_x1 * sum_of_squares_x2 - sum_of_products_x1_x2 * sum_of_products_x1_x2);
    double a = mean_y - (b1 * mean_x1) - (b2 * mean_x2);

    return std::make_tuple(std::make_pair(b1, b2), a);
  }
}
