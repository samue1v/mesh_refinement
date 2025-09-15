#version 330 core

layout (location = 0) in vec2 aPos;

uniform mat4 m_view;
uniform mat4 m_proj;

void main(void)
{
    gl_Position = m_proj*m_view*vec4(aPos , -1.0, 1.0);
}
