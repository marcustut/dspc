#ifdef _MSC_VER
// not #if defined(_WIN32) || defined(_WIN64) because we have strncasecmp in mingw
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

#include <stdio.h>

#include "core/application.h"
#include "core/technique.h"

int main(int argc, char **argv)
{
  std::string db_path = "dataset/basketball.sqlite";
  DSPC::ApplicationType app_type;
  DSPC::Technique technique;

  // Check if user wants to run the CLI or GUI
  if (argc < 3 || argc > 4)
  {
    fprintf(stderr, "Usage: ./main <type> <technique> [dataset]\n");
    fprintf(stderr, " <type>          Type of application to open, specify either 'GUI' or 'CLI'\n");
    fprintf(stderr, " <technique>     Type of technique to perform the algorithm, specify one of 'Serial', 'OpenMP', 'Pthread', 'CppStdLib', 'CUDA'\n");
    fprintf(stderr, " [dataset]       Relative path to dataset (a sqlite database), default is '%s' if not provided\n", db_path.c_str());
    return 1;
  }

  // set db_path if specified
  if (argv[3] != nullptr)
    db_path = std::string(argv[3]);

  // verify the type of app to run
  if (strcasecmp(argv[1], "CLI") == 0)
    app_type = DSPC::ApplicationType::CLI;
  else if (strcasecmp(argv[1], "GUI") == 0)
    app_type = DSPC::ApplicationType::GUI;
  else
  {
    fprintf(stderr, "<type> must be either 'GUI' or 'CLI'\n");
    return 1;
  }

  // verify the technique
  if (strcasecmp(argv[2], "Serial") == 0)
    technique = DSPC::Technique::Serial;
  else if (strcasecmp(argv[2], "OpenMP") == 0)
    technique = DSPC::Technique::OpenMP;
  else if (strcasecmp(argv[2], "Pthread") == 0)
    technique = DSPC::Technique::Pthread;
  else if (strcasecmp(argv[2], "CppStdLib") == 0)
    technique = DSPC::Technique::CppStdLib;
  else if (strcasecmp(argv[2], "CUDA") == 0)
    technique = DSPC::Technique::CUDA;
  else
  {
    fprintf(stderr, "<technique> must be one of 'Serial', 'OpenMP', 'Pthread', 'CppStdLib', 'CUDA'");
    return 1;
  }

  // initialize application
  DSPC::Application app = DSPC::Application(db_path);

  // run the app
  app.Run(app_type, technique);

  return 0;
}
