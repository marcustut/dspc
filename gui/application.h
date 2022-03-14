#pragma once

#include <string>
#include <vector>
#include <functional>

namespace DSPC
{
  enum class ApplicationType
  {
    GUI,
    CLI
  };

  struct PlayerData
  {
    std::string name;
    double points;
    double assists;
    double rebounds;
    double salary;
  };

  class Application
  {
  private:
    bool show_sequential_window = false;
    bool show_parallel_window = false;
    std::vector<PlayerData> players;

    void RenderUI();
    void RunCLI();
    void RunGUI();

  public:
    Application(std::string db_path);
    ~Application();
    void Run(ApplicationType type);
  };

  void WrapInDockSpace(std::function<void(void)> render_func);
}
