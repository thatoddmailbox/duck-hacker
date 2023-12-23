#version 330 core

#define NUM_LIGHTS 2

struct Material {
	vec4 ambient;
	vec4 diffuse;
	sampler2D diffuse_map;
	float diffuse_map_enabled;
	vec4 specular;
	float shininess;
};

struct Light {
	vec3 direction;
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
uniform Light lights[NUM_LIGHTS];

vec4 calc_light(Light light, vec3 camera_direction, vec2 uv_corrected)
{
	vec4 ambient = light.ambient * material.ambient * light.color;

	vec3 light_direction = normalize(-light.direction);

	vec3 norm = normal_matrix * normalize(normal);
	float diffuse_strength = max(dot(norm, light_direction), 0.0);
	vec4 material_diffuse = (material.diffuse_map_enabled == 1.0 ? texture(material.diffuse_map, uv_corrected) : material.diffuse);
	vec4 diffuse = light.diffuse * diffuse_strength * material_diffuse * light.color;

	vec3 reflectDir = reflect(-light_direction, norm);
	float specular_strength = pow(max(dot(camera_direction, reflectDir), 0.0), material.shininess);
	vec4 specular = light.specular * specular_strength * material.specular * light.color;

	return (ambient + diffuse + specular);
}

void main()
{
	// convert to OpenGL's coordinate system
	vec2 uv_corrected = vec2(uv.x, 1.0 - uv.y);

	vec3 camera_direction = normalize(camera_pos - frag_pos);

	frag_color = calc_light(lights[0], camera_direction, uv_corrected);
	frag_color += calc_light(lights[1], camera_direction, uv_corrected);
	frag_color.a = 1.0;
}