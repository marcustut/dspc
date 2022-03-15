#pragma once

#include <vector>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "core/player_data.h"

namespace DSPC::UI
{
  // Core functions
  std::tuple<GLFWwindow *, const char *> SetupWindow(int width, int height, const char *title);
  void SetupImGui(GLFWwindow *window, const char *glsl_version);
  void MainLoop(GLFWwindow *window, std::function<void(void)> render_func);
  void CleanUp(GLFWwindow *window);

  // State
  struct GUIState
  {
    bool show_sequential_window;
    bool show_parallel_window;
    std::vector<DSPC::PlayerData> players;
  };

  // Utility
  void WrapInDockSpace(std::function<void(void)> render_func);
  void RenderGUI(GUIState *state);
}