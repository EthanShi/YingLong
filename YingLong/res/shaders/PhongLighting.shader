#shader vertex
#version 330 core

layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTexCoords;
layout(location = 3) in vec3 VertexColor;

out vec3 FragColor;
out vec3 FragNormal;
out vec3 FragWorldPos;
out vec2 FragTexCoords;

uniform float UnitScale;

uniform mat4 Model;
uniform mat4 VP;
uniform mat4 InverseModel;

void main()
{
    mat4 ModelMat = Model * mat4(UnitScale);
    mat4 InverseModelMat = InverseModel * inverse(mat4(UnitScale));
    gl_Position = VP * ModelMat * vec4(VertexPos, 1.0f);
    FragColor = VertexColor;
    FragNormal = mat3(transpose(InverseModelMat)) * VertexNormal;
    FragWorldPos = vec3(ModelMat * vec4(VertexPos, 1.0));
    FragTexCoords = VertexTexCoords;
}


#shader fragment
#version 330 core


struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;


struct MaterialWithMap {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform MaterialWithMap materialWithMap;

uniform int useMaterialWithMap; // 0 or 1

struct Light {
    int type;

    vec3 positionOrDirection;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    vec3 spotDirection;
    float innerCutOffInCos;
    float outerCutOffInCos;
};

uniform int LightCounts;

#define LIGHT_MAX_COUNTS 16
uniform Light Lights[LIGHT_MAX_COUNTS];

vec3 CalcAmbient(Light light, vec2 fragTexCoords)
{
    return light.ambient * (
        useMaterialWithMap * vec3(texture(materialWithMap.diffuse, fragTexCoords))
        + (1 - useMaterialWithMap) * material.ambient);
}

vec3 CalcDiffuse(Light light, vec3 normal, vec3 lightDir, vec2 fragTexCoords)
{
    float diff = max(dot(normal, lightDir), 0.0);
    return light.diffuse * diff * (
        useMaterialWithMap * vec3(texture(materialWithMap.diffuse, fragTexCoords))
        + (1 - useMaterialWithMap) * material.diffuse);
}

vec3 CalcSpecular(Light light, vec3 normal, vec3 lightDir, vec3 viewDir, vec2 fragTexCoords)
{
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    return light.specular * spec * (
        useMaterialWithMap * vec3(texture(materialWithMap.specular, fragTexCoords))
        + (1 - useMaterialWithMap) * material.specular);
}

float CalcIntensity(Light light, vec3 lightDir, vec3 fragPos, float unitScale)
{
    float distance = length(light.positionOrDirection * unitScale - fragPos);
    float intensity = 1.f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    if (light.type == 2)
    {
        float theta = dot(lightDir, normalize(-light.spotDirection));
        float epsilon = light.innerCutOffInCos - light.outerCutOffInCos;
        intensity *= clamp((theta - light.outerCutOffInCos) / epsilon, 0.0, 1.0);
    }
    return intensity;
}

vec3 CalcLight(Light light, vec3 normal, vec3 viewDir, vec3 fragPos, vec2 fragTexCoords, float unitScale)
{
    vec3 lightDir;
    if (light.type == 0)
    {
        lightDir = normalize(-light.positionOrDirection);
    }
    else
    {
        lightDir = normalize(light.positionOrDirection * unitScale - fragPos);
    }

    vec3 ambient = CalcAmbient(light, fragTexCoords);
    vec3 diffuse = CalcDiffuse(light, normal, lightDir, fragTexCoords);
    vec3 specular = CalcSpecular(light, normal, lightDir, viewDir, fragTexCoords);

    return CalcIntensity(light, lightDir, fragPos, unitScale) * (ambient + diffuse + specular);
}

out vec4 FragColor;

in vec3 FragNormal;
in vec3 FragWorldPos;
in vec2 FragTexCoords;

uniform float UnitScale;

uniform vec3 viewPos;

void main()
{
    vec3 norm = normalize(FragNormal);
    vec3 viewDir = normalize(viewPos * UnitScale - FragWorldPos);

    vec3 result;
    for (int i = 0; i < LightCounts; i++)
    {
        result += CalcLight(Lights[i], norm, viewDir, FragWorldPos, FragTexCoords, UnitScale);
    }

    FragColor = vec4(result, 1.0);
};