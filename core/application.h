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

    void RunCLI();
    void RunGUI();

  public:
    Application(std::string db_path);
    ~Application();
    void Run(ApplicationType type, Technique technique);
  };

  void WrapInDockSpace(std::function<void(void)> render_func);
}
