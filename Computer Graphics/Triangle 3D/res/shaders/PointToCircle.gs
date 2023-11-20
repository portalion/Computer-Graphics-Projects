#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 74) out;

uniform float radius = 6;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main() {
    const int numberOfVerticles = 36;
    float angle = 360.f / numberOfVerticles;
    for(int i = 0; i <= numberOfVerticles; i++)
    {
        float currentAngle = angle * i;
        gl_Position = viewMatrix * gl_in[0].gl_Position + vec4(radius * cos(radians(currentAngle)), 
            radius * sin(radians(currentAngle)), 0.f, 0.f);
        gl_Position = projectionMatrix * gl_Position;
        EmitVertex();
        gl_Position = projectionMatrix * viewMatrix * gl_in[0].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}