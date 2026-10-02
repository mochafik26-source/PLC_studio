#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

int main()
{
    // Initialize GLFW
    if (!glfwInit())
        return 1;

    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(
        1280,
        720,
        "PLC GUI",
        nullptr,
        nullptr
    );

    if (!window)
    {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // -------------------------
    // Initialize Dear ImGui
    // -------------------------

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Dark theme
    ImGui::StyleColorsDark();

    // Initialize ImGui backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // -------------------------
    // Main loop
    // -------------------------

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // -------------------------
        // GUI
        // -------------------------
        ImGui::SetNextWindowSize(ImVec2(300, 1080));
        ImGui::SetNextWindowPos(ImVec2(10, 10));
        ImGui::Begin("PLC GUI", nullptr, ImGuiWindowFlags_NoResize);

        ImGui::Text("Hello from Dear ImGui!");

        ImGui::Separator();

        // Table settings
        ImGuiTableFlags flags =
            ImGuiTableFlags_Borders |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_Resizable;

        ImGui::Text("PLC Control");

if (ImGui::Button("RUN", ImVec2(250, 40)))
{
    // Start PLC
}


if (ImGui::Button("STOP", ImVec2(250, 40)))
{
    // Stop PLC
} 
        // End window
        ImGui::End();

        // -------------------------
        // Render
        // -------------------------

        ImGui::Render();

        int width = 1280;
        int height = 720;

        glfwGetFramebufferSize(
            window,
            &width,
            &height
        );

        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(
            ImGui::GetDrawData()
        );

        glfwSwapBuffers(window);
    }

    // -------------------------
    // Cleanup
    // -------------------------

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
