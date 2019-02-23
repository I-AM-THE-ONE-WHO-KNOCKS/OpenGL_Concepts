#version 330
in vec4 v_Color;
out vec4 o_Color;

varying float distance;


void main(void)
{
    if(distance < 0){
        discard;
    }else{
        o_Color = v_Color;
    }
}

