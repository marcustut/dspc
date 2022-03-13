#pragma once

#include <sqlite_orm/sqlite_orm.h>

namespace DSPC::Entity::PlayerSalary
{
  const std::string table_name = "Player_Salary";

  struct PlayerSalary
  {
    std::unique_ptr<std::string> slugSeason;
    std::unique_ptr<std::string> nameTeam;
    std::unique_ptr<std::string> namePlayer;
    std::unique_ptr<std::string> statusPlayer;
    std::unique_ptr<int> isFinalSeason;
    std::unique_ptr<int> isWaived;
    std::unique_ptr<int> isOnRoster;
    std::unique_ptr<int> isNonGuaranteed;
    std::unique_ptr<int> isTeamOption;
    std::unique_ptr<int> isPlayerOption;
    std::unique_ptr<std::string> typeContractDetail;
    std::unique_ptr<double> value;
  };

  auto TableBuilder()
  {
    return sqlite_orm::make_table("Player_Salary",
                                  sqlite_orm::make_column("slugSeason", &PlayerSalary::slugSeason),
                                  sqlite_orm::make_column("nameTeam", &PlayerSalary::nameTeam),
                                  sqlite_orm::make_column("namePlayer", &PlayerSalary::namePlayer),
                                  sqlite_orm::make_column("statusPlayer", &PlayerSalary::statusPlayer),
                                  sqlite_orm::make_column("isFinalSeason", &PlayerSalary::isFinalSeason),
                                  sqlite_orm::make_column("isWaived", &PlayerSalary::isWaived),
                                  sqlite_orm::make_column("isOnRoster", &PlayerSalary::isOnRoster),
                                  sqlite_orm::make_column("isNonGuaranteed", &PlayerSalary::isNonGuaranteed),
                                  sqlite_orm::make_column("isTeamOption", &PlayerSalary::isTeamOption),
                                  sqlite_orm::make_column("isPlayerOption", &PlayerSalary::isPlayerOption),
                                  sqlite_orm::make_column("typeContractDetail", &PlayerSalary::typeContractDetail),
                                  sqlite_orm::make_column("value", &PlayerSalary::value));
  }
}