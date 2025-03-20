//
// Created by Leon Kang on 2025/3/20.
//

#include <spdlog/spdlog.h>
#include "pipeline.h"

namespace volantis {
    Pipeline::Pipeline(const PipelineType type, std::vector<const Shader*> shaders) {
        _type = type;
        if (_type == Rasterization) {
            const auto vertexShader = std::ranges::find_if(shaders, [](const Shader* shader) {
                return shader->GetType() == Shader::ShaderType::Vertex;
            });
            if (vertexShader == shaders.end()) {
                throw std::runtime_error("Vertex shader not found");
            }
            const auto fragmentShader = std::ranges::find_if(shaders, [](const Shader* shader) {
                return shader->GetType() == Shader::ShaderType::Fragment;
            });
            if (fragmentShader == shaders.end()) {
                throw std::runtime_error("Fragment shader not found");
            }
            _handle = LinkRasterizationProgram(**vertexShader, **fragmentShader);
        } else if (_type == Compute) {
            const auto computeShader = std::ranges::find_if(shaders, [](const Shader* shader) {
                return shader->GetType() == Shader::ShaderType::Compute;
            });
            if (computeShader == shaders.end()) {
                throw std::runtime_error("Compute shader not found");
            }
            _handle = LinkComputeProgram(**computeShader);
        } else {
            throw std::runtime_error("Unknown pipeline type");
        }
    }

    Pipeline::~Pipeline() {
        glDeleteProgram(_handle);
    }

    auto Pipeline::LoadRasterizationProgram(const std::string_view vertexShaderPath, const std::string_view fragmentShaderPath) -> std::unique_ptr<Pipeline> {
        const auto vertexShader = Shader::Load(Shader::Vertex, vertexShaderPath);
        const auto fragmentShader = Shader::Load(Shader::Fragment, fragmentShaderPath);
        std::vector<const Shader*> shaders = {vertexShader.get(), fragmentShader.get()};
        return std::make_unique<Pipeline>(Rasterization, shaders);
    }

    auto Pipeline::LoadComputeProgram(const std::string_view computeShaderPath) -> std::unique_ptr<Pipeline> {
        const auto computeShader = Shader::Load(Shader::Compute, computeShaderPath);
        std::vector<const Shader*> shaders = {computeShader.get()};
        return std::make_unique<Pipeline>(Compute, shaders);
    }

    auto Pipeline::LinkRasterizationProgram(const Shader &vertexShader, const Shader &fragmentShader) -> GLuint {
        const auto program = glCreateProgram();
        glAttachShader(program, vertexShader.GetHandle());
        glAttachShader(program, fragmentShader.GetHandle());
        glLinkProgram(program);
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
            spdlog::error("Program linking failed: {}", infoLog);
            throw std::runtime_error("Program linking failed");
        }
        glDetachShader(program, vertexShader.GetHandle());
        glDetachShader(program, fragmentShader.GetHandle());
        spdlog::debug("Rasterization program linked successfully");
        return program;
    }

    auto Pipeline::LinkComputeProgram(const Shader &computeShader) -> GLuint {
        const auto program = glCreateProgram();
        glAttachShader(program, computeShader.GetHandle());
        glLinkProgram(program);
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
            spdlog::error("Compute program linking failed: {}", infoLog);
            throw std::runtime_error("Compute program linking failed");
        }
        glDetachShader(program, computeShader.GetHandle());
        spdlog::debug("Compute program linked successfully");
        return program;
    }
} // namespace volantis