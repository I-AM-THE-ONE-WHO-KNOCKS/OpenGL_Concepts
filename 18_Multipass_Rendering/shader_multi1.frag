
varying float distance;
varying vec4 v_Color;
varying vec2 v_TextureCoordinates;
uniform sampler2D activeTexture;
uniform sampler2D activeTexture1;
uniform int enableTexture;

void main(void)
{
    float T = 1.0;
    vec2 st = v_TextureCoordinates.st;

    if(enableTexture ==  1){
        //read color data from texture
        vec4 textureColor =
                texture2D(activeTexture,
                          v_TextureCoordinates);

        vec3 neg = vec3(1.0, 1.0, 1.0) - textureColor.rgb;

        gl_FragColor = vec4(mix(textureColor.rgb, neg, T), 1.0);
    }
    else{
        gl_FragColor = v_Color;
    }
}
