//
// Created by Leon Kang on 2025/3/20.
//

#ifndef PIPELINE_H
#define PIPELINE_H

#include "shader.h"

namespace volantis {
    class Pipeline {
    public:
        enum PipelineType {
            Rasterization,
            Compute,
        };
        Pipeline(PipelineType type, std::vector<const Shader*> shaders);
        ~Pipeline();
        [[nodiscard]] static auto LoadRasterizationProgram(std::string_view vertexShaderPath, std::string_view fragmentShaderPath) -> std::unique_ptr<Pipeline>;
        [[nodiscard]] static auto LoadComputeProgram(std::string_view computeShaderPath) -> std::unique_ptr<Pipeline>;
        [[nodiscard]] auto GetType() const noexcept -> PipelineType { return _type; }
        [[nodiscard]] auto GetHandle() const noexcept -> GLuint { return _handle; }
    private:
        PipelineType _type;
        GLuint _handle;
        [[nodiscard]] static auto LinkRasterizationProgram(const Shader& vertexShader, const Shader& fragmentShader) -> GLuint;
        [[nodiscard]] static auto LinkComputeProgram(const Shader& computeShader) -> GLuint;
    };
} // namespace volantis

#endif //PIPELINE_H
