#version 330

in vec3 vs_out_pos;
in vec3 vs_out_normal;
in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform vec3 eye_pos = vec3(0, 15, 15);

uniform vec3 light_pos = vec3( 5, 5, 5 );

uniform vec4 La = vec4(0.1f, 0.1f, 0.1f, 1);
uniform vec4 Ld = vec4(0.75f, 0.75f, 0.75f, 1);
uniform vec4 Ls = vec4(1, 1, 1, 1);

uniform float specular_power = 32;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform samplerCube skyboxTexture;

void main()
{
	vec4 ambient = La;

	vec3 normal = normalize(vs_out_normal);
	vec3 toLight = normalize(light_pos - vs_out_pos);
	float di = clamp(dot(toLight, normal), 0.0f, 1.0f);
	vec4 diffuse = Ld * di;

	vec4 specular = vec4(0);
	float specularPower = texture(specularTexture, vs_out_tex.st).r;
	if ( di > 0 )
	{
		vec3 e = normalize(eye_pos - vs_out_pos);
		vec3 r = reflect(-toLight, normal);
		float si = pow(clamp(dot(e, r), 0.0f, 1.0f), specular_power);
		specular = Ls * si * specularPower;
	}

	vec3 skyboxReflect = reflect(normalize(vs_out_pos - eye_pos), normal);

	fs_out_col = (ambient + diffuse + specular) *
		mix(texture(diffuseTexture, vs_out_tex.st), texture(skyboxTexture, skyboxReflect), specularPower * 0.5f);
	//fs_out_col = (ambient + diffuse + specular );
	//fs_out_col = texture(texImage, vs_out_tex.st);
	//fs_out_col = vec4(normal, 1);
	//fs_out_col = vec4(specularPower, 1, 1, 1);
}










