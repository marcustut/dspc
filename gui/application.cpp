#include "application.h"

#include "imgui.h"

#include <iostream>

namespace DSPC
{
  // constructor for the Application class
  Application::Application(
      bool show_sequential_window,
      bool show_parallel_window) : show_sequential_window(show_sequential_window),
                                   show_parallel_window(show_parallel_window)
  {
  }

  // destructor for the Application class
  Application::~Application()
  {
  }

  void Application::RenderUI()
  {
    bool *sqw = &show_sequential_window;
    bool *spw = &show_parallel_window;
    WrapInDockSpace([&sqw, &spw]()
                    {
                      const char *sequential_window_id = "Machine Learning in Sequential";
                      const char *parallel_window_id = "Machine Learning in Parallel";

                      ImGui::Begin("DSPC Assignment - Machine Learning");
                      // ImGui::ShowDemoWindow();
                      ImGui::Text("Hello, World");

                      if (!*sqw)
                        *sqw = ImGui::Button("Run Sequential");
                      else
                      {
                        ImGui::Button("Run Sequential");
                        ImGui::Begin(sequential_window_id, sqw);
                        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Not implemented yet");
                        ImGui::End();
                      }

                      if (!*spw)
                        *spw = ImGui::Button("Run Parallel");
                      else
                      {
                        ImGui::Button("Run Parallel");
                        ImGui::Begin(parallel_window_id, spw);
                        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Not implemented yet");
                        ImGui::End();
                      }

                      // if (ImGui::BeginPopupModal(sequential_window_id, NULL, ImGuiWindowFlags_AlwaysAutoResize))
                      // {
                      //   ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Not implemented yet");

                      //   if (ImGui::Button("Close"))
                      //     ImGui::CloseCurrentPopup();

                      //   ImGui::EndPopup();
                      // }

                      ImGui::End();
                    });
  }

  void Application::Run()
  {
    RenderUI();
  }

  void WrapInDockSpace(std::function<void(void)> render_func)
  {
    static bool opt_fullscreen = true;
    static bool opt_padding = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
      const ImGuiViewport *viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->WorkPos);
      ImGui::SetNextWindowSize(viewport->WorkSize);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
      dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
      window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", NULL, window_flags);
    if (!opt_padding)
      ImGui::PopStyleVar();

    if (opt_fullscreen)
      ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
      ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    // Render the UI here
    render_func();

    ImGui::End();
  }
} // namespace DSPC
