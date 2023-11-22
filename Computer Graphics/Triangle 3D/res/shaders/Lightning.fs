#version 330 core

in vec3 FragPos;
out vec4 FragColor;

uniform float m = 1;
uniform float kd = 1;
uniform float ks = 0;

uniform vec3 lightningSourcePosition;

uniform vec3 lightColor = vec3(1, 1, 1);
uniform vec3 objectColor = vec3(1, 0, 0);
uniform vec3 normal;

void main() {
    vec3 lightningSourceVector = FragColor = vec4(kd * lightColor * objectColor * dot(lightningSourceVector, normal), 1.0);
}