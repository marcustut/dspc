#include "gui.h"

#include <stdexcept>

namespace DSPC::UI
{
  std::tuple<GLFWwindow *, const char *> SetupWindow(int width, int height, const char *title)
  {
    auto glfw_error_callback = [](int error, const char *description)
    {
      fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    };

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
      throw std::runtime_error(std::string("[ERROR] unable to init GLFW"));

      // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char *glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow *window = glfwCreateWindow(1280, 720, "DSPC Assignment GUI", NULL, NULL);
    if (window == NULL)
      throw std::runtime_error(std::string("[ERROR] unable to create window"));
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    return std::make_tuple(window, glsl_version);
  }

  void SetupImGui(GLFWwindow *window, const char *glsl_version)
  {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      style.WindowRounding = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    // IM_ASSERT(font != NULL);
  }

  void MainLoop(GLFWwindow *window, std::function<void(void)> render_func)
  {
    ImGuiIO &io = ImGui::GetIO();

    // state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
      // Poll and handle events (inputs, window resize, etc.)
      // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
      // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
      // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
      // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
      glfwPollEvents();

      // Start the Dear ImGui frame
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      // Render User Defined GUI
      render_func();

      // Rendering
      ImGui::Render();
      int display_w, display_h;
      glfwGetFramebufferSize(window, &display_w, &display_h);
      glViewport(0, 0, display_w, display_h);
      glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
      glClear(GL_COLOR_BUFFER_BIT);
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      // Update and Render additional Platform Windows
      // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
      //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
      if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
      {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
      }

      glfwSwapBuffers(window);
    }
  }

  void CleanUp(GLFWwindow *window)
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
  }

  void WrapInDockSpace(std::function<void(void)> render_func)
  {
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
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

  void RenderGUI(GUIState *state)
  {
    const char *sequential_window_id = "Linear Regression in Sequential";
    const char *parallel_window_id = "Linear Regression in Parallel";

    ImGui::Begin("DSPC Assignment - Linear Regression");
    // ImGui::ShowDemoWindow();
    ImGui::Text("Hello, World");

    if (!state->show_sequential_window)
      state->show_sequential_window = ImGui::Button("Run Sequential");
    else
    {
      // std::vector<std::string> players{"Cade Cunningham", "Anthony Edwards", "Zion Williamson", "DeAndre Ayton", "Lonzo Ball"};
      // std::vector<Coordinate> coordinates{
      //     (Coordinate){1, 10.0},
      //     (Coordinate){2, 10.2},
      //     (Coordinate){3, 10.7},
      //     (Coordinate){4, 12.6},
      //     (Coordinate){5, 18.6},
      // };
      // LinearRegression::LeastSquare lin_reg = LinearRegression::LeastSquare(Technique::Serial, coordinates);
      // lin_reg.InitModel();

      ImGui::Begin(sequential_window_id, &state->show_sequential_window);

      ImGui::Text("Below table shows the data fed to the Linear Regression model");

      static ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
      ImVec2 outer_size = ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 8);
      if (ImGui::BeginTable("table_scrolly", 3, flags, outer_size))
      {
        ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
        ImGui::TableSetupColumn("Player's Name", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Number of Years in the NBA", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Current Salary (USD, in millions)", ImGuiTableColumnFlags_None);
        ImGui::TableHeadersRow();

        // Demonstrate using clipper for large vertical lists
        // ImGuiListClipper clipper;
        // clipper.Begin(coordinates.size());
        // while (clipper.Step())
        // {
        //   for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
        //   {
        //     ImGui::TableNextRow();
        //     for (int column = 0; column < 3; column++)
        //     {
        //       ImGui::TableSetColumnIndex(column);
        //       if (column == 0)
        //         ImGui::Text("%s", players[row].c_str());
        //       else if (column == 1)
        //         ImGui::Text("%.0f", coordinates[row].x);
        //       else if (column == 2)
        //         ImGui::Text("%.1f", coordinates[row].y);
        //     }
        //   }
        // }
        ImGui::EndTable();
      }

      // ImGui::Text("Formula of the straight line: %s", lin_reg.Formula().c_str());
      // ImGui::Text("When X = 6, Y = %.1f", lin_reg.PredictY(6).y);
      // ImGui::Text("Hence, based on the data given, the predicted salary of an NBA player with six years of experience is %.2f million USD.", lin_reg.PredictY(6).y);

      ImGui::End();
    }

    if (!state->show_parallel_window)
      state->show_parallel_window = ImGui::Button("Run Parallel");
    else
    {
      ImGui::Begin(parallel_window_id, &state->show_parallel_window);
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
  }

}