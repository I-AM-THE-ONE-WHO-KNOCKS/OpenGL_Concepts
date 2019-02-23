#version 330
attribute vec4 a_Position;
in vec4 a_Color;
out vec4 v_Color;

uniform vec4 clippingPlane;
varying float distance;

void main(void)
{
    gl_Position = a_Position;

    distance = (dot(gl_Position.xyz, clippingPlane.xyz) + clippingPlane.w);

    v_Color = a_Color;
}
