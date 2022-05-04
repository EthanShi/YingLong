#shader vertex
#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;

out vec3 vertexColor;

void main()
{
	gl_Position = vec4(in_position, 1.0);
	vertexColor = in_color;
};

#shader fragment
#version 330 core

out vec4 color;
in vec3 vertexColor;

void main()
{
	color = vec4(vertexColor, 1.0);
};