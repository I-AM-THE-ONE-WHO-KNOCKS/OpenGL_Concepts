#version 330
out vec4 outColor;
in vec3 N;
in vec3 L;
in vec3 V;

uniform vec3 light_pos = vec3(100.0, 100.0, 100.0);
uniform vec3 diffuse_component = vec3(0.9, 0.9, 0.9);
uniform vec3 specular_component = vec3(0.7);
uniform float specular_power = 128.0;
uniform vec3 ambient = vec3(0.1, 0.1, 0.1);

void main(void) {

// Normalize the incoming N, L and V vectors
    vec3 N = normalize(N);
    vec3 L = normalize(L);
    vec3 V = normalize(V);

    // Calculate R locally
    vec3 R = reflect(-L, N);

    // Compute the diffuse and specular components for each fragment
    vec3 diffuse = max(dot(N, L), 0.0) * diffuse_component;
    vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * specular_component;

    // Write final color to the framebuffer
    outColor = vec4(ambient + diffuse + specular, 1.0);

} 
