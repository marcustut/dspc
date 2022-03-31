#include "pthread.h"

#include <iostream>
#include <pthread.h>

// maximum number of threads
#define MAX_THREAD 4

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

  std::tuple<double, double> CalculateGradient(const std::vector<MultivariateCoordinate> &multivariate_coordinates)
  {
    return std::make_pair(0.0, 0.0);
  }

  double CalculateYIntercept(const std::vector<MultivariateCoordinate> &multivariate_coordinates, const std::tuple<double, double> gradient)
  {
    return 0.0;
  }
}
