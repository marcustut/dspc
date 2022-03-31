#include "cli.h"

#include <stdio.h>
#include <iostream>

#include "core/linear_regression/serial.h"

namespace DSPC::UI
{
  void RenderCLI(CLIState *state)
  {
    // for (auto &player : state->players)
    //   printf("  %s: (%.1f PTS) (%.1f AST) (%.1f REB) - $%.2f\n", player.name.c_str(), player.points, player.assists, player.rebounds, player.salary);

    std::vector<MultivariateCoordinate> coords{
        (MultivariateCoordinate){{7, 8}, 1000},
        (MultivariateCoordinate){{6, 6}, 1789},
        (MultivariateCoordinate){{8, 9}, 2000},
        (MultivariateCoordinate){{9, 8}, 2400},
        (MultivariateCoordinate){{14, 13}, 3800},
        (MultivariateCoordinate){{19, 18}, 4000},
    };

    double res = DSPC::LinearRegression::Serial::CalculateGradient(coords);
    std::cout << res << std::endl;
  }
}