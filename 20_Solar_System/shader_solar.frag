varying vec2 v_Texcoord;
//varying vec4 v_Color;
uniform sampler2D activeTexture;

void main(void)
{
    vec4 color = texture2D(activeTexture, v_Texcoord);
//    vec4 vertColor = v_Color;
    gl_FragColor = color;
}
