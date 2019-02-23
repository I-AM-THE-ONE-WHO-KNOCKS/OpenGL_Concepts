attribute vec4 a_Position;
attribute vec4 a_Color;
attribute vec2 a_TextureCoordinates;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

varying vec2 v_TextureCoordinates;
varying vec4 v_Color;

void main(void)
{
    gl_Position =
            u_ProjectionMatrix *
            u_ViewMatrix *
            u_ModelMatrix *
            a_Position;

    v_TextureCoordinates = a_TextureCoordinates;
    v_Color = a_Color;
}
