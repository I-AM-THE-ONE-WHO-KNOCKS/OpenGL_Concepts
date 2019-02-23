#version 330
in vec3 a_Normal;
in vec4 a_Position;
in vec4 a_Color;
out vec3 v_Color;
uniform mat4 u_ModelMatrix;
uniform mat4 u_ProjectionMatrix;
uniform vec3 u_Ambient = vec3(0.3, 0.3, 0.3);
uniform vec3 diffuseMaterial = vec3(0.4,0.6,0.2);
uniform vec3 specularmaterial = vec3(0.6,0.5,3.0);
uniform float shininess = 70;
uniform vec3 light_pos = vec3(10.0,10.0,10.0);



void main(void) {
	gl_Position = u_ProjectionMatrix * u_ModelMatrix * a_Position;

    vec4 P = u_ModelMatrix * a_Position;
    vec3 N = mat3(u_ModelMatrix) * a_Normal;
    vec3 L = light_pos - P.xyz;
    N = normalize(N);
    L = normalize(L);

    vec3 diffuse = max(dot(N, L), 0.0) * diffuseMaterial;

    vec3 V = -P.xyz;//eye vector
    V = normalize(V);

    vec3 R = reflect(-L, N);
    vec3 specular = pow(max(dot(V, R), 0.0),shininess) * specularmaterial;


	vec4 normal = vec4(a_Normal.xyz, 1.0);
    v_Color = u_Ambient.rgb + diffuse + specular;
}
