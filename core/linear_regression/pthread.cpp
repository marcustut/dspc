#include "pthread.h"

#include <functional>
#include <iostream>
#include <functional>
#include <pthread.h>

// maximum number of threads
#define MAX_THREAD 4

struct calculate_wrapper_args
{
  std::vector<DSPC::Coordinate> &coordinates;
  std::function<double(std::vector<DSPC::Coordinate> &)> callback;
};

// void *calculate_wrapper(void *args)
// {
//   auto c = ((calculate_wrapper_args *)args)->coordinates;
//   auto callback = ((calculate_wrapper_args *)args)->callback;
//   return static_cast<void *>(&callback(c));
// }

struct calculate_multivariate_wrapper_args
{
  std::vector<DSPC::MultivariateCoordinate> &coordinates;
  std::function<double(std::vector<DSPC::MultivariateCoordinate> &)> callback;
};

// void *calculate_multivariate_wrapper(void *args)
// {
//   auto mc = ((calculate_multivariate_wrapper_args *)args)->coordinates;
//   auto callback = ((calculate_multivariate_wrapper_args *)args)->callback;
//   return static_cast<void *>(&callback(mc));
// }

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
    // pthread_t threads[8];

    // double sum_of_x1, sum_of_x2, sum_of_y;

    // auto calculate_sum_of_x1_callback = [](const std::vector<MultivariateCoordinate> &mc)
    // { return std::accumulate(mc.begin(), mc.end(), 0.0, [](double pv, MultivariateCoordinate c)
    //                          { return pv + c.xs[0]; }); };
    // auto calculate_sum_of_x2_callback = [](const std::vector<MultivariateCoordinate> &mc)
    // { return std::accumulate(mc.begin(), mc.end(), 0.0, [](double pv, MultivariateCoordinate c)
    //                          { return pv + c.xs[1]; }); };
    // auto calculate_sum_of_y_callback = [](const std::vector<MultivariateCoordinate> &mc)
    // { return std::accumulate(mc.begin(), mc.end(), 0.0, [](double pv, MultivariateCoordinate c)
    //                          { return pv + c.y; }); };

    // calculate_multivariate_wrapper_args *args1 = new calculate_multivariate_wrapper_args{
    //     mc, calculate_sum_of_x1_callback};
    // calculate_multivariate_wrapper_args *args2 = new calculate_multivariate_wrapper_args{
    //     mc, calculate_sum_of_x2_callback};
    // calculate_multivariate_wrapper_args *args3 = new calculate_multivariate_wrapper_args{
    //     mc, calculate_sum_of_y_callback};

    // if (pthread_create(&threads[0], nullptr, calculate_multivariate_wrapper, args1) != 0)
    //   perror("failed to create thread 1");
    // delete args1;

    // if (pthread_create(&threads[1], nullptr, calculate_multivariate_wrapper, args2) != 0)
    //   perror("failed to create thread 2");
    // delete args2;

    // if (pthread_create(&threads[2], nullptr, calculate_multivariate_wrapper, args3) != 0)
    //   perror("failed to create thread 3");
    // delete args3;

    // double *r = new double();

    // if (pthread_join(threads[0], (void **)&r) != 0)
    //   perror("failed to join thread");
    // sum_of_x1 = *r;

    // if (pthread_join(threads[0], (void **)&r) != 0)
    //   perror("failed to join thread");
    // sum_of_x2 = *r;

    // if (pthread_join(threads[0], (void **)&r) != 0)
    //   perror("failed to join thread");
    // sum_of_y = *r;

    // delete r;

    return std::make_tuple(std::make_pair(0.0, 0.0), 0.0);
  }
}
