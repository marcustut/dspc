#include "pthread.h"

#include <functional>
#include <iostream>
#include <numeric>
#include <pthread.h>
#include <math.h>

#define MAX_THREAD 4

enum Operation
{
  SUM_OF_X1,
  SUM_OF_X2,
  SUM_OF_Y,
  SUM_OF_SQUARES_X1,
  SUM_OF_SQUARES_X2,
  SUM_OF_PRODUCTS_X1_Y,
  SUM_OF_PRODUCTS_X2_Y,
  SUM_OF_PRODUCTS_X1_X2,
};

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

double sum_of_x1 = 0.0;
double sum_of_x2 = 0.0;
double sum_of_y = 0.0;
double mean_x1 = 0.0;
double mean_x2 = 0.0;
double mean_y = 0.0;
double sum_of_squares_x1 = 0.0;
double sum_of_squares_x2 = 0.0;
double sum_of_products_x1_y = 0.0;
double sum_of_products_x2_y = 0.0;
double sum_of_products_x1_x2 = 0.0;

struct calculate_wrapper_args
{
  const std::vector<DSPC::Coordinate> &coordinates;
  std::function<double(const std::vector<DSPC::Coordinate> &)> callback;
};

void *calculate_wrapper(void *args)
{
  auto c = ((calculate_wrapper_args *)args)->coordinates;
  auto callback = ((calculate_wrapper_args *)args)->callback;
  double result = callback(c);
  return static_cast<void *>(&result);
}

struct calculate_multivariate_wrapper_args
{
  const std::vector<DSPC::MultivariateCoordinate> &coordinates;
  std::function<double(const std::vector<DSPC::MultivariateCoordinate> &)> callback;
  Operation op;
};

void *calculate_multivariate_wrapper(void *args)
{
  auto mc = ((calculate_multivariate_wrapper_args *)args)->coordinates;
  auto callback = ((calculate_multivariate_wrapper_args *)args)->callback;
  auto operation = ((calculate_multivariate_wrapper_args *)args)->op;
  double result = callback(mc);
  // Lock the mutex before updating sum
  pthread_mutex_lock(&lock);
  switch (operation)
  {
  case SUM_OF_X1:
    sum_of_x1 = result;
    break;
  case SUM_OF_X2:
    sum_of_x2 = result;
    break;
  case SUM_OF_Y:
    sum_of_y = result;
    break;
  case SUM_OF_SQUARES_X1:
    sum_of_squares_x1 = result;
    break;
  case SUM_OF_SQUARES_X2:
    sum_of_squares_x2 = result;
    break;
  case SUM_OF_PRODUCTS_X1_Y:
    sum_of_products_x1_y = result;
    break;
  case SUM_OF_PRODUCTS_X2_Y:
    sum_of_products_x2_y = result;
    break;
  case SUM_OF_PRODUCTS_X1_X2:
    sum_of_products_x1_x2 = result;
    break;
  }
  pthread_mutex_unlock(&lock);
  pthread_exit(NULL);
}

namespace DSPC::LinearRegression::Pthread
{
  struct sum_x_args
  {
    unsigned long index;
    std::vector<Coordinate> coordinates;
  };

  void *sum_x(void *args)
  {
    unsigned long index = ((sum_x_args *)args)->index;
    std::vector<Coordinate> coordinates = ((sum_x_args *)args)->coordinates;
    double sum = 0;

    for (int j = 0; j < coordinates.size() / MAX_THREAD; j++)
      sum += coordinates[index + j].x;

    return static_cast<void *>(&sum);
  }

  double CalculateGradient(const std::vector<Coordinate> &coordinates)
  {
    pthread_t threads[MAX_THREAD];

    for (int i = 0; i < MAX_THREAD; i++)
    {
      // int *n = new int(i * (coordinates.size() / MAX_THREAD));
      sum_x_args *args = new sum_x_args{i * (coordinates.size() / MAX_THREAD), coordinates};
      if (pthread_create(&threads[i], nullptr, sum_x, args) != 0)
        perror("failed to create thread");
      delete args;
    }

    double globalSum = 0;
    for (int i = 0; i < MAX_THREAD; i++)
    {
      double *r = new double();
      if (pthread_join(threads[i], (void **)&r) != 0)
        perror("failed to join thread");
      globalSum += *r;
      delete r;
    }

    std::cout << "sum_x: " << globalSum << '\n';

    // sum of x*y

    // sum of x

    // sum of y

    // sum of x*x

    return 0.0;
  }

  double CalculateYIntercept(const std::vector<Coordinate> &coordinates, const double gradient)
  {
    return 0.0;
  }

  std::tuple<std::pair<double, double>, double> CalculateGradientAndYIntercept(const std::vector<MultivariateCoordinate> &mc)
  {
    int n = mc.size();
    pthread_t threads[8];

    auto calculate_sum_of_x1_callback = [](const std::vector<MultivariateCoordinate> &mc)
    { return std::accumulate(mc.begin(), mc.end(), 0.0, [](double pv, MultivariateCoordinate c)
                             { return pv + c.xs[0]; }); };
    auto calculate_sum_of_x2_callback = [](const std::vector<MultivariateCoordinate> &mc)
    { return std::accumulate(mc.begin(), mc.end(), 0.0, [](double pv, MultivariateCoordinate c)
                             { return pv + c.xs[1]; }); };
    auto calculate_sum_of_y_callback = [](const std::vector<MultivariateCoordinate> &mc)
    { return std::accumulate(mc.begin(), mc.end(), 0.0, [](double pv, MultivariateCoordinate c)
                             { return pv + c.y; }); };

    calculate_multivariate_wrapper_args *args1 = new calculate_multivariate_wrapper_args{
        mc, calculate_sum_of_x1_callback, SUM_OF_X1};
    calculate_multivariate_wrapper_args *args2 = new calculate_multivariate_wrapper_args{
        mc, calculate_sum_of_x2_callback, SUM_OF_X2};
    calculate_multivariate_wrapper_args *args3 = new calculate_multivariate_wrapper_args{
        mc, calculate_sum_of_y_callback, SUM_OF_Y};

    if (pthread_create(&threads[0], nullptr, calculate_multivariate_wrapper, args1) != 0)
      perror("failed to create thread 1");
    if (pthread_create(&threads[1], nullptr, calculate_multivariate_wrapper, args2) != 0)
      perror("failed to create thread 2");
    if (pthread_create(&threads[2], nullptr, calculate_multivariate_wrapper, args3) != 0)
      perror("failed to create thread 3");

    void *retval;
    if (pthread_join(threads[0], &retval) != 0)
      perror("failed to join thread");
    if (pthread_join(threads[1], &retval) != 0)
      perror("failed to join thread");
    if (pthread_join(threads[2], &retval) != 0)
      perror("failed to join thread");

    double mean_x1 = sum_of_x1 / n;
    double mean_x2 = sum_of_x2 / n;
    double mean_y = sum_of_y / n;

    auto calculate_sum_of_squares_x1_callback = [&](const std::vector<MultivariateCoordinate> &mc)
    { return std::accumulate(mc.begin(), mc.end(), 0.0, [&](double pv, MultivariateCoordinate c)
                             { return pv + std::pow(c.xs[0] - mean_x1, 2); }); };
    auto calculate_sum_of_squares_x2_callback = [&](const std::vector<MultivariateCoordinate> &mc)
    { return std::accumulate(mc.begin(), mc.end(), 0.0, [&](double pv, MultivariateCoordinate c)
                             { return pv + std::pow(c.xs[1] - mean_x2, 2); }); };
    auto calculate_sum_of_products_x1_y = [&](const std::vector<MultivariateCoordinate> &mc)
    { return std::accumulate(mc.begin(), mc.end(), 0.0, [&](double pv, MultivariateCoordinate c)
                             { return pv + (c.xs[0] - mean_x1) * (c.y - mean_y); }); };
    auto calculate_sum_of_products_x2_y = [&](const std::vector<MultivariateCoordinate> &mc)
    { return std::accumulate(mc.begin(), mc.end(), 0.0, [&](double pv, MultivariateCoordinate c)
                             { return pv + (c.xs[1] - mean_x2) * (c.y - mean_y); }); };
    auto calculate_sum_of_products_x1_x2 = [&](const std::vector<MultivariateCoordinate> &mc)
    { return std::accumulate(mc.begin(), mc.end(), 0.0, [&](double pv, MultivariateCoordinate c)
                             { return pv + (c.xs[0] - mean_x1) * (c.xs[1] - mean_x2); }); };

    calculate_multivariate_wrapper_args *args4 = new calculate_multivariate_wrapper_args{
        mc, calculate_sum_of_squares_x1_callback, SUM_OF_SQUARES_X1};
    calculate_multivariate_wrapper_args *args5 = new calculate_multivariate_wrapper_args{
        mc, calculate_sum_of_squares_x2_callback, SUM_OF_SQUARES_X2};
    calculate_multivariate_wrapper_args *args6 = new calculate_multivariate_wrapper_args{
        mc, calculate_sum_of_products_x1_y, SUM_OF_PRODUCTS_X1_Y};
    calculate_multivariate_wrapper_args *args7 = new calculate_multivariate_wrapper_args{
        mc, calculate_sum_of_products_x2_y, SUM_OF_PRODUCTS_X2_Y};
    calculate_multivariate_wrapper_args *args8 = new calculate_multivariate_wrapper_args{
        mc, calculate_sum_of_products_x1_x2, SUM_OF_PRODUCTS_X1_X2};

    if (pthread_create(&threads[3], nullptr, calculate_multivariate_wrapper, args4) != 0)
      perror("failed to create thread 3");
    if (pthread_create(&threads[4], nullptr, calculate_multivariate_wrapper, args5) != 0)
      perror("failed to create thread 4");
    if (pthread_create(&threads[5], nullptr, calculate_multivariate_wrapper, args6) != 0)
      perror("failed to create thread 5");
    if (pthread_create(&threads[6], nullptr, calculate_multivariate_wrapper, args7) != 0)
      perror("failed to create thread 6");
    if (pthread_create(&threads[7], nullptr, calculate_multivariate_wrapper, args8) != 0)
      perror("failed to create thread 7");

    if (pthread_join(threads[3], &retval) != 0)
      perror("failed to join thread");
    if (pthread_join(threads[4], &retval) != 0)
      perror("failed to join thread");
    if (pthread_join(threads[5], &retval) != 0)
      perror("failed to join thread");
    if (pthread_join(threads[6], &retval) != 0)
      perror("failed to join thread");
    if (pthread_join(threads[7], &retval) != 0)
      perror("failed to join thread");

    double b1 = (sum_of_products_x1_y * sum_of_squares_x2 - sum_of_products_x1_x2 * sum_of_products_x2_y) / (sum_of_squares_x1 * sum_of_squares_x2 - sum_of_products_x1_x2 * sum_of_products_x1_x2);
    double b2 = (sum_of_products_x2_y * sum_of_squares_x1 - sum_of_products_x1_x2 * sum_of_products_x1_y) / (sum_of_squares_x1 * sum_of_squares_x2 - sum_of_products_x1_x2 * sum_of_products_x1_x2);
    double a = mean_y - (b1 * mean_x1) - (b2 * mean_x2);

    return std::make_tuple(std::make_pair(b1, b2), a);
  }
}
