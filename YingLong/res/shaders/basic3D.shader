#shader vertex
#version 330 core

layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTexCoords;
layout(location = 3) in vec3 VertexColor;

out vec3 OutVColor;

uniform float UnitScale;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * mat4(UnitScale) * vec4(VertexPos, 1.0f);
    OutVColor = VertexColor;
}


#shader fragment
#version 330 core

out vec4 color;

in vec3 OutVColor;

void main()
{
    color = vec4(OutVColor, 1.0f);
};