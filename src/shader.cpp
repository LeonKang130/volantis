//
// Created by Leon Kang on 2025/3/19.
//

#include <spdlog/spdlog.h>
#include "shader.h"
#include <fstream>

namespace volantis {
    Shader::Shader(const ShaderType type, std::string&& source) {
        _type = type;
        _source = std::move(source);
        _handle = CompileShader(_type, _source);
    }

    Shader::~Shader() {
        glDeleteShader(_handle);
    }

    auto Shader::Load(const ShaderType type, std::string_view path) -> std::unique_ptr<Shader> {
        auto source = LoadShaderSource(path);
        return std::make_unique<Shader>(type, std::move(source));
    }

    auto Shader::LoadShaderSource(const std::filesystem::path& path) -> std::string {
        const auto dir = path.parent_path();
        std::ifstream file;
        file.open(path);
        if (!file.is_open()) {
            spdlog::error("Failed to open file {}", path.string());
            throw std::runtime_error("Shader file not found");
        }
        std::string source;
        std::string line;
        while (std::getline(file, line)) {
            if (line.starts_with("#include")) {
                spdlog::debug("#include directive found: {}", line);
                const auto start = line.find_first_of("<\"") + 1;
                const auto end = line.find_first_of(">\"", start);
                source += LoadShaderSource(dir / line.substr(start, end - start)) + "\n";
            } else {
                source += line + "\n";
            }
        }
        file.close();
        return source;
    }

    auto Shader::CompileShader(const ShaderType type, std::string_view source) -> GLuint {
        GLuint handle;
        if (type == Vertex) {
            handle = glCreateShader(GL_VERTEX_SHADER);
        } else if (type == Fragment) {
            handle = glCreateShader(GL_FRAGMENT_SHADER);
        } else if (type == Compute) {
            handle = glCreateShader(GL_COMPUTE_SHADER);
        } else {
            throw std::runtime_error("Unknown shader type");
        }
        const auto s = source.data();
        glShaderSource(handle, 1, &s, nullptr);
        glCompileShader(handle);
        GLint success;
        glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(handle, sizeof(infoLog), nullptr, infoLog);
            spdlog::error("Shader compilation failed: {}", infoLog);
            throw std::runtime_error("Shader compilation failed");
        }
        spdlog::debug("Shader compiled successfully");
        return handle;
    }
} // namespace volantis