#pragma once

namespace DSPC
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
