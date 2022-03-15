#include "cli.h"

#include <stdio.h>

namespace DSPC::UI
{
  void RenderCLI(CLIState *state)
  {
    for (auto &player : state->players)
      printf("  %s: (%.1f PTS) (%.1f AST) (%.1f REB) - $%.2f\n", player.name.c_str(), player.points, player.assists, player.rebounds, player.salary);
  }
}