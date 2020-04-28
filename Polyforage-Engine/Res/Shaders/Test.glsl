#vertex
#version 330

layout(location = 0) in vec3 Pos;
layout(location = 1) in vec3 Color;

uniform mat4 Model;
uniform mat4 ViewProjection;

out vec3 Color0;

void main()
{
	Color0 = Color;
	gl_Position = vec4(Pos, 1.0) * ViewProjection * Model;
}

#fragment
#version 330

in vec3 Color0;
out vec4 OutColor;

void main()
{
	OutColor = vec4(Color0, 1.0);
}