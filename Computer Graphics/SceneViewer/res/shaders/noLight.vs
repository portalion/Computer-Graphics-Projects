#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 u_WorldMatrix;
uniform mat4 u_ModelMatrix = mat4(1.0);

void main() {
    gl_Position = u_WorldMatrix * u_ModelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}