#ifndef __NVCC__
#error Not using NVCC, perhaps not available
#endif // !__NVCC__

#ifndef __CUDACC__
#define __CUDACC__
#endif

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include <stdio.h>

const unsigned int ARR_SIZE = 1024;

struct Coordinate
{
    double x, y;

    Coordinate(double a, double b)
    {
        x = a;
        y = b;
    }
};

enum TargetCoordinateOperation
{
    SumX,
    SumY,
    XTimesX,
    XTimesY,
    XMinusXMean,
    YMinusYMean,
    YMYMSquare,
    XMXMTimesYMYM
};

__global__ void
operateKernel(const Coordinate *coord, double *vec, double mean, TargetCoordinateOperation op)
{
    int i = threadIdx.x;
    if (op == XMinusXMean)
        vec[i] = coord[i].x - mean;
    else if (op == YMinusYMean)
        vec[i] = coord[i].y - mean;
    else if (op == YMYMSquare)
        vec[i] = (coord[i].y - mean) * 2;
    else if (op == XMXMTimesYMYM)
        vec[i] = (coord[i].x - mean) * (coord[i].y - mean);
}

cudaError_t operateWithCuda(const Coordinate *coordinates, double *vector, unsigned int size, double mean, TargetCoordinateOperation op)
{
    Coordinate *dev_coord;
    double *dev_vec;
    cudaError_t cudaStatus;

    // Choose GPU
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaSetDevice failed! Do you have a CUDA-capable GPU installed?");
        goto Clean;
    }

    // Allocate GPU memory for one input and one output
    cudaStatus = cudaMalloc((void **)&dev_coord, size * sizeof(Coordinate));
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMalloc failed!");
        goto Clean;
    }

    cudaStatus = cudaMalloc((void **)&dev_vec, size * sizeof(double));
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMalloc failed!");
        goto Clean;
    }

    // Copy input data from CPU to GPU
    cudaStatus = cudaMemcpy(dev_coord, coordinates, size * sizeof(Coordinate), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Clean;
    }

    // Launch a kernel on the GPU with one thread for each element
    operateKernel<<<1, size>>>(dev_coord, dev_vec, mean, op);

    // Check for any errors launching the kernel
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Clean;
    }

    // Wait for kernel to finish
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
        goto Clean;
    }

    // Copy output data from GPU to CPU
    cudaStatus = cudaMemcpy(vector, dev_vec, size * sizeof(double), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Clean;
    }

    goto Clean;

Clean:
    cudaFree(dev_coord);
    cudaFree(dev_vec);

    return cudaStatus;
}

__global__ void
pushBackKernel(const Coordinate *coord, double *vec, TargetCoordinateOperation op)
{
    int i = threadIdx.x;
    if (op == SumX)
        vec[i] = coord[i].x;
    else if (op = SumY)
        vec[i] = coord[i].y;
    else if (op == XTimesY)
        vec[i] = coord[i].x * coord[i].y;
    else if (op == XTimesX)
        vec[i] = coord[i].x * coord[i].x;
}

cudaError_t pushBackWithCuda(const Coordinate *coordinates, double *vector, unsigned int size, TargetCoordinateOperation op)
{
    Coordinate *dev_coord;
    double *dev_vec;
    cudaError_t cudaStatus;

    // Choose GPU
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaSetDevice failed! Do you have a CUDA-capable GPU installed?");
        goto Clean;
    }

    // Allocate GPU memory for one input and one output
    cudaStatus = cudaMalloc((void **)&dev_coord, size * sizeof(Coordinate));
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMalloc failed!");
        goto Clean;
    }

    cudaStatus = cudaMalloc((void **)&dev_vec, size * sizeof(double));
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMalloc failed!");
        goto Clean;
    }

    // Copy input data from CPU to GPU
    cudaStatus = cudaMemcpy(dev_coord, coordinates, size * sizeof(Coordinate), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Clean;
    }

    // Launch a kernel on the GPU with one thread for each element
    pushBackKernel<<<1, size>>>(dev_coord, dev_vec, op);

    // Check for any errors launching the kernel
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Clean;
    }

    // Wait for kernel to finish
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
        goto Clean;
    }

    // Copy output data from GPU to CPU
    cudaStatus = cudaMemcpy(vector, dev_vec, size * sizeof(double), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Clean;
    }

    goto Clean;

Clean:
    cudaFree(dev_coord);
    cudaFree(dev_vec);

    return cudaStatus;
}

double accumulate(double sum, int n, const double vec[])
{
    for (int i = 0; i < n; i++)
    {
        sum += vec[i];
    }
    return sum;
}

double CalculateGradient(const std::vector<Coordinate> &coordinates)
{
    // auto timer = Timer("CalculateGradient");

    int n = coordinates.size();
    const Coordinate *coord = &coordinates[0];

    // Sum of x*y
    double vec_xy[ARR_SIZE], sum_xy = 0;
    pushBackWithCuda(coord, vec_xy, n, XTimesY);
    sum_xy = accumulate(sum_xy, n, vec_xy);

    // Sum of x
    double vec_x[ARR_SIZE], sum_x = 0;
    pushBackWithCuda(coord, vec_x, n, SumX);
    sum_x = accumulate(sum_x, n, vec_x);

    // Sum of y
    double vec_y[ARR_SIZE], sum_y = 0;
    pushBackWithCuda(coord, vec_y, n, SumY);
    sum_x = accumulate(sum_y, n, vec_y);

    // Sum of x*x
    double vec_xx[ARR_SIZE], sum_xx = 0;
    pushBackWithCuda(coord, vec_xx, n, XTimesX);
    sum_xx = accumulate(sum_xx, n, vec_xx);

    return (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
}

double CalculateMultiGradient(const std::vector<Coordinate> &coordinates)
{
    int n = coordinates.size();
    const Coordinate *coord = &coordinates[0];

    // Calculate mean_x
    double vec_x[ARR_SIZE];
    pushBackWithCuda(coord, vec_x, n, SumX);
    double sum_x = accumulate(sum_x, n, vec_x);
    double mean_x = sum_x / n;

    // Calculate mean_y
    double vec_y[ARR_SIZE];
    pushBackWithCuda(coord, vec_y, n, SumY);
    double sum_y = accumulate(sum_y, n, vec_y);
    double mean_y = sum_y / n;

    // ???????? mind block
}

double CalculateYIntercept(const std::vector<Coordinate> &coordinates, const double gradient)
{
    int n = coordinates.size();
    const Coordinate *coord = &coordinates[0];

    // Sum of y
    double vec_y[ARR_SIZE], sum_y = 0;
    pushBackWithCuda(coord, vec_y, n, SumY);
    sum_y = accumulate(sum_y, n, vec_y);

    // Sum of x
    double vec_x[ARR_SIZE], sum_x = 0;
    pushBackWithCuda(coord, vec_x, n, SumX);
    sum_x = accumulate(sum_x, n, vec_x);

    return (1 / (double)n) * (sum_y - gradient * sum_x);
}

// cudaDeviceReset must be called before exiting in order for profiling and
// tracing tools such as Nsight and Visual Profiler to show complete traces.
void EndCuda()
{
    cudaError_t cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaDeviceReset failed!");
    }
}