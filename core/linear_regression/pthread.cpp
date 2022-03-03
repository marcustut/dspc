#include "pthread.h"

#include <pthread.h>

// maximum number of threads
#define MAX_THREAD 4

namespace DSPC::LinearRegression::Pthread
{
  

  double CalculateGradient(const std::vector<Coordinate>& coordinates)
  {
    pthread_t threads[MAX_THREAD];

    for (int i = 0; i < MAX_THREAD; i++)
      pthread_create(&threads[i], nullptr, nullptr, nullptr);

    // num of coordinate
    int n = coordinates.size();

    // sum of x*y

    // sum of x

    // sum of y

    // sum of x*x

    return 0.0;
  }

  double CalculateYIntercept(const std::vector<Coordinate>& coordinates, const double gradient)
  {
    return 0.0;
  }
}
