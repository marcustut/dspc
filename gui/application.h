#pragma once

#include <functional>

namespace DSPC
{
  class Application
  {
  private:
    bool show_sequential_window = false;
    bool show_parallel_window = false;

    void RenderUI();
    void RunSequential();

  public:
    Application();
    ~Application();
    void Run();
  };

  void WrapInDockSpace(std::function<void(void)> render_func);
} // namespace DSPC
