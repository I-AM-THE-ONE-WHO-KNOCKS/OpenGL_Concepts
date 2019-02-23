#version 330
in vec3 v_Color;
out vec4 o_color;

void main(void) {

    o_color = vec4(v_Color,1.0);

} 
