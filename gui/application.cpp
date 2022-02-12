#include "application.h"

#include "imgui.h"

namespace DSPC
{
  void RenderUI()
  {
    ImGui::Begin("DSPC Assignment - Machine Learning");
    ImGui::ShowDemoWindow();
    ImGui::End();
  }
} // namespace DSPC
