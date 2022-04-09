#include "cli.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>

#include "core/linear_regression/least_square.h"
#include "util/csv.h"

namespace DSPC::UI
{
  void RenderCLI(CLIState *state, DSPC::Technique technique)
  {
    // transform intermediate state to data
    std::vector<MultivariateCoordinate> data;
    if (state->mocks.size() > 0)
      std::transform(state->mocks.begin(), state->mocks.end(), std::back_inserter(data), [](CSVData cd)
                     { return (MultivariateCoordinate){{cd.points, cd.assists}, cd.salary}; });
    else
      std::transform(state->players.begin(), state->players.end(), std::back_inserter(data), [](PlayerData pd)
                     { return (MultivariateCoordinate){{pd.points, pd.assists}, pd.salary}; });

<<<<<<< HEAD
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
=======
    // print how many coordinates in red
    std::cout << "\033[1;31m[DEBUG] " << data.size() << " coordinate is loaded\033[0m" << std::endl;

    // construct and init linear regression model
    auto linreg = new DSPC::LinearRegression::LeastSquare(technique, DSPC::Type::Multivariate, data);
    linreg->InitModel();

    // print the straight line formula
    std::cout << "\nThe following is the calculated formula of straight line: " << std::endl;
    std::cout << linreg->Formula() << std::endl;

    // get user input
    std::array<double, 2> Xs;
    std::cout << "\nEnter the X values separated by space, for example: 32.3 8.9" << std::endl;
    std::cin >> Xs[0] >> Xs[1];

    // calculate the predicted Y value and print it
    MultivariateCoordinate predicted = linreg->PredictY(Xs);
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "\nThe predicted Y value is " << predicted.y << " (4 d.p.)" << std::endl;
>>>>>>> 8438b5b13e5b32cb4da43e81e51d9fb6bfa51ee2
  }
}