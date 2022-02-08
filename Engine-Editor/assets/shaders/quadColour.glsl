#region Vertex

#version 440 core

layout(location = 0) in vec4 a_vertexPosition;
layout(location = 1) in vec2 a_texCoord;
layout(location = 2) in int a_texUnit;
layout(location = 3) in vec4 a_tint;

out vec2 texCoord;
out flat int textUnit;
out vec4 tint;

layout (std140) uniform ubo_camera
{
	mat4 u_view;
	mat4 u_projection;
};

void main()
{
	texCoord = vec2(a_texCoord);
	textUnit = a_texUnit;
	tint = a_tint;
	gl_Position =  u_projection * u_view * a_vertexPosition;
}

#region Fragment

#version 440 core

layout(location = 0) out vec4 colour;

in vec2 texCoord;
in flat int textUnit;
in vec4 tint;

uniform sampler2D[32] u_texData;

void main()
{
	colour = texture(u_texData[textUnit],texCoord) * tint;
}
