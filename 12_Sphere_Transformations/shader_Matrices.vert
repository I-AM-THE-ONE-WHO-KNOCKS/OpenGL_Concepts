#version 330
attribute vec4 a_Position;
in vec4 a_Color;
out vec4 v_Color;

uniform mat4 u_ModelMatrix;
uniform mat4 u_projectionMatrix;
uniform mat4 u_virewMatrix;

void main(void)
{
    gl_Position = u_projectionMatrix * u_virewMatrix * u_ModelMatrix * a_Position;

    v_Color = a_Color;
}
