#pragma once

#include <string>
#include <vector>
#include <functional>

#include "ui/cli.h"
#include "ui/gui.h"
#include "core/technique.h"

namespace DSPC
{
  enum class ApplicationType
  {
    GUI,
    CLI
  };

  class Application
  {
  private:
    DSPC::UI::CLIState cli_state;
    DSPC::UI::GUIState gui_state;

    void RunCLI(Technique technique);
    void RunGUI(Technique technique);

  public:
    Application(std::string file_path, int *num_of_rows);
    ~Application();
    void Run(ApplicationType type, Technique technique);
  };

  void WrapInDockSpace(std::function<void(void)> render_func);
}
