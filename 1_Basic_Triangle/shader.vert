#version 330
attribute vec4 a_Position;
in vec4 a_Color;
out vec4 v_Color;

void main(void)
{
    gl_Position = a_Position;
    v_Color = a_Color;
}

