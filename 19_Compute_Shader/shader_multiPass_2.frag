precision mediump float;
uniform sampler2D u_Texture;
varying vec2 v_TextureCoordinates;
void main()
{
float T = 1.0;
vec2 st = v_TextureCoordinates.st;
vec3 irgb = texture2D(u_Texture, st).rgb;
vec3 neg = vec3(1., 1., 1.)-irgb;
gl_FragColor = vec4(mix(irgb,neg, T), 1.);
}
