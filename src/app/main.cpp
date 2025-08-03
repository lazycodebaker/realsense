// main.cpp

#include <iostream>
#include <vector>
#include <string>
#include <memory>

// --- Core Graphics Libraries (Order is important!) ---
#include <glad/gl.h>
#include <GLFW/glfw3.h>

// --- ImGui Headers ---
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// --- OpenCV Headers ---
#include <opencv2/opencv.hpp>

#include "Application.h"
#include "cvt/core/CameraManager.h"
#include "cvt/ui/TextureManager.h"
#include "cvt/processing/Processors.h"

// Note: The custom Application and TextureManager includes have been removed
// as the logic is self-contained in main for this example.

// Helper to update an OpenGL Texture with a CV Mat

// struct ProcessedFeed
// {
//     std::string name;
//     cv::Mat mat;
//     GLuint texture_id = 0;
//     bool is_visible = false;

//     ProcessedFeed(std::string name, bool visible = false) : name(std::move(name)), is_visible(visible) {}

//     void initialize_texture(TextureManager &texture_manager)
//     {
//         if (texture_id == 0)
//         {
//             texture_id = texture_manager.createTexture();
//         }
//     }

//     void release()
//     {
//         if (texture_id != 0)
//         {
//             glDeleteTextures(1, &texture_id);
//             texture_id = 0;
//         }
//     }
// };

int main()
{

    TextureManager* texture_manager = new TextureManager();
    Application* application = new Application();

    AppState* appState = new AppState();

    const char* glsl_version = application->getGSLVersion();

    GLFWwindow* window = application->createWindow(1920, 1080, "Professional Computer Vision Toolkit", nullptr, nullptr);
    if (window == nullptr)
        return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable VSync

    // --- 3. Setup Dear ImGui ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // #if defined(IMGUI_HAS_VIEWPORT) || (IMGUI_ENABLE_VIEWPORTS && !defined(IMGUI_DISABLE_VIEWPORTS))
    //     io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //     io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    // #endif

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags) //  & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // --- 4. Setup OpenCV & Application State ---
    // --- NEW: Camera selection state ---
    static int selected_camera_index = 0;
    int active_camera_index = -1;
    bool camera_is_functional = false;

    int frame_width = 0;
    int frame_height = 0;

    CameraManager* camera_manager = new CameraManager(frame_width, frame_height, selected_camera_index);
    cv::VideoCapture cap = camera_manager->getCapture();

    static int blur_kernel_size = 15;

    static float canny_threshold1 = 100.0f;
    static float canny_threshold2 = 200.0f;
    static int binary_thresh = 128;
    static int harris_block_size = 2;
    static int harris_ksize = 3;
    static float harris_k = 0.04f;
    static float hsv_hue[2] = { 10.f, 30.f };
    static float hsv_sat[2] = { 100.f, 255.f };
    static float hsv_val[2] = { 100.f, 255.f };

    // --- Create all our feed objects ---
    std::vector<std::unique_ptr<IProcessor>> feeds;
    // ... (feed creation is unchanged)
    feeds.emplace_back(std::make_unique<OriginalProcessor>(true));
    feeds.emplace_back(std::make_unique<GrayscaleProcessor>(true));
    feeds.emplace_back(std::make_unique<GaussianBlurProcessor>(true));
    feeds.emplace_back(std::make_unique<CannyProcessor>(true));
    feeds.emplace_back(std::make_unique<ContourProcessor>());
    feeds.emplace_back(std::make_unique<ColorMaskProcessor>());
    // feeds.emplace_back(std::make_unique<IProcessor>("HSV Colorspace"));
    // feeds.emplace_back(std::make_unique<IProcessor>("Binary Threshold"));
    // feeds.emplace_back(std::make_unique<IProcessor>("Otsu Threshold"));
    // feeds.emplace_back(std::make_unique<IProcessor>("Adaptive Threshold"));
    // feeds.emplace_back(std::make_unique<IProcessor>("Morph Dilation"));
    // feeds.emplace_back(std::make_unique<IProcessor>("Morph Erosion"));
    // feeds.emplace_back(std::make_unique<IProcessor>("Sobel Edges"));
    // feeds.emplace_back(std::make_unique<IProcessor>("Laplacian"));
    // feeds.emplace_back(std::make_unique<IProcessor>("Histogram Equalized"));
    // feeds.emplace_back(std::make_unique<IProcessor>("Harris Corners"));

    for (auto& feed : feeds)
    {
        feed->initialize_texture(*texture_manager);
    }

    cv::Mat frame, rgb_frame, gray_frame;
    bool is_first_frame = true;

    // --- 5. Main Application Loop ---
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // --- NEW: Camera Switching Logic ---
        if (selected_camera_index != active_camera_index)
        {
            if (camera_manager->isOpened())
            {
                camera_manager->capRelease();
            }
            camera_manager->open(selected_camera_index);
            cap = camera_manager->getCapture();

            if (camera_manager->isOpened())
            {
                camera_is_functional = true;
                frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
                frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
                is_first_frame = true;
                std::cout << "Successfully opened camera " << selected_camera_index
                    << " with resolution " << frame_width << "x" << frame_height << std::endl;
            }
            else
            {
                camera_is_functional = false;
                std::cerr << "Error: Could not open camera " << selected_camera_index << std::endl;
            }
            active_camera_index = selected_camera_index;
        }

        // --- Frame Capture & Core Processing ---
        if (camera_is_functional)
        {
            camera_manager->readFrame(frame);
            if (!frame.empty())
            {
                // This block is the same as before...
                cv::cvtColor(frame, rgb_frame, cv::COLOR_BGR2RGB);
                cv::cvtColor(rgb_frame, gray_frame, cv::COLOR_RGB2GRAY);
                cv::cvtColor(gray_frame, gray_frame, cv::COLOR_GRAY2RGB);

                appState->blur_kernel_size = blur_kernel_size;
                appState->canny_threshold1 = canny_threshold1;
                appState->canny_threshold2 = canny_threshold2;
                appState->hsv_hue[0] = hsv_hue[0];
                appState->hsv_hue[1] = hsv_hue[1];
                appState->hsv_sat[0] = hsv_sat[0];
                appState->hsv_sat[1] = hsv_sat[1];
                appState->hsv_val[0] = hsv_val[0];
                appState->hsv_val[1] = hsv_val[1];

                for (auto& feed : feeds)
                {
                    if (!feed->is_visible)
                        continue;
                    // ... (All the `if (name == ...)` processing logic is unchanged)
                    const std::string& name = feed->name;
                    cv::Mat& target_mat = feed->mat;

                    // --- Perform requested CV operation ---
                    if (name == "Original")
                        target_mat = rgb_frame;
                    else if (name == "Grayscale")
                        target_mat = gray_frame;
                    else if (name == "Gaussian Blur")
                    {
                        int k_size = (blur_kernel_size / 2) * 2 + 1;
                        cv::GaussianBlur(rgb_frame, target_mat, cv::Size(k_size, k_size), 0);
                    }
                    else if (name == "Canny Edges")
                        cv::Canny(gray_frame, target_mat, canny_threshold1, canny_threshold2);
                    // else if (name == "HSV Colorspace")
                    //     cv::cvtColor(rgb_frame, target_mat, cv::COLOR_RGB2HSV);
                    // else if (name == "Binary Threshold")
                    //     cv::threshold(gray_frame, target_mat, binary_thresh, 255, cv::THRESH_BINARY);
                    // else if (name == "Otsu Threshold")
                    //     cv::threshold(gray_frame, target_mat, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
                    // else if (name == "Adaptive Threshold")
                    //     cv::adaptiveThreshold(gray_frame, target_mat, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 2);
                    // else if (name == "Morph Dilation")
                    //     cv::dilate(gray_frame, target_mat, cv::Mat());
                    // else if (name == "Morph Erosion")
                    //     cv::erode(gray_frame, target_mat, cv::Mat());
                    // else if (name == "Sobel Edges")
                    // {
                    //     cv::Mat sobel_x, sobel_y, abs_sobel_x, abs_sobel_y;
                    //     cv::Sobel(gray_frame, sobel_x, CV_16S, 1, 0);
                    //     cv::Sobel(gray_frame, sobel_y, CV_16S, 0, 1);
                    //     convertScaleAbs(sobel_x, abs_sobel_x);
                    //     convertScaleAbs(sobel_y, abs_sobel_y);
                    //     addWeighted(abs_sobel_x, 0.5, abs_sobel_y, 0.5, 0, target_mat);
                    // }
                    // else if (name == "Laplacian")
                    // {
                    //     cv::Mat lap;
                    //     cv::Laplacian(gray_frame, lap, CV_16S, 3);
                    //     cv::convertScaleAbs(lap, target_mat);
                    // }
                    // else if (name == "Histogram Equalized")
                    //     cv::equalizeHist(gray_frame, target_mat);
                    // else if (name == "Harris Corners")
                    // {
                    //     cv::Mat harris_dst, harris_norm;
                    //     int k_size = (harris_ksize / 2) * 2 + 1; // Must be odd
                    //     cornerHarris(gray_frame, harris_dst, harris_block_size, k_size, harris_k);
                    //     normalize(harris_dst, harris_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());
                    //     target_mat = rgb_frame.clone();
                    //     for (int i = 0; i < harris_norm.rows; i++)
                    //     {
                    //         for (int j = 0; j < harris_norm.cols; j++)
                    //         {
                    //             if ((int)harris_norm.at<float>(i, j) > 150)
                    //             {
                    //                 cv::circle(target_mat, cv::Point(j, i), 5, cv::Scalar(0, 255, 0), 2, 8, 0);
                    //             }
                    //         }
                    //     }
                    // }
                    else if (name == "Color Mask")
                    {
                        cv::Mat hsv;
                        cv::cvtColor(rgb_frame, hsv, cv::COLOR_RGB2HSV);
                        cv::inRange(hsv, cv::Scalar(hsv_hue[0], hsv_sat[0], hsv_val[0]), cv::Scalar(hsv_hue[1], hsv_sat[1], hsv_val[1]), target_mat);
                    }
                    else if (name == "Contours")
                    {
                        cv::Mat canny_output;
                        cv::Canny(gray_frame, canny_output, 100, 200);
                        std::vector<std::vector<cv::Point>> contours;
                        findContours(canny_output, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
                        target_mat = cv::Mat::zeros(canny_output.size(), CV_8UC3);
                        for (size_t i = 0; i < contours.size(); i++)
                        {
                            cv::Scalar color = cv::Scalar(rand() % 256, rand() % 256, rand() % 256);
                            drawContours(target_mat, contours, (int)i, color, 2);
                        }
                    }

                    if (!target_mat.empty())
                    {
                        texture_manager->update_gl_texture(feed->texture_id, target_mat, is_first_frame);
                    }
                }
                if (is_first_frame)
                    is_first_frame = false;
            }
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // #if defined(IMGUI_HAS_VIEWPORT) || (IMGUI_ENABLE_VIEWPORTS && !defined(IMGUI_DISABLE_VIEWPORTS))
        //         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        // #endif
        // --- Build ImGui UI ---
        ImGui::Begin("Controls");
        // --- NEW: Camera selection UI ---
        ImGui::Text("Camera Source");
        ImGui::Separator();
        ImGui::RadioButton("Camera 0", &selected_camera_index, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Camera 1", &selected_camera_index, 1);
        ImGui::Separator();

        ImGui::Text("Visibility");
        ImGui::Separator();
        for (const auto& feed : feeds)
        {
            ImGui::Checkbox(feed->name.c_str(), &feed->is_visible);
        }
        ImGui::Separator();
        ImGui::Text("Parameters");
        ImGui::Separator();
        // ... (rest of UI controls are unchanged)
        if (ImGui::CollapsingHeader("Blur"))
        {
            ImGui::SliderInt("Blur Kernel", &blur_kernel_size, 1, 51);
        }
        if (ImGui::CollapsingHeader("Canny"))
        {
            ImGui::SliderFloat("Threshold 1", &canny_threshold1, 0.0f, 500.0f);
            ImGui::SliderFloat("Threshold 2", &canny_threshold2, 0.0f, 500.0f);
        }
        if (ImGui::CollapsingHeader("Thresholding"))
        {
            ImGui::SliderInt("Binary Threshold", &binary_thresh, 0, 255);
        }
        if (ImGui::CollapsingHeader("Harris Corner"))
        {
            ImGui::SliderInt("Block Size", &harris_block_size, 1, 20);
            ImGui::SliderInt("Kernel Size", &harris_ksize, 1, 31);
            ImGui::SliderFloat("K Value", &harris_k, 0.01f, 0.20f);
        }
        if (ImGui::CollapsingHeader("HSV Color Mask"))
        {
            ImGui::Text("Hue");
            ImGui::DragFloatRange2("##hue", &hsv_hue[0], &hsv_hue[1], 1.0f, 0.0f, 180.0f);
            ImGui::Text("Saturation");
            ImGui::DragFloatRange2("##sat", &hsv_sat[0], &hsv_sat[1], 1.0f, 0.0f, 255.0f);
            ImGui::Text("Value");
            ImGui::DragFloatRange2("##val", &hsv_val[0], &hsv_val[1], 1.0f, 0.0f, 255.0f);
        }
        ImGui::Separator();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        ImGui::Begin("Live Feeds");
        if (camera_is_functional && frame_width > 0)
        {
            float aspect_ratio = (float)frame_width / (float)frame_height;
            float window_width = ImGui::GetContentRegionAvail().x;
            int columns = std::max(1, static_cast<int>(window_width / 350.f));
            ImGui::Columns(columns, nullptr, false);
            for (const auto& feed : feeds)
            {
                if (feed->is_visible)
                {
                    ImGui::Text("%s", feed->name.c_str());
                    float col_width = ImGui::GetColumnWidth();
                    ImVec2 image_size(col_width - 10, (col_width - 10) / aspect_ratio); // A little padding
                    ImGui::Image((void*)(intptr_t)feed->texture_id, image_size);
                    ImGui::NextColumn();
                }
            }
            ImGui::Columns(1);
        }
        else
        {
            ImGui::Text("Camera %d not available or failed to open.", active_camera_index);
            ImGui::Text("Please check the camera connection and select a valid source.");
        }
        ImGui::End();

        // --- 6. Rendering ---
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags) // & ImGuiConfigFlags_ViewportsEnable)
        {
            // GLFWwindow *backup_current_context = glfwGetCurrentContext();
            // #if defined(IMGUI_HAS_VIEWPORT) || (IMGUI_ENABLE_VIEWPORTS && !defined(IMGUI_DISABLE_VIEWPORTS))
            //             ImGui::UpdatePlatformWindows();
            //             ImGui::RenderPlatformWindowsDefault();
            //             glfwMakeContextCurrent(backup_current_context);
            // #endif
        }

        glfwSwapBuffers(window);
    }

    // --- 7. Cleanup ---
    cap.release();
    for (auto& feed : feeds)
    {
        feed->release();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

/*

cv::Mat canny_output;
cv::Canny(gray_frame, canny_output, 100, 200);
std::vector<std::vector<cv::Point>> contours;
findContours(canny_output, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
target_mat = cv::Mat::zeros(canny_output.size(), CV_8UC3);
for (size_t i = 0; i < contours.size(); i++)
{
    cv::Scalar color = cv::Scalar(rand() % 256, rand() % 256, rand() % 256);
    drawContours(target_mat, contours, (int)i, color, 2);
}

*/
