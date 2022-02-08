#region Vertex

#version 440 core

layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec3 a_vertexColour;
out vec3 fragmentColour;

layout (std140) uniform ubo_camera
{
	mat4 u_projection;
	mat4 u_view;
};

uniform mat4 u_model;

void main()
{
	fragmentColour = a_vertexColour;
	gl_Position =  u_projection * u_view * u_model * vec4(a_vertexPosition,1);
}

#region Fragment

#version 440 core

layout(location = 0) out vec4 colour;
in vec3 fragmentColour;
void main()
{
	colour = vec4(fragmentColour, 1.0);
}
