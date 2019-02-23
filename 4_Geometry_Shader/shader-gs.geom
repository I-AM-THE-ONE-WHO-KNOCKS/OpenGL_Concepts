#version 330

in vec4 v_Color[];
layout (points) in;
layout (points, max_vertices = 100) out;
out vec4 vv_Color;

void main()
{
    for(int i=0; i<gl_in.length(); ++i){
    gl_Position = gl_in[i].gl_Position - vec4(-0.2, 0.0, 0.0, 0.0);
    vv_Color = v_Color[i];
    EmitVertex();
    gl_Position = gl_in[i].gl_Position - vec4(0.0, -0.2, 0.0, 0.0);
    vv_Color = v_Color[i];
    EmitVertex();
    gl_Position = gl_in[i].gl_Position - vec4(0.2, 0.0, 0.0, 0.0);
    vv_Color = v_Color[i];
    EmitVertex();

    EndPrimitive();
    }
}
