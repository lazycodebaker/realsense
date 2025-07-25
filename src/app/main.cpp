// main.cpp

#include <iostream>

// The Golden Rule: GLAD first, then GLFW.
#include <glad/gl.h>    // <--- MUST BE FIRST
#include <GLFW/glfw3.h> // <--- MUST BE SECOND

// --- ImGui Headers ---
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// A simple error callback for GLFW
static void glfw_error_callback(int error, const char *description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main()
{
    // --- 1. Setup GLFW and create a window ---
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        return 1;
    }

    // Set OpenGL version for macOS
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor(); 

    // Create the window
    GLFWwindow *window = glfwCreateWindow(1280, 720, "Simple ImGui Example", monitor, nullptr);
    if (window == nullptr)
    {
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval (1); // Enable VSync

    // --- 2. Initialize GLAD ---
    // --- 2. Initialize GLAD ---
    // This version is slightly more modern but functionally identical for this use case.
    // if (!gladLoadGLLoader((GLADloadfunc)glfwGetProcAddress))
    // {
    //     std::cerr << "Failed to initialize GLAD" << std::endl;
    //     return -1;
    // }

    // --- 3. Setup Dear ImGui ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Optional: Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Optional: Enable Multi-Viewport

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // ====================================================================
    // --- Our Application State ---
    // These are the variables that our UI will interact with.
    // ====================================================================
    float clear_color[4] = {0.45f, 0.55f, 0.60f, 1.00f};
    int button_press_count = 0;
    bool show_another_window = false;

    // --- 4. Main Application Loop ---
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start a new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // --- Create a Simple UI Window ---
        // This begins the definition of a new window.
        ImGui::Begin("Hello, ImGui!");

        // Display some text
        ImGui::Text("This is a simple window to control the app.");
        ImGui::Separator(); // Draw a horizontal line

        // Slider to control the background color.
        // The label "Background Color" is displayed, and the float array `clear_color` is modified.
        ImGui::ColorEdit3("Background Color", clear_color);

        // A button that increments a counter.
        // The 'if' block is executed only when the button is clicked.
        if (ImGui::Button("Click Me!"))
        {
            button_press_count++;
        }

        // Display the counter value. We use SameLine() to put it next to the button.
        ImGui::SameLine();
        ImGui::Text("Button clicks: %d", button_press_count);

        // A checkbox to control the visibility of another window
        ImGui::Checkbox("Show Another Window", &show_another_window);

        // Display a live Frames-Per-Second counter
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

        // This ends the window definition.
        ImGui::End();

        // If the checkbox is ticked, show a second simple window
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing 'x')
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
            {
                show_another_window = false;
            }
            ImGui::End();
        }

        // --- 5. Rendering ---
        // Prepare the framebuffer for rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render the ImGui draw data
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // --- 6. Cleanup ---
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}