#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 m_view;
uniform mat4 m_proj;

void main()
{
    gl_Position  = m_proj*m_view*vec4(position.x,position.y,-1.00000001, 1.0);
    //gl_Position  = m_view*vec4(position.x,position.y,-1.0, 1.0);
}
