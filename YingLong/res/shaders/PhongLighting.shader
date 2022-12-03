#shader vertex
#version 330 core

layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec3 VertexColor;

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

out vec4 FragColor;

in vec3 FragNormal;
in vec3 FragWorldPos;

uniform float UnitScale;

uniform vec3 lightColor;
uniform vec3 lightPos;

uniform vec3 viewPos;

void main()
{
    vec3 ambient = material.ambient * lightColor;

    vec3 norm = normalize(FragNormal);
    vec3 lightDir = normalize(lightPos * UnitScale - FragWorldPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * diff * material.diffuse;

    vec3 viewDir = normalize(viewPos * UnitScale - FragWorldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * lightColor;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
};