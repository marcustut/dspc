#include "least_square.h"

#include <cstdio>
#include <stdexcept>

#include "core/linear_regression/serial.h"
#include "core/linear_regression/open_mp.h"
#include "core/linear_regression/pthread.h"
#include "core/linear_regression/cpp_std_lib.h"
#include "util/time.h"

namespace DSPC::LinearRegression
{
  LeastSquare::LeastSquare(Technique technique, Type type)
  {
    this->technique = technique;
    this->type = type;
  }

  LeastSquare::LeastSquare(Technique technique, Type type, std::vector<Coordinate> coordinates)
  {
    this->coordinates = coordinates;
    this->technique = technique;
    this->type = type;
  }

  LeastSquare::LeastSquare(Technique technique, Type type, std::vector<MultivariateCoordinate> multivariate_coordinates)
  {
    this->multivariate_coordinates = multivariate_coordinates;
    this->technique = technique;
    this->type = type;
  }

  LeastSquare::~LeastSquare()
  {
  }

  void LeastSquare::SetCoordinates(std::vector<Coordinate> coordinates)
  {
    this->coordinates = coordinates;
  }

  void LeastSquare::SetCoordinates(std::vector<MultivariateCoordinate> multivariate_coordinates)
  {
    this->multivariate_coordinates = multivariate_coordinates;
  }

  void LeastSquare::InitModel()
  {
    auto timer = DSPC::Util::Timer("InitModel");

    // check if coordinates are set
    if ((this->type == Type::Normal && coordinates.empty()) || (this->type == Type::Multivariate && multivariate_coordinates.empty()))
      throw std::logic_error(std::string("coordinates are empty"));

    // calculate gradient and y-intercept based on the type
    if (this->type == Type::Normal)
    {
      this->CalculateGradient();
      this->CalculateYIntercept();
    }
    else
      this->CalculateGradientAndYIntercept();
  }

  void LeastSquare::CalculateGradient()
  {
    switch (this->technique)
    {
    case Technique::Serial:
      this->m = Serial::CalculateGradient(this->coordinates);
      break;
    case Technique::OpenMP:
      this->m = OpenMP::CalculateGradient(this->coordinates);
      break;
    case Technique::Pthread:
      this->m = Pthread::CalculateGradient(this->coordinates);
      break;
    case Technique::CppStdLib:
      this->m = CppStdLib::CalculateGradient(this->coordinates);
      break;
    case Technique::CUDA:
      break;
    }
  }

  void LeastSquare::CalculateYIntercept()
  {
    switch (this->technique)
    {
    case Technique::Serial:
      this->c = Serial::CalculateYIntercept(this->coordinates, this->m);
      break;
    case Technique::OpenMP:
      this->c = OpenMP::CalculateYIntercept(this->coordinates, this->m);
      break;
    case Technique::Pthread:
      this->c = Pthread::CalculateYIntercept(this->coordinates, this->m);
      break;
    case Technique::CppStdLib:
      this->c = CppStdLib::CalculateYIntercept(this->coordinates, this->m);
      break;
    case Technique::CUDA:
      break;
    }
  }

  void LeastSquare::CalculateGradientAndYIntercept()
  {
    switch (this->technique)
    {
    case Technique::Serial:
      std::forward_as_tuple(std::tie(this->b1, this->b2), this->a) = Serial::CalculateGradientAndYIntercept(this->multivariate_coordinates);
      break;
    case Technique::OpenMP:
      std::forward_as_tuple(std::tie(this->b1, this->b2), this->a) = OpenMP::CalculateGradientAndYIntercept(this->multivariate_coordinates);
      break;
    case Technique::Pthread:
      std::forward_as_tuple(std::tie(this->b1, this->b2), this->a) = Pthread::CalculateGradientAndYIntercept(this->multivariate_coordinates);
      break;
    case Technique::CppStdLib:
      std::forward_as_tuple(std::tie(this->b1, this->b2), this->a) = CppStdLib::CalculateGradientAndYIntercept(this->multivariate_coordinates);
      break;
    case Technique::CUDA:
      break;
    }
  }

  std::variant<Coordinate, MultivariateCoordinate> LeastSquare::PredictX(double Y)
  {
    if (this->type == Type::Normal)
      // x = (y - c) / m
      return (Coordinate){(Y - this->c) / this->m, Y};
    else
      return MultivariateCoordinate{};
  }

  Coordinate LeastSquare::PredictY(double X)
  {
    // y = mx + c
    return (Coordinate){X, (this->m * X) + this->c};
  }

  MultivariateCoordinate LeastSquare::PredictY(std::array<double, 2> Xs)
  {
    // y = b1(x1) + b2(x2) - a
    return MultivariateCoordinate{Xs, (Xs[0] * this->b1) + (Xs[1] * this->b2) - this->a};
  }

  std::string LeastSquare::Formula()
  {
    if (this->type == Type::Normal)
      return "y = " + std::to_string(this->m) + "x + " + std::to_string(this->c);
    else
      return "y = " + std::to_string(this->b1) + "x₁ + " + std::to_string(this->b2) + "x₂ + " + std::to_string(this->a);
  }
} // namespace DSPC
