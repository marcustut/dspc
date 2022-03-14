#include <stdio.h>

#include "gui/application.h"

int main(int argc, char **argv)
{
  std::string db_path = "dataset/basketball.sqlite";

  // Check if user wants to run the CLI or GUI
  if (argc < 2 || argc > 3)
  {
    fprintf(stderr, "Usage: ./main <type> [dataset]\n");
    fprintf(stderr, " <type>      Type of application to open, specify either 'GUI' or 'CLI'\n");
    fprintf(stderr, " [dataset]   Relative path to dataset (a sqlite database), default is '%s' if not provided\n", db_path.c_str());
    return 1;
  }

  // set db_path if specified
  if (argv[2] != nullptr)
    db_path = std::string(argv[2]);

  // initialize application
  DSPC::Application app = DSPC::Application(db_path);

  // launch CLI if specified
  if (strcmp(argv[1], "CLI") == 0)
    app.Run(DSPC::ApplicationType::CLI);
  else if (strcmp(argv[1], "GUI") == 0)
    app.Run(DSPC::ApplicationType::GUI);
  else
  {
    fprintf(stderr, "<type> must be either 'GUI' or 'CLI'\n");
    return 1;
  }

  return 0;
}
