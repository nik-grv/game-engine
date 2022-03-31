#region Vertex

#version 440 core
			
layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec3 a_vertexNormal;
layout(location = 2) in vec2 a_texCoord;
out vec3 fragmentPos;
out vec3 normal;
out vec2 texCoord;

layout (std140) uniform ubo_camera
{
	mat4 u_projection;
	mat4 u_view;
};

uniform mat4 u_model;

void main()
{
	fragmentPos = vec3(u_model * vec4(a_vertexPosition, 1.0));
	normal = mat3(transpose(inverse(u_model))) * a_vertexNormal;
	texCoord = vec2(a_texCoord.x, a_texCoord.y);
	gl_Position =  u_projection * u_view * u_model * vec4(a_vertexPosition,1.0);
}

#region Fragment

#version 440 core
			
layout(location = 0) out vec4 colour;
in vec3 normal;
in vec3 fragmentPos;
in vec2 texCoord;

layout(std140) uniform ubo_lights
{
	vec3 u_lightPos;
	vec3 u_viewPos;
	vec3 u_lightColour;
};
uniform vec4 u_tint;
uniform sampler2D u_texData;
uniform int setPP = 0;

// Controls how many lines are drawn. Lower value means more edges drawn.
const float EDGE_THRESHOLD = 0.5;
// Makes everything a bit brighter
const float COLOR_MULTIPLIER = 1.;

// Color weights for nicer grayscale
const vec3 W = vec3(0.2125, 0.7154, 0.0721);

int screenWidth = 1280;
int screenHeight = 720;

void main()
{
	float ambientStrength = 0.4;
	vec3 ambient = ambientStrength * u_lightColour;
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(u_lightPos - fragmentPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * u_lightColour;
	float specularStrength = 0.8;
	vec3 viewDir = normalize(u_viewPos - fragmentPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specularStrength * spec * u_lightColour;  
	
	if(setPP == 0)
	{
		colour = vec4((ambient + diffuse + specular), 1.0) * texture(u_texData, texCoord) * u_tint;
		//float avg = (colour.r + colour.g + colour.b) / 3.0f;
		//colour = vec4(vec3(avg),1.0f);
	}
	else
	{
		vec4 Newcolour = texture2D(u_texData,texCoord);

		Newcolour.rgb *= vec3(COLOR_MULTIPLIER, COLOR_MULTIPLIER, COLOR_MULTIPLIER);

        // Single pixel offsets in the (0.0, 1.0) address space
        float dx = 1.0 / screenWidth;
        float dy = 1.0 / screenHeight;

        // Surrounding pixels as grayscale: {bottom,center,top}{left,center,right}
        float bl = dot(texture2D(u_texData, texCoord - vec2(dx, dy)).rgb, W);
        float tr = dot(texture2D(u_texData, texCoord + vec2(dx, dy)).rgb, W);
        float br = dot(texture2D(u_texData, texCoord - vec2(dx,-dy)).rgb, W);
        float tl = dot(texture2D(u_texData, texCoord + vec2(dx,-dy)).rgb, W);
        float bc = dot(texture2D(u_texData, texCoord - vec2(dx, 0.)).rgb, W);
        float tc = dot(texture2D(u_texData, texCoord + vec2(dx, 0.)).rgb, W);
        float cl = dot(texture2D(u_texData, texCoord - vec2(0., dy)).rgb, W);
        float cr = dot(texture2D(u_texData, texCoord + vec2(0., dy)).rgb, W);

        // Weighted differences of the surrounding pixels
        float h = -br - 2.0 * cr - tr + bl + 2.0 * cl + tl;
        float v = -bl - 2.0 * bc - br + tl + 2.0 * tc + tr;

        // All those above threshold are treated as edges and drawn black.
        if (length(vec2(h, v)) > EDGE_THRESHOLD)
        Newcolour  = vec4(0.0, 0.0, 0.0, 1.0);

		colour = vec4((ambient + diffuse + specular), 1.0) * texture(u_texData, texCoord) * u_tint;
		colour = Newcolour;

	}
}
