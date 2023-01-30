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
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

out vec4 FragColor;

in vec3 FragNormal;
in vec3 FragWorldPos;
in vec2 FragTexCoords;

uniform float UnitScale;

uniform vec3 viewPos;

void main()
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, FragTexCoords));

    vec3 norm = normalize(FragNormal);
    vec3 lightDir = normalize(light.position * UnitScale - FragWorldPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, FragTexCoords));

    vec3 viewDir = normalize(viewPos * UnitScale - FragWorldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, FragTexCoords));

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
};