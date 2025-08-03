#include "cvt/ui/UIManager.h" // Include the corresponding header
#include <iostream>

// CONSTRUCTOR DEFINITION
UIManager::UIManager() {
    // Code to initialize the UI Manager goes here.
    // For example, setting up ImGui context.
    std::cout << "UIManager created!" << std::endl;
}

// DESTRUCTOR DEFINITION
UIManager::~UIManager() {
    // Code to clean up resources goes here.
    // For example, shutting down ImGui.
    std::cout << "UIManager destroyed!" << std::endl;
}
