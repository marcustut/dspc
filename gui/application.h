#pragma once

#include <functional>

namespace DSPC
{
  class Application
  {
  private:
    bool show_sequential_window;
    bool show_parallel_window;

    void RenderUI();

  public:
    Application(bool show_sequential_window, bool show_parallel_window);
    ~Application();
    void Run();
  };

  void WrapInDockSpace(std::function<void(void)> render_func);

} // namespace DSPC
