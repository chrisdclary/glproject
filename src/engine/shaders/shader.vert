#version 330
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 in_color;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
out vec3 vert_color;

void main()
{
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(pos, 1.0);
    vert_color = in_color;
}