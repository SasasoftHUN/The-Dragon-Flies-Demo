#version 330

layout(location = 0) in vec3 vs_in_pos;
layout(location = 1) in vec3 vs_in_norm;
layout(location = 2) in vec2 vs_in_tex;

out vec3 vs_out_pos;
out vec3 vs_out_normal;
out vec2 vs_out_tex;

uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;

void main()
{
	gl_Position = MVP * vec4( vs_in_pos, 1 );

	vs_out_pos = (world * vec4(vs_in_pos, 1)).xyz;
	vs_out_normal = (worldIT * vec4(vs_in_norm, 0)).xyz;
	vs_out_tex = vs_in_tex;
}


