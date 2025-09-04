#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 m_proj;
uniform mat4 m_view;

void main() {
    gl_Position = m_proj * m_view * vec4(vertex.xy, -1.0, 1.0);
    TexCoords = vertex.zw;
}