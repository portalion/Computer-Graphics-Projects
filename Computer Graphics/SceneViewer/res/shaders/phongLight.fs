#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
struct Light
{
    vec3 position;
    vec3 direction;
    vec3 color;
};

uniform Light pointLight[2];
uniform Light spotlight;
uniform vec3 viewPos; 
uniform vec3 u_Color;

vec3 GetLightning(Light l);

vec3 getPointLight(Light l);
vec3 getSpotlightLight(Light l);



void main()
{
    vec3 result = (getPointLight(pointLight[0]) + getPointLight(pointLight[1])
        + getSpotlightLight(spotlight)) * u_Color;
    FragColor = vec4(result, 1.0);
} 

vec3 GetLightning(Light l)
{
    vec3 lightDir = normalize(l.position - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * l.color;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * l.color; 
    return diffuse + specular;
}

vec3 getPointLight(Light l)
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * l.color;

    return ambient + GetLightning(l);
}

vec3 getSpotlightLight(Light l)
{
    vec3 lightDir = normalize(l.position - FragPos);
    
    // check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-l.direction)); 
    float epsilon   = cos(radians(12.5f)) - cos(radians(17.5f));
    float intensity = clamp((theta - cos(radians(17.5f))) / epsilon, 0.0, 1.0);    

    return 0.1 * l.color + GetLightning(l) * intensity;
}

