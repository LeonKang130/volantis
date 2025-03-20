#version 460

layout(location = 0) in vec3 vertex_position;

out vec4 vertex_color;

void main() {
    gl_Position = vec4(vertex_position, 1.0);
    vertex_color = vec4(vertex_position.xy, 1.0, 1.0);
}
