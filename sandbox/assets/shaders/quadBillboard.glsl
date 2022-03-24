#region Vertex

#version 440 core

layout (location = 0) in vec2 a_relativePosition;
layout (location = 1) in vec3 a_quadCentre;
layout (location = 2) in vec2 a_texCoord;
layout (location = 3) in int a_texUnit;
layout (location = 4) in vec4 a_tint;

out vec2 texCoord;
out flat int texUnit;
out vec4 tint;

layout (std140) uniform b_camera
{
	mat4 u_view;
	mat4 u_projection;
};

void main()
{
	texCoord = vec2(a_texCoord);
	texUnit = a_texUnit;
	tint = a_tint;

	vec3 cameraRight = vec3(u_view[0][0], u_view[1][0], u_view[2][0]);
	vec3 cameraUp = vec3(u_view[0][1], u_view[1][1], u_view[2][1]);

	vec3 position = a_quadCentre + cameraRight * a_relativePosition.x + cameraUp * a_relativePosition.y;

	gl_Position = u_projection * u_view * vec4(position, 1.0f);
}

#region Fragment

#version 440 core

layout (location = 0) out vec4 colour;

in vec2 texCoord;
in flat int texUnit;
in vec4 tint;

uniform sampler2D[32] u_texData;

void main()
{
	vec4 textureColour = texture(u_texData[texUnit], texCoord);
	if(textureColour.a < 0.05f)
	{
		discard;
	}
	//colour = textureColour * tint;
	colour = vec4(0, 1, 1, 1);
}
