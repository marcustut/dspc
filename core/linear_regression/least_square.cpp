#include "least_square.h"

#include <cstdio>
#include <stdexcept>

#include "core/linear_regression/serial.h"
#include "core/linear_regression/pthread.h"

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
    // check if coordinates are set
    if ((this->type == Type::Normal && coordinates.empty()) || (this->type == Type::Multivariate && multivariate_coordinates.empty()))
      throw std::logic_error(std::string("coordinates are empty"));

    this->CalculateGradient();
    this->CalculateYIntercept();
  }

  void LeastSquare::CalculateGradient()
  {
    switch (this->technique)
    {
    case Technique::Serial:
      if (this->type == Type::Normal)
        this->m = Serial::CalculateGradient(this->coordinates);
      else
        std::tie(this->b1, this->b2) = Serial::CalculateGradient(this->multivariate_coordinates);
      break;
    case Technique::OpenMP:
      break;
    case Technique::Pthread:
      if (this->type == Type::Normal)
        this->m = Pthread::CalculateGradient(this->coordinates);
      else
        std::tie(this->b1, this->b2) = Pthread::CalculateGradient(this->multivariate_coordinates);
      break;
    case Technique::CppStdLib:
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
      if (this->type == Type::Normal)
        this->c = Serial::CalculateYIntercept(this->coordinates, this->m);
      else
        this->a = Serial::CalculateYIntercept(this->multivariate_coordinates, std::make_tuple(b1, b2));
      break;
    case Technique::OpenMP:
      break;
    case Technique::Pthread:
      if (this->type == Type::Normal)
        this->c = Pthread::CalculateYIntercept(this->coordinates, this->m);
      else
        this->a = Pthread::CalculateYIntercept(this->multivariate_coordinates, std::make_tuple(b1, b2));
      break;
    case Technique::CppStdLib:
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

  MultivariateCoordinate LeastSquare::PredictY(std::vector<double> Xs)
  {
    // x = (y - c) / m
    return MultivariateCoordinate{};
  }

  std::string LeastSquare::Formula()
  {
    return "y = " + std::to_string(this->m) + "x + " + std::to_string(this->c);
  }
} // namespace DSPC
