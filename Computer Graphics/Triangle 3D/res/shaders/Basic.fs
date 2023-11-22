#version 330 core

in vec3 FragPos;
out vec4 FragColor;

uniform float m = 100;
uniform float kd = 0.8;
uniform float ks = 0.2;

uniform vec3 lightningSourcePosition;

uniform vec3 lightColor = vec3(1, 1, 1);
uniform vec3 objectColor = vec3(0, 1, 0);
uniform vec3 normal;

void main() {
    vec3 lightningSourceVector = normalize(lightningSourcePosition - FragPos);
    vec3 r = 2 * dot(normal, lightningSourceVector) * normal - lightningSourceVector;
    FragColor = vec4(kd * lightColor * objectColor * dot(lightningSourceVector, normalize(normal)) +
        ks * lightColor * objectColor * pow(dot(vec3(0., 0., 1.), r), m), 1.0);
}