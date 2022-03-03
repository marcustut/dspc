#include "least_square.h"

#include <string>
#include <cstdio>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <stdexcept>
#endif

#include "serial.h"

namespace DSPC::LinearRegression
{
  LeastSquare::LeastSquare(Technique technique)
  {
    this->technique = technique;
  }

  LeastSquare::LeastSquare(Technique technique, std::vector<Coordinate> coordinates)
  {
    this->coordinates = coordinates;
    this->technique = technique;
  }

  LeastSquare::~LeastSquare()
  {
  }

  void LeastSquare::SetCoordinates(std::vector<Coordinate> coordinates)
  {
    this->coordinates = coordinates;
  }

  void LeastSquare::InitModel()
  {
    this->CalculateGradient();
    this->CalculateYIntercept();
  }

  void LeastSquare::CalculateGradient()
  {
    if (coordinates.empty())
      throw std::logic_error(std::string("coordinates are empty"));

    switch (this->technique)
    {
    case Technique::Serial:
      this->m = Serial::CalculateGradient(this->coordinates);
      break;
    case Technique::OpenMP:
      break;
    case Technique::Pthread:
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
      this->c = Serial::CalculateYIntercept(this->coordinates, this->m);
      break;
    case Technique::OpenMP:
      break;
    case Technique::Pthread:
      break;
    case Technique::CppStdLib:
      break;
    case Technique::CUDA:
      break;
    }
  }

  Coordinate LeastSquare::PredictX(double Y)
  {
    // x = (y - c) / m
    return (Coordinate){(Y - this->c) / this->m, Y};
  }

  Coordinate LeastSquare::PredictY(double X)
  {
    // y = mx + c
    return (Coordinate){X, (this->m * X) + this->c};
  }

  std::string LeastSquare::Formula()
  {
    return "y = " + std::to_string(this->m) + "x + " + std::to_string(this->c);
  }
} // namespace DSPC
