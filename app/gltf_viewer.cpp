//
// Created by Leon Kang on 2025/3/19.
//

#include <cxxopts.hpp>
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

auto main(const int argc, char** argv) -> int {
    cxxopts::Options options("miniature", "Experimental appearance aggregation algorithm");
    options.add_options()
        ("d,debug", "Enable debugging messages", cxxopts::value<bool>()->default_value("false"))
        ("w,width", "Window width", cxxopts::value<int>()->default_value("1920"))
        ("h,height", "Window height", cxxopts::value<int>()->default_value("1080"));
    const auto result = options.parse(argc, argv);
    const auto width = result["width"].as<int>();
    const auto height = result["height"].as<int>();
    if (result["debug"].as<bool>()) {
        spdlog::set_level(spdlog::level::debug);
        spdlog::debug("Debugging messages enabled");
    }
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    const auto window = glfwCreateWindow(width, height, "Appearance Aggregation Experiment", nullptr, nullptr);
    if (!window) {
        spdlog::critical("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {

    });
    if (!gladLoadGL()) {
        spdlog::critical("Failed to initialize GLAD");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}