#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * vec4(position.x, position.y, position.z, 1.f);
};

#shader fragment
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}