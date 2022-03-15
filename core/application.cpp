#include "application.h"

#include <iostream>

#include "core/linear_regression/least_square.h"
#include "core/coordinate.h"
#include "core/entity/player_salary.h"
#include "core/entity/player_attributes.h"
#include "core/player_data.h"
#include "util/time.h"

using namespace sqlite_orm;

namespace DSPC
{
  // constructor for the Application class
  Application::Application(std::string db_path)
  {
    using namespace DSPC::Entity;

    // initialize UI state
    this->cli_state = DSPC::UI::CLIState{std::vector<DSPC::PlayerData>()};
    this->gui_state = DSPC::UI::GUIState{false, false, std::vector<DSPC::PlayerData>()};

    // create the storage object
    auto storage = make_storage(db_path,
                                PlayerSalary::TableBuilder(),
                                PlayerAttributes::TableBuilder());

    // simulate sync (to safely check if schema matches)
    auto result = storage.sync_schema_simulate();

    // throw an error if schema does not match
    for (auto it = result.cbegin(); it != result.cend(); it++)
      if (it->second != sync_schema_result::already_in_sync)
        throw std::runtime_error(std::string("Failed to sync schema for table '" + it->first + "'\n"));

    // actually syncs the db
    storage.sync_schema();

    // SELECT PS.namePlayer, PA.PTS, PA.AST, PA.REB, MAX(IFNULL(PS.value, 0))
    // FROM Player_Salary PS, Player_Attributes PA
    // WHERE PA.FIRST_NAME || ' ' || PA.LAST_NAME = PS.namePlayer
    // GROUP BY PS.namePlayer
    // ORDER BY PS.value DESC;
    auto rows = storage.select(
        columns(&PlayerSalary::PlayerSalary::namePlayer, &PlayerAttributes::PlayerAttributes::PTS, &PlayerAttributes::PlayerAttributes::AST, &PlayerAttributes::PlayerAttributes::REB, max(ifnull<double>(&PlayerSalary::PlayerSalary::value, 0))),
        where(
            c(
                c(&PlayerAttributes::PlayerAttributes::FIRST_NAME) || " " || c(&PlayerAttributes::PlayerAttributes::LAST_NAME)) == &PlayerSalary::PlayerSalary::namePlayer),
        group_by(&PlayerSalary::PlayerSalary::namePlayer),
        order_by(&PlayerSalary::PlayerSalary::value).desc());

    for (auto &row : rows)
    {
      // skip invalid data
      if (std::get<0>(row) == nullptr || std::get<1>(row) == nullptr || std::get<2>(row) == nullptr || std::get<3>(row) == nullptr || std::get<4>(row) == nullptr)
        continue;

      this->cli_state.players.push_back(PlayerData{*std::get<0>(row), *std::get<1>(row), *std::get<2>(row), *std::get<3>(row), *std::get<4>(row)});
      this->gui_state.players.push_back(PlayerData{*std::get<0>(row), *std::get<1>(row), *std::get<2>(row), *std::get<3>(row), *std::get<4>(row)});
    }

    std::cout << "[DEBUG] " << this->gui_state.players.size() << " player's data are loaded." << '\n';
  }

  // destructor for the Application class
  Application::~Application() {}

  void Application::RunCLI(Technique technique)
  {
    auto timer = DSPC::Util::Timer("RunCLI");

    DSPC::UI::RenderCLI(&this->cli_state);
  }

  void Application::RunGUI(Technique technique)
  {
    GLFWwindow *window;
    const char *glsl_version;
    std::tie(window, glsl_version) = DSPC::UI::SetupWindow(1280, 720, "Linear Regression GUI");

    DSPC::UI::SetupImGui(window, glsl_version);

    DSPC::UI::GUIState *gui_state = &this->gui_state;

    DSPC::UI::MainLoop(window, [gui_state]()
                       { DSPC::UI::RenderGUI(gui_state); });

    DSPC::UI::CleanUp(window);
  }

  void Application::Run(ApplicationType type, Technique technique)
  {
    if (type == ApplicationType::CLI)
      RunCLI(technique);
    else if (type == ApplicationType::GUI)
      RunGUI(technique);
  }
}
