#include "cli.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>

#include "core/linear_regression/least_square.h"
#include "util/csv.h"

std::vector<DSPC::MultivariateCoordinate> read_mock_data(const char *filepath)
{
  std::vector<DSPC::MultivariateCoordinate> data;

  io::CSVReader<5> in(filepath);
  in.read_header(io::ignore_extra_column, "name", "points", "skill", "assists", "salary");
  std::string name, skill;
  double points, assists, salary;

  // read all rows
  while (in.read_row(name, points, skill, assists, salary))
    data.push_back((DSPC::MultivariateCoordinate){{points, assists}, salary});

  // read 10k rows
  for (int i = 0; i < 500; i++)
  {
    in.read_row(name, points, skill, assists, salary);
    data.push_back((DSPC::MultivariateCoordinate){{points, assists}, salary});
  }

  return data;
}

namespace DSPC::UI
{
  void RenderCLI(CLIState *state, DSPC::Technique technique)
  {
    // for (auto &player : state->players)
    //   printf("  %s: (%.1f PTS) (%.1f AST) (%.1f REB) - $%.2f\n", player.name.c_str(), player.points, player.assists, player.rebounds, player.salary);

    // std::vector<MultivariateCoordinate> data;
    // std::transform(state->players.begin(), state->players.end(), std::back_inserter(data), [](PlayerData pd)
    //                { return (MultivariateCoordinate){{pd.points, pd.assists}, pd.salary}; });

    // for (auto &mc : data)
    //   fmt::print("PTS: {}, ASSISTS: {}, SALARY: {}\n", mc.xs[0], mc.xs[1], mc.y);

    // std::vector<MultivariateCoordinate> coords{
    //     (MultivariateCoordinate){{7, 8}, 1000},
    //     (MultivariateCoordinate){{6, 6}, 1789},
    //     (MultivariateCoordinate){{8, 9}, 2000},
    //     (MultivariateCoordinate){{9, 8}, 2400},
    //     (MultivariateCoordinate){{14, 13}, 3800},
    //     (MultivariateCoordinate){{19, 18}, 4000},
    // };

    auto linreg = new DSPC::LinearRegression::LeastSquare(technique, DSPC::Type::Multivariate, read_mock_data("dataset/mock.csv"));
    linreg->InitModel();

    std::cout << linreg->Formula() << std::endl;
  }
}