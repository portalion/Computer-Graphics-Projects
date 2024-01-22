#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 u_ModelMatrix;
uniform mat4 u_WorldMatrix;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 u_Color;

out vec3 result;

void main()
{
    vec3 FragPos = vec3(u_ModelMatrix * vec4(aPos, 1.0));
    vec3 Normal = mat3(transpose(inverse(u_ModelMatrix))) * aNormal;  
    
    gl_Position = u_WorldMatrix * vec4(FragPos, 1.0);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    result = (ambient + diffuse + specular) * u_Color;
}