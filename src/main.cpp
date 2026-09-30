#include <cstdio>

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "core/AppState.h"
#include "ui/BootScreen.h"
#include "ui/Desktop.h"
#include "ui/DesktopIcons.h"
#include "ui/Taskbar.h"
#include "ui/MockApps.h"

// Swallow OS close requests so shutdown only happens via is_running.
static void OnWindowClose(GLFWwindow *window)
{
    glfwSetWindowShouldClose(window, GLFW_FALSE);
}

int main()
{
    // Init the windowing library, bail if unavailable.
    if (!glfwInit())
    {
        std::fprintf(stderr, "Failed to init GLFW\n");
        return -1;
    }

    // Request an OpenGL 3.0 context before creating the window.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Open the emulator window at a fixed 1280x720 size and title.
    GLFWwindow *window = glfwCreateWindow(1280, 720, "CSOPESY Emulator", nullptr, nullptr);
    if (window == nullptr)
    {
        std::fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowCloseCallback(window, OnWindowClose);

    // Create the ImGui context with keyboard navigation and docking.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;

    // Bind ImGui to the GLFW window and the OpenGL3 renderer.
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Run until shutdown is requested; draws the base layer first.
    while (AppState::is_running && !glfwWindowShouldClose(window))
    {
        // Pump events and set up the per-frame ImGui state.
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Boot splash covers everything first: fade in, hold ~3s, fade out.
        // While active, skip the desktop UI so it can't show through.
        if (!g_boot_screen.Render())
        {
            // Draw the layers back-to-front: desktop, icons, apps, taskbar on top.
            g_desktop.Render();
            g_desktop_icons.Render();
            g_mock_apps.Render();
            g_taskbar.Render();
        }

        // Record draw data, fit the viewport, and present the frame.
        ImGui::Render();
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Shut down renderer, platform, context, and window in order.
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}