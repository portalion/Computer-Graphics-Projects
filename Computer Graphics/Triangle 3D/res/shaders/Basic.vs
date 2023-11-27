#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextCoords;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main() {
    gl_Position = projectionMatrix * viewMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    FragPos = aPos;
    Normal = normalize(aNormal);
    TexCoords = aTextCoords;
}