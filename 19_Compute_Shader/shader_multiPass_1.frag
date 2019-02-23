uniform sampler2D u_Texture;
varying vec2 v_TextureCoordinates;
void main()
{
    const vec3 W = vec3(0.2125, 0.1754, 0.0721);
    vec3 irgb = texture2D(u_Texture, v_TextureCoordinates).rgb;
    float luminance = dot(irgb, W);
    gl_FragColor = vec4(luminance, luminance, luminance, 1.0);
}
