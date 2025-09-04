#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color_a;

uniform mat4 m_view;
uniform mat4 m_proj;
out vec3 colorFrag;

void main()
{
    gl_Position  = m_proj*m_view*vec4(position.x,position.y,-1.00000001, 1.0);
    //gl_Position  = m_view*vec4(position, 1.0);
    colorFrag = color_a; 
}
