#version 330
in vec4 a_Position;
in vec4 a_Color;
in vec3 a_Normal;
in vec2 a_TextureCoordinates;
out vec2 v_TextureCoordinates;
out vec4 v_Color;
out vec3 N;
out vec3 L;
out vec3 V;
uniform mat4 u_ModelMatrix;
uniform mat4 u_ProjectionMatrix;
uniform vec3 light_pos = vec3(100.0, 100.0, 100.0);

void main(void) {
	gl_Position = u_ProjectionMatrix * u_ModelMatrix * a_Position;
	v_TextureCoordinates = a_TextureCoordinates;
	// Calculate view-space coordinate
    vec4 P = u_ModelMatrix * a_Position;

    // Calculate normal in view space
    N = mat3(u_ModelMatrix) * a_Normal;

    // Calculate view-space light vector
    L = light_pos - P.xyz;
    
	// Calculate view vector (simply the negative of the view-space position)
    V = -P.xyz;

	v_Color = a_Color;
}