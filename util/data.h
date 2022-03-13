#pragma once

#include <string>
#include <sqlite3.h>

namespace DSPC::Util
{
  class Data
  {
  private:
    sqlite3 *db;

  public:
    Data(std::string db_path);
    ~Data();

    void Test();
  };
}
