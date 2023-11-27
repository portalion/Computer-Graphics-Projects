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
uniform bool showNormals;
uniform sampler2D Texture;
uniform sampler2D NormalMap;

vec3 getNormal() {
    float eps = 0.001;
    vec3 b;
    if(abs(Normal.x) <= eps && abs(Normal.y) <= eps && abs(Normal.z - 1) <= eps)
        b = vec3(0, 1, 0);
    else
        b = cross(Normal, vec3(0, 0, 1));
    vec3 t = cross(b, Normal);
    mat3 matr;
    matr[0] = b;
    matr[1] = t;
    matr[2] = Normal;

    return matr * (texture(NormalMap, TexCoords).xyz * 2.f - 1.f);
}
void main() {
    vec3 currentNormal = Normal;
    if(showNormals)
        currentNormal = normalize(getNormal());

    vec3 lightningSourceVector = normalize(lightningSourcePosition - FragPos);
    vec3 r = normalize(2 * dot(currentNormal, lightningSourceVector) * currentNormal - lightningSourceVector);

    vec4 currentColor = vec4(objectColor, 1.0);
    if(showTexture)
        currentColor = texture(Texture, TexCoords);

    FragColor = vec4(kd * lightColor * currentColor.xyz * dot(lightningSourceVector, currentNormal) +
        ks * lightColor * currentColor.xyz * pow(dot(vec3(0., 0., 1.), r), m), 1.0);
}