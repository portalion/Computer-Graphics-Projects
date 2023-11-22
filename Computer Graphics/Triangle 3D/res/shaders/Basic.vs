#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 FragPos;

void main() {
    gl_Position = projectionMatrix * viewMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    FragPos = aPos;
}