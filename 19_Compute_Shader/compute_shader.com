#version 430

layout (local_size_x = 32, local_size_y = 16) in;
layout (rgba8) uniform image2D output_image;

void main() {
    imageStore(output_image, ivec2(gl_GlobalInvocationID.xy), vec4(vec2(gl_LocalInvocationID.xy) / vec2(gl_WorkGroupSize.xy) , 0.0, 0.0));
}


//vec4(vec2(gl_LocalInvocationID.xy) / vec2(gl_WorkGroupSize.xy) , 0.0, 1.0)
