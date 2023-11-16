#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 72) out;

uniform float radius = 6;
uniform mat4 projectionMatrix;

void main() {
    for (int i = 0; i <= 36; ++i) {
        float angle = radians(float(i) * 360.0 / 30.0);
        gl_Position = gl_in[0].gl_Position + projectionMatrix * vec4(radius * cos(angle), radius * sin(angle), 0.0, 0.0);
        EmitVertex();
        gl_Position = gl_in[0].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}