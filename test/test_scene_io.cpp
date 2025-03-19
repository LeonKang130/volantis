//
// Created by Leon Kang on 2025/3/19.
//

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>
#include <string>

TEST(SceneLoader, LoadSceneFromBinary) {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    const std::string filename = "asset/WaterBottle.glb";
    const auto ret = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
    if (!warn.empty()) {
        spdlog::warn("Warning: {}", warn);
    }
    if (!err.empty()) {
        spdlog::error("Error: {}", err);
    }
    EXPECT_TRUE(ret) << "Failed to load scene from file: " << filename;
}

auto main(int argc, char** argv) -> int {
    ::testing::InitGoogleTest(&argc, argv);
    spdlog::set_level(spdlog::level::debug);
    return RUN_ALL_TESTS();
}
