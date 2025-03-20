//
// Created by Leon Kang on 2025/3/20.
//

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <filesystem>

namespace volantis {
    class Shader {
    public:
        enum ShaderType {
            Vertex,
            Fragment,
            Compute,
        };
        Shader(ShaderType type, std::string&& source);
        ~Shader();
        [[nodiscard]] static auto Load(ShaderType type, std::string_view path) -> std::unique_ptr<Shader>;
        [[nodiscard]] auto GetType() const noexcept -> ShaderType { return _type; }
        [[nodiscard]] auto GetSource() const noexcept -> std::string_view { return _source; }
        [[nodiscard]] auto GetHandle() const noexcept -> GLuint { return _handle; }
    private:
        ShaderType _type;
        std::string _source;
        GLuint _handle;
        [[nodiscard]] static auto LoadShaderSource(const std::filesystem::path& path) -> std::string;
        [[nodiscard]] static auto CompileShader(ShaderType type, std::string_view source) -> GLuint;
    };
} // namespace volantis

#endif //SHADER_H
