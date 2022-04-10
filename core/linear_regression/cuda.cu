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
#include <functional>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <array>
#include "util/csv.h"

static const int ARR_SIZE = 100000;
static const int BLOCK_SIZE = 1024;

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
    // operateKernel<<<1, size>>>(dev_coord, dev_vec, mean, op);
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
    else if (op == SumY)
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

enum Operation
{
    SumOfX1,
    SumOfX2,
    SumOfY,
    SumOfSquaresX1,
    SumOfSquaresX2,
    SumOfProductsX1Y,
    SumOfProductsX2Y,
    SumofProductsX1X2
};

struct MultivariateCoordinate
{
    double xs[2];
    double y;

    MultivariateCoordinate(double x1, double x2, double y)
    {
        this->xs[0] = x1;
        this->xs[1] = x2;
        this->y = y;
    }
};

__global__ void calculate_sum(const MultivariateCoordinate *coord, double *out, Operation op)
{
    int idx = threadIdx.x;
    double sum = 0;

    switch (op)
    {
    case SumOfX1:
        for (int i = idx; i < ARR_SIZE; i += BLOCK_SIZE)
            sum += coord[i].xs[0];
        break;
    case SumOfX2:
        for (int i = idx; i < ARR_SIZE; i += BLOCK_SIZE)
            sum += coord[i].xs[1];
        break;
    case SumOfY:
        for (int i = idx; i < ARR_SIZE; i += BLOCK_SIZE)
            sum += coord[i].y;
        break;
    default:
        break;
    }

    __shared__ double r[BLOCK_SIZE];
    r[idx] = sum;
    __syncthreads();
    for (int size = BLOCK_SIZE / 2; size > 0; size /= 2)
    { // uniform
        if (idx < size)
            r[idx] += r[idx + size];
        __syncthreads();
    }
    if (idx == 0)
        *out = r[0];
}

__global__ void calculate_sum(const MultivariateCoordinate *coord, double *out, double mean, Operation op)
{
    int idx = threadIdx.x;
    double sum = 0;

    switch (op)
    {
    case SumOfSquaresX1:
        for (int i = idx; i < ARR_SIZE; i += BLOCK_SIZE)
            sum += (coord[i].xs[0] - mean) * (coord[i].xs[0] - mean);
        break;
    case SumOfSquaresX2:
        for (int i = idx; i < ARR_SIZE; i += BLOCK_SIZE)
            sum += (coord[i].xs[1] - mean) * (coord[i].xs[1] - mean);
        break;
    default:
        break;
    }

    __shared__ double r[BLOCK_SIZE];
    r[idx] = sum;
    __syncthreads();
    for (int size = BLOCK_SIZE / 2; size > 0; size /= 2)
    { // uniform
        if (idx < size)
            r[idx] += r[idx + size];
        __syncthreads();
    }
    if (idx == 0)
        *out = r[0];
}

__global__ void calculate_sum(const MultivariateCoordinate *coord, double *out, double mean1, double mean2, Operation op)
{
    int idx = threadIdx.x;
    double sum = 0;

    switch (op)
    {
    case SumOfProductsX1Y:
        for (int i = idx; i < ARR_SIZE; i += BLOCK_SIZE)
            sum += (coord[i].xs[0] - mean1) * (coord[i].y - mean2);
        break;
    case SumOfProductsX2Y:
        for (int i = idx; i < ARR_SIZE; i += BLOCK_SIZE)
            sum += (coord[i].xs[1] - mean1) * (coord[i].y - mean2);
        break;
    case SumofProductsX1X2:
        for (int i = idx; i < ARR_SIZE; i += BLOCK_SIZE)
            sum += (coord[i].xs[0] - mean1) * (coord[i].xs[1] - mean2);
        break;
    default:
        break;
    }

    __shared__ double r[BLOCK_SIZE];
    r[idx] = sum;
    __syncthreads();
    for (int size = BLOCK_SIZE / 2; size > 0; size /= 2)
    { // uniform
        if (idx < size)
            r[idx] += r[idx + size];
        __syncthreads();
    }
    if (idx == 0)
        *out = r[0];
}

double SumWithCuda(const std::vector<MultivariateCoordinate> &mc, Operation op)
{
    // Variables on CPU
    const MultivariateCoordinate *coord = &mc[0];
    double out[ARR_SIZE];
    int n = mc.size();

    // Variables on GPU
    MultivariateCoordinate *dev_coord;
    double *dev_out;
    cudaError_t cudaStatus;

    // Choose GPU
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Clean;
    }

    // Allocate memory on GPU
    cudaStatus = cudaMalloc((void **)&dev_coord, n * sizeof(MultivariateCoordinate));
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMalloc failed!");
        goto Clean;
    }

    cudaStatus = cudaMalloc((void **)&dev_out, n * sizeof(double));
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMalloc failed!");
        goto Clean;
    }

    // Copy input data from CPU to GPU
    cudaStatus = cudaMemcpy(dev_coord, coord, n * sizeof(MultivariateCoordinate), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMemcpy input failed!");
        goto Clean;
    }

    // Launch kernel
    calculate_sum<<<1, BLOCK_SIZE>>>(dev_coord, dev_out, op);

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

    // Copy output data from output to input
    cudaStatus = cudaMemcpy(out, dev_out, n * sizeof(double), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMemcpy output failed!");
        goto Clean;
    }

    return out[0];

Clean:
    cudaFree(dev_coord);
    cudaFree(dev_out);
}

double SumWithCuda(const std::vector<MultivariateCoordinate> &mc, double mean, Operation op)
{
    // Variables on CPU
    const MultivariateCoordinate *coord = &mc[0];
    double out[ARR_SIZE];
    int n = mc.size();

    // Variables on GPU
    MultivariateCoordinate *dev_coord;
    double *dev_out;
    cudaError_t cudaStatus;

    // Choose GPU
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Clean;
    }

    // Allocate memory on GPU
    cudaStatus = cudaMalloc((void **)&dev_coord, n * sizeof(MultivariateCoordinate));
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMalloc failed!");
        goto Clean;
    }

    cudaStatus = cudaMalloc((void **)&dev_out, n * sizeof(double));
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMalloc failed!");
        goto Clean;
    }

    // Copy input data from CPU to GPU
    cudaStatus = cudaMemcpy(dev_coord, coord, n * sizeof(MultivariateCoordinate), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMemcpy input failed!");
        goto Clean;
    }

    // Launch kernel
    calculate_sum<<<1, BLOCK_SIZE>>>(dev_coord, dev_out, mean, op);

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

    // Copy output data from output to input
    cudaStatus = cudaMemcpy(out, dev_out, n * sizeof(double), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMemcpy output failed!");
        goto Clean;
    }

    return out[0];

Clean:
    cudaFree(dev_coord);
    cudaFree(dev_out);
}

double SumWithCuda(const std::vector<MultivariateCoordinate> &mc, double mean1, double mean2, Operation op)
{
    // Variables on CPU
    const MultivariateCoordinate *coord = &mc[0];
    double out[ARR_SIZE];
    int n = mc.size();

    // Variables on GPU
    MultivariateCoordinate *dev_coord;
    double *dev_out;
    cudaError_t cudaStatus;

    // Choose GPU
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Clean;
    }

    // Allocate memory on GPU
    cudaStatus = cudaMalloc((void **)&dev_coord, n * sizeof(MultivariateCoordinate));
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMalloc failed!");
        goto Clean;
    }

    cudaStatus = cudaMalloc((void **)&dev_out, n * sizeof(double));
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMalloc failed!");
        goto Clean;
    }

    // Copy input data from CPU to GPU
    cudaStatus = cudaMemcpy(dev_coord, coord, n * sizeof(MultivariateCoordinate), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMemcpy input failed!");
        goto Clean;
    }

    // Launch kernel
    calculate_sum<<<1, BLOCK_SIZE>>>(dev_coord, dev_out, mean1, mean2, op);

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

    // Copy output data from output to input
    cudaStatus = cudaMemcpy(out, dev_out, n * sizeof(double), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMemcpy output failed!");
        goto Clean;
    }

    return out[0];

Clean:
    cudaFree(dev_coord);
    cudaFree(dev_out);
}

std::tuple<std::pair<double, double>, double> CalculateGradientAndYIntercept(const std::vector<MultivariateCoordinate> &mc)
{
    Operation op;

    // Total number of coordinates
    int n = mc.size();

    // Calculate sum of x1, sum of x2 and sum of y
    op = SumOfX1;
    double sum_of_x1 = SumWithCuda(mc, op);
    op = SumOfX2;
    double sum_of_x2 = SumWithCuda(mc, op);
    op = SumOfY;
    double sum_of_y = SumWithCuda(mc, op);

    // Calculate mean
    double mean_x1 = sum_of_x1 / n;
    double mean_x2 = sum_of_x2 / n;
    double mean_y = sum_of_y / n;

    // Calculate sum of squares
    op = SumOfSquaresX1;
    double sum_of_squares_x1 = SumWithCuda(mc, mean_x1, op);
    op = SumOfSquaresX2;
    double sum_of_squares_x2 = SumWithCuda(mc, mean_x2, op);

    // Calculate sum of products
    op = SumOfProductsX1Y;
    double sum_of_products_x1_y = SumWithCuda(mc, mean_x1, mean_y, op);
    op = SumOfProductsX2Y;
    double sum_of_products_x2_y = SumWithCuda(mc, mean_x2, mean_y, op);
    op = SumofProductsX1X2;
    double sum_of_products_x1_x2 = SumWithCuda(mc, mean_x1, mean_x2, op);

    // Calculate b1, b2 and a
    double b1 = (sum_of_products_x1_y * sum_of_squares_x2 - sum_of_products_x1_x2 * sum_of_products_x2_y) / (sum_of_squares_x1 * sum_of_squares_x2 - sum_of_products_x1_x2 * sum_of_products_x1_x2);

    double b2 = (sum_of_products_x2_y * sum_of_squares_x1 - sum_of_products_x1_x2 * sum_of_products_x1_y) / (sum_of_squares_x1 * sum_of_squares_x2 - sum_of_products_x1_x2 * sum_of_products_x1_x2);

    double a = mean_y - (b1 * mean_x1) - (b2 * mean_x2);

    // Test return
    return std::make_tuple(std::make_pair(b1, b2), a);
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

std::vector<MultivariateCoordinate> read_mock_csv(const char *filepath, int *num_of_rows)
{
    io::CSVReader<5> in(filepath);
    in.read_header(io::ignore_extra_column, "name", "points", "skill", "assists", "salary");
    std::string name, skill;
    double points, assists, salary;
    std::vector<MultivariateCoordinate> coord;

    if (num_of_rows == nullptr)
        // read all rows
        while (in.read_row(name, points, skill, assists, salary))
            coord.push_back(MultivariateCoordinate(points, assists, salary));
    else
        // read specified number of rows
        for (int i = 0; i < *num_of_rows; i++)
        {
            in.read_row(name, points, skill, assists, salary);
            coord.push_back(MultivariateCoordinate(points, assists, salary));
        }

    return coord;
}

double PredictY(std::array<double, 2> Xs, double b1, double b2, double a)
{
    return (Xs[0] * b1) + (Xs[1] * b2) - a;
}

int main(int argc, char **argv)
{
    int num_of_rows = -1;

    // Check for user arguments
    if (argc != 2 && argc != 1)
    {
        fprintf(stderr, "Usage: ./cuda [num_of_rows]\n");
        return 1;
    }

    if (argv[1] != nullptr)
        num_of_rows = std::stoi(argv[1]);

    std::vector<MultivariateCoordinate> data = read_mock_csv("dataset/mock.csv", num_of_rows == -1 ? nullptr : &num_of_rows);

    // print how many coordinates in red
    std::cout << "\033[1;31m[DEBUG] " << data.size() << " coordinate is loaded\033[0m" << std::endl;

    double b1 = 0.0, b2 = 0.0, a = 0.0;

    auto start = std::chrono::high_resolution_clock::now();

    std::forward_as_tuple(std::tie(b1, b2), a) = CalculateGradientAndYIntercept(data);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - start;

    const char *operation_name = "InitModel";
    float ms = duration.count() * 1000.0f;
    const char *to_print = operation_name == nullptr ? "An unknown operation" : operation_name;
    std::cout << "\033[1;31m[TIMER] " << to_print << " took " << ms << "ms\033[0m\n";

    // print the straight line formula
    std::cout << "\nThe following is the calculated formula of straight line: " << std::endl;
    std::cout << "y = " + std::to_string(b1) + "x₁ + " + std::to_string(b2) + "x₂ + " + std::to_string(a) << std::endl;

    // get user input
    std::array<double, 2> Xs;
    std::cout << "\nEnter the X values separated by space, for example: 32.3 8.9" << std::endl;
    std::cin >> Xs[0] >> Xs[1];

    // calculate the predicted Y value and print it
    double predicted = PredictY(Xs, b1, b2, a);
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "\nThe predicted Y value is " << predicted << " (4 d.p.)" << std::endl;

    EndCuda();
}