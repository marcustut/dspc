#include "data.h"

namespace DSPC::Util
{
  Data::Data(std::string db_path)
  {
    int status = sqlite3_open(db_path.c_str(), &this->db);

    if (status != SQLITE_OK)
      fprintf(stderr, "Can't open database [%s]: %s\n", db_path.c_str(), sqlite3_errmsg(db));
    else
      fprintf(stdout, "Opened database successfully\n");
  }

  Data::~Data()
  {
    if (sqlite3_close(db) != SQLITE_OK)
      fprintf(stderr, "Unable to close database\n");
  }

  void Data::Test()
  {
    char *errMsg = 0;
    const char *query = "SELECT * FROM Player_Salary";

    int status = sqlite3_exec(
        this->db, query, [](void *data, int argc, char **argv, char **azColName)
        {
          for (int i = 0; i < argc; i++) 
          {
            printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
          }
          printf("\n");
          return 0; },
        nullptr, &errMsg);

    if (status != SQLITE_OK)
    {
      fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
    }
    else
      fprintf(stdout, "Operation done successfully\n");
  }
}
