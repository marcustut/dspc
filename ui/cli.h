#pragma once

#include <vector>

#include "core/player_data.h"
#include "core/technique.h"

namespace DSPC::UI
{
  // State
  struct CLIState
  {
    std::vector<DSPC::PlayerData> players;
  };

  // Utility
  void RenderCLI(CLIState *state, DSPC::Technique technique);
}