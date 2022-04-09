#pragma once

#include <vector>

#include "core/player_data.h"
#include "core/csv_data.h"
#include "core/technique.h"

namespace DSPC::UI
{
  // State
  struct CLIState
  {
    std::vector<DSPC::PlayerData> players;
    std::vector<DSPC::CSVData> mocks;
    int *num_of_rows;
  };

  // Utility
  void RenderCLI(CLIState *state, DSPC::Technique technique);
}