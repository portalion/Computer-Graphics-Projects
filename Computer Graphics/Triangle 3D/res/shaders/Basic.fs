#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
out vec4 FragColor;

uniform float m = 100;
uniform float kd = 0.8;
uniform float ks = 0.2;

uniform vec3 lightningSourcePosition;

uniform vec3 lightColor = vec3(1, 1, 1);
uniform vec3 objectColor = vec3(0, 1, 0);
uniform bool showTexture;
uniform sampler2D ourTexture;

void main() {
    vec3 lightningSourceVector = normalize(lightningSourcePosition - FragPos);
    vec3 r = normalize(2 * dot(Normal, lightningSourceVector) * Normal - lightningSourceVector);
    vec4 currentColor = vec4(objectColor, 1.0);
    if(showTexture) {
        currentColor = texture(ourTexture, TexCoords);
    }

    FragColor = vec4(kd * lightColor * currentColor.xyz * dot(lightningSourceVector, Normal) +
        ks * lightColor * currentColor.xyz * pow(dot(vec3(0., 0., 1.), r), m), 1.0);
}