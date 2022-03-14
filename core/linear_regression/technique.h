#pragma once

namespace DSPC::LinearRegression
{
  enum class Technique
  {
    Serial,
    OpenMP,
    Pthread,
    CppStdLib,
    CUDA,
  };
}
