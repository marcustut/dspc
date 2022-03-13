#pragma once

#include <sqlite_orm/sqlite_orm.h>

namespace DSPC::Entity::PlayerAttributes
{
  const std::string table_name = "Player_Attributes";

  struct PlayerAttributes
  {
    std::unique_ptr<std::string> ID;
    std::unique_ptr<std::string> FIRST_NAME;
    std::unique_ptr<std::string> LAST_NAME;
    std::unique_ptr<std::string> DISPLAY_FIRST_LAST;
    std::unique_ptr<std::string> DISPLAY_LAST_COMMA_FIRST;
    std::unique_ptr<std::string> DISPLAY_FI_LAST;
    std::unique_ptr<std::string> PLAYER_SLUG;
    std::unique_ptr<std::string> BIRTHDATE;
    std::unique_ptr<std::string> SCHOOL;
    std::unique_ptr<std::string> COUNTRY;
    std::unique_ptr<std::string> LAST_AFFILIATION;
    std::unique_ptr<double> HEIGHT;
    std::unique_ptr<double> WEIGHT;
    std::unique_ptr<int> SEASON_EXP;
    std::unique_ptr<std::string> JERSEY;
    std::unique_ptr<std::string> POSITION;
    std::unique_ptr<std::string> ROSTERSTATUS;
    std::unique_ptr<std::string> GAMES_PLAYED_CURRENT_SEASON_FLAG;
    std::unique_ptr<std::string> TEAM_ID;
    std::unique_ptr<std::string> TEAM_NAME;
    std::unique_ptr<std::string> TEAM_ABBREVIATION;
    std::unique_ptr<std::string> TEAM_CODE;
    std::unique_ptr<std::string> TEAM_CITY;
    std::unique_ptr<std::string> PLAYERCODE;
    std::unique_ptr<std::string> FROM_YEAR;
    std::unique_ptr<std::string> TO_YEAR;
    std::unique_ptr<std::string> DLEAGUE_FLAG;
    std::unique_ptr<std::string> NBA_FLAG;
    std::unique_ptr<std::string> GAMES_PLAYED_FLAG;
    std::unique_ptr<std::string> DRAFT_YEAR;
    std::unique_ptr<std::string> DRAFT_ROUND;
    std::unique_ptr<std::string> DRAFT_NUMBER;
    std::unique_ptr<double> PTS;
    std::unique_ptr<double> AST;
    std::unique_ptr<double> REB;
    std::unique_ptr<double> ALL_STAR_APPEARANCES;
    std::unique_ptr<double> PIE;
  };

  auto TableBuilder()
  {
    return sqlite_orm::make_table("Player_Attributes",
                                  sqlite_orm::make_column("ID", &PlayerAttributes::ID),
                                  sqlite_orm::make_column("FIRST_NAME", &PlayerAttributes::FIRST_NAME),
                                  sqlite_orm::make_column("LAST_NAME", &PlayerAttributes::LAST_NAME),
                                  sqlite_orm::make_column("DISPLAY_FIRST_LAST", &PlayerAttributes::DISPLAY_FIRST_LAST),
                                  sqlite_orm::make_column("DISPLAY_LAST_COMMA_FIRST", &PlayerAttributes::DISPLAY_LAST_COMMA_FIRST),
                                  sqlite_orm::make_column("DISPLAY_FI_LAST", &PlayerAttributes::DISPLAY_FI_LAST),
                                  sqlite_orm::make_column("PLAYER_SLUG", &PlayerAttributes::PLAYER_SLUG),
                                  sqlite_orm::make_column("BIRTHDATE", &PlayerAttributes::BIRTHDATE),
                                  sqlite_orm::make_column("SCHOOL", &PlayerAttributes::SCHOOL),
                                  sqlite_orm::make_column("COUNTRY", &PlayerAttributes::COUNTRY),
                                  sqlite_orm::make_column("LAST_AFFILIATION", &PlayerAttributes::LAST_AFFILIATION),
                                  sqlite_orm::make_column("HEIGHT", &PlayerAttributes::HEIGHT),
                                  sqlite_orm::make_column("WEIGHT", &PlayerAttributes::WEIGHT),
                                  sqlite_orm::make_column("SEASON_EXP", &PlayerAttributes::SEASON_EXP),
                                  sqlite_orm::make_column("JERSEY", &PlayerAttributes::JERSEY),
                                  sqlite_orm::make_column("POSITION", &PlayerAttributes::POSITION),
                                  sqlite_orm::make_column("ROSTERSTATUS", &PlayerAttributes::ROSTERSTATUS),
                                  sqlite_orm::make_column("GAMES_PLAYED_CURRENT_SEASON_FLAG", &PlayerAttributes::GAMES_PLAYED_CURRENT_SEASON_FLAG),
                                  sqlite_orm::make_column("TEAM_ID", &PlayerAttributes::TEAM_ID),
                                  sqlite_orm::make_column("TEAM_NAME", &PlayerAttributes::TEAM_NAME),
                                  sqlite_orm::make_column("TEAM_ABBREVIATION", &PlayerAttributes::TEAM_ABBREVIATION),
                                  sqlite_orm::make_column("TEAM_CODE", &PlayerAttributes::TEAM_CODE),
                                  sqlite_orm::make_column("TEAM_CITY", &PlayerAttributes::TEAM_CITY),
                                  sqlite_orm::make_column("PLAYERCODE", &PlayerAttributes::PLAYERCODE),
                                  sqlite_orm::make_column("FROM_YEAR", &PlayerAttributes::FROM_YEAR),
                                  sqlite_orm::make_column("TO_YEAR", &PlayerAttributes::TO_YEAR),
                                  sqlite_orm::make_column("DLEAGUE_FLAG", &PlayerAttributes::DLEAGUE_FLAG),
                                  sqlite_orm::make_column("NBA_FLAG", &PlayerAttributes::NBA_FLAG),
                                  sqlite_orm::make_column("GAMES_PLAYED_FLAG", &PlayerAttributes::GAMES_PLAYED_FLAG),
                                  sqlite_orm::make_column("DRAFT_YEAR", &PlayerAttributes::DRAFT_YEAR),
                                  sqlite_orm::make_column("DRAFT_ROUND", &PlayerAttributes::DRAFT_ROUND),
                                  sqlite_orm::make_column("DRAFT_NUMBER", &PlayerAttributes::DRAFT_NUMBER),
                                  sqlite_orm::make_column("PTS", &PlayerAttributes::PTS),
                                  sqlite_orm::make_column("AST", &PlayerAttributes::AST),
                                  sqlite_orm::make_column("REB", &PlayerAttributes::REB),
                                  sqlite_orm::make_column("ALL_STAR_APPEARANCES", &PlayerAttributes::ALL_STAR_APPEARANCES),
                                  sqlite_orm::make_column("PIE", &PlayerAttributes::PIE));
  }
}