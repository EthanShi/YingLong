#shader vertex
#version 330 core

layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTexCoords;
layout(location = 3) in vec3 VertexColor;

out vec3 FragColor;
out vec3 FragNormal;
out vec3 FragWorldPos;

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

struct Light {
    int type;

    vec3 positionOrDirection;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Light light;

out vec4 FragColor;

in vec3 FragNormal;
in vec3 FragWorldPos;

uniform float UnitScale;

uniform vec3 viewPos;

void main()
{
    vec3 ambient = material.ambient * light.ambient;

    vec3 norm = normalize(FragNormal);

    vec3 lightDir;
    float attenuation;

    if (light.type == 0)
    {
        lightDir = normalize(-light.positionOrDirection);
        attenuation = 1.0;
    }
    else if (light.type == 1)
    {
        lightDir = normalize(light.positionOrDirection * UnitScale - FragWorldPos);
        float distance = length(light.positionOrDirection * UnitScale - FragWorldPos);
        attenuation = 1.f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    }
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    vec3 viewDir = normalize(viewPos * UnitScale - FragWorldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * light.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
};