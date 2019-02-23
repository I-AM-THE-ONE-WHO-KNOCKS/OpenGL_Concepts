varying vec4 v_Color;
varying vec2 v_TextureCoordinates;
uniform sampler2D activeTexture;
uniform int enableTexture;

void main(void)
{
    if(enableTexture ==  1){
        //read color data from texture
        vec4 textureColor =
                texture2D(activeTexture,
                          v_TextureCoordinates);

        gl_FragColor = textureColor;
    }
    else{
        gl_FragColor = v_Color;
    }
}
