#version 330 core

struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec4 color;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

in vec3 normal;
in vec3 frag_pos;
in vec2 uv;
out vec4 frag_color;

uniform mat3 normal_matrix;
uniform vec3 camera_pos;

uniform Material material;
uniform Light light;

uniform sampler2D tex;

void main()
{
	frag_color = vec4(0.5, 0.5, 0.5, 1.0);
	// vec4 ambient = light.ambient * material.ambient * light.color;

	// vec3 norm = normal_matrix * normalize(normal);
	// vec3 lightDir = normalize(light.position - frag_pos);
	// float diffuse_strength = max(dot(norm, lightDir), 0.0);
	// vec4 diffuse = light.diffuse * diffuse_strength * material.diffuse * light.color;

	// vec3 camera_direction = normalize(camera_pos - frag_pos);
	// vec3 reflectDir = reflect(-lightDir, norm);
	// float specular_strength = pow(max(dot(camera_direction, reflectDir), 0.0), material.shininess);
	// vec4 specular = light.specular * specular_strength * material.specular * light.color;

	// frag_color = texture(tex, uv) * (ambient + diffuse + specular);
}