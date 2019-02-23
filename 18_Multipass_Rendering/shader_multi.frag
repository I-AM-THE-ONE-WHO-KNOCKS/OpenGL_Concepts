varying float distance;
varying vec4 v_Color;
varying vec2 v_TextureCoordinates;
uniform sampler2D activeTexture;
uniform sampler2D activeTexture1;
uniform int enableTexture;

void main(void)
{
    const vec3 W = vec3(0.2125, 0.1754, 0.0721);
    if(enableTexture ==  1){
        //read color data from texture
        vec4 textureColor =
                texture2D(activeTexture,
                          v_TextureCoordinates);
        float luminance = dot(textureColor.rgb, W);

        gl_FragColor = vec4(luminance, luminance, luminance, 1.0);
    }
    else{
        gl_FragColor = v_Color;
    }


}
