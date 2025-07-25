// main.cpp

#include <iostream>
#include <vector>

// --- Core Graphics Libraries (Order is important!) ---
#include <glad/gl.h>
#include <GLFW/glfw3.h>

// --- ImGui Headers ---
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// --- OpenCV Headers ---
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

// A simple error callback for GLFW
static void glfw_error_callback(int error, const char *description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

// Helper function to create an OpenGL texture
GLuint create_texture()
{
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Use GL_CLAMP_TO_EDGE to avoid artifacts at texture borders
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return texture_id;
}

int main()
{
    // --- 1. Setup GLFW and create a window ---
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "OpenCV + ImGui Advanced Example", nullptr, nullptr);
    if (window == nullptr)
        return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable VSync

    // --- 2. Initialize GLAD (Fixed!) ---
    // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    // {
    //     std::cerr << "Failed to initialize GLAD" << std::endl;
    //     return -1;
    // }

    // --- 3. Setup Dear ImGui ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // ====================================================================
    // --- NEW: Setup OpenCV ---
    // ====================================================================
    cv::VideoCapture cap(0); // Open the default camera
    if (!cap.isOpened())
    {
        std::cerr << "Error: Could not open camera!" << std::endl;
        return -1;
    }

    // --- State variables for our UI ---
    bool show_grayscale_window = false;
    bool show_blur_window = false;
    bool show_canny_window = false;

    // --- OpenCV Mats for various image processing steps ---
    cv::Mat frame;
    cv::Mat gray_frame;
    cv::Mat blurred_frame;
    cv::Mat canny_frame;
    // Store frame dimensions for robust UI rendering
    int frame_width = 0;
    int frame_height = 0;

    // --- Create OpenGL textures for our video frames ---
    GLuint color_texture = create_texture();
    GLuint gray_texture = create_texture();
    GLuint blur_texture = create_texture();
    GLuint canny_texture = create_texture();

    bool is_first_frame = true;

    // --- 4. Main Application Loop ---
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // --- Capture and process a frame from the camera ---
        cap.read(frame);
        if (!frame.empty())
        {
            if (is_first_frame)
            {
                frame_width = frame.cols;
                frame_height = frame.rows;
            }

            // IMPORTANT: OpenCV captures in BGR format, but OpenGL expects RGB.
            cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

            // --- Perform all image processing steps ---
            cv::cvtColor(frame, gray_frame, cv::COLOR_RGB2GRAY);
            cv::GaussianBlur(frame, blurred_frame, cv::Size(15, 15), 0);
            // Canny edge detection works on a single-channel image
            cv::Canny(gray_frame, canny_frame, 100, 200);

            // --- Upload frame data to OpenGL textures ---
            if (is_first_frame)
            {
                // For the first frame, use glTexImage2D to allocate memory
                // Color
                glBindTexture(GL_TEXTURE_2D, color_texture);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_width, frame_height, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);
                // Grayscale (single channel, so we use GL_RED)
                glBindTexture(GL_TEXTURE_2D, gray_texture);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frame_width, frame_height, 0, GL_RED, GL_UNSIGNED_BYTE, gray_frame.data);
                // Blur
                glBindTexture(GL_TEXTURE_2D, blur_texture);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_width, frame_height, 0, GL_RGB, GL_UNSIGNED_BYTE, blurred_frame.data);
                // Canny (single channel, so we use GL_RED)
                glBindTexture(GL_TEXTURE_2D, canny_texture);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frame_width, frame_height, 0, GL_RED, GL_UNSIGNED_BYTE, canny_frame.data);

                is_first_frame = false;
            }
            else
            {
                // For subsequent frames, use glTexSubImage2D for better performance
                glBindTexture(GL_TEXTURE_2D, color_texture);
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame_width, frame_height, GL_RGB, GL_UNSIGNED_BYTE, frame.data);

                glBindTexture(GL_TEXTURE_2D, gray_texture);
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame_width, frame_height, GL_RED, GL_UNSIGNED_BYTE, gray_frame.data);

                glBindTexture(GL_TEXTURE_2D, blur_texture);
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame_width, frame_height, GL_RGB, GL_UNSIGNED_BYTE, blurred_frame.data);

                glBindTexture(GL_TEXTURE_2D, canny_texture);
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame_width, frame_height, GL_RED, GL_UNSIGNED_BYTE, canny_frame.data);
            }
        }

        // Start a new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // --- Create our ImGui Windows ---

        // 1. The Control Sidebar
        {
            ImGui::Begin("Controls");
            ImGui::Text("Toggle filtered views:");
            ImGui::Checkbox("Grayscale", &show_grayscale_window);
            ImGui::Checkbox("Blur", &show_blur_window);
            ImGui::Checkbox("Canny Edge", &show_canny_window);

            ImGui::Separator();
            ImGui::Text("Original Webcam Feed:");
            if (frame_width > 0)
            {
                // Display the main color texture in the sidebar, scaled down
                ImVec2 sidebar_img_size = ImVec2(frame_width / 3.0f, frame_height / 3.0f);
                ImGui::Image((void *)(intptr_t)color_texture, sidebar_img_size);
            }
            ImGui::End();
        }

        // 2. Conditionally render the filter windows
        if (show_grayscale_window)
        {
            // Pass the boolean by pointer, so the 'x' button on the window can turn it off
            ImGui::Begin("Grayscale Feed", &show_grayscale_window);
            if (frame_width > 0)
                ImGui::Image((void *)(intptr_t)gray_texture, ImVec2(frame_width, frame_height));
            ImGui::End();
        }

        if (show_blur_window)
        {
            ImGui::Begin("Blur Feed", &show_blur_window);
            if (frame_width > 0)
                ImGui::Image((void *)(intptr_t)blur_texture, ImVec2(frame_width, frame_height));
            ImGui::End();
        }

        if (show_canny_window)
        {
            ImGui::Begin("Canny Edge Feed", &show_canny_window);
            if (frame_width > 0)
                ImGui::Image((void *)(intptr_t)canny_texture, ImVec2(frame_width, frame_height));
            ImGui::End();
        }

        // --- 5. Rendering ---
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render the ImGui draw data
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // --- 6. Cleanup ---
    // Release OpenCV and OpenGL resources
    cap.release();
    glDeleteTextures(1, &color_texture);
    glDeleteTextures(1, &gray_texture);
    glDeleteTextures(1, &blur_texture);
    glDeleteTextures(1, &canny_texture);

    // ImGui and GLFW cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}