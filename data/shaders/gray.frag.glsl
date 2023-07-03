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
	// TODO: this is a BIG hack!!! pls remove me :)
	vec4 material_vary = vec4(1.0, 1.0, 1.0, 1.0);
	if (uv.x == 0.32) {
		material_vary = vec4(0.0, 0.0, 1.0, 1.0);
	}

	vec4 ambient = light.ambient * (material.ambient * material_vary) * light.color;

	vec3 camera_direction = normalize(camera_pos - frag_pos);
	vec3 light_direction = normalize(light.position - frag_pos);

	vec3 norm = normal_matrix * normalize(normal);
	float diffuse_strength = max(dot(norm, light_direction), 0.0);
	vec4 diffuse = light.diffuse * diffuse_strength * (material.diffuse * material_vary) * light.color;

	vec3 reflectDir = reflect(-light_direction, norm);
	float specular_strength = pow(max(dot(camera_direction, reflectDir), 0.0), material.shininess);
	vec4 specular = light.specular * specular_strength * (material.specular * material_vary) * light.color;

	// TODO: this is a hack lol
	specular = vec4(0.15, 0.15, 0.15, 0);

	frag_color = (ambient + diffuse + specular);
}