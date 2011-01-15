//basic fragment shader

varying vec3 normal;
varying vec3 trueNormal;
varying vec4 position;

//attribute vec3 tangent;
//attribute vec3 bitangent;

uniform sampler2D color_map;
uniform sampler2D normal_map;

vec3 view;
vec3 reflection;
vec3 light;
float light_dist;

void main() {

	view = normalize(position.xyz);
	light_dist = length((position-gl_LightSource[0].position).xyz);
	light = normalize((position-gl_LightSource[0].position).xyz);
	reflection = normalize(reflect(view, normal));

	float kdiff = -dot(light, normal);
	kdiff = max(kdiff, 0.0);
	//kdiff = pow(kdiff, modifier);
	
	float kspec =-dot(reflection,light);
	kspec = max(kspec, 0.0);
	kspec = pow(kspec, gl_FrontMaterial.shininess);
	//kspec = pow(kspec, modifier);
	
	//vec4 diffuse_color = gl_FrontMaterial.diffuse*gl_LightSource[0].diffuse*kdiff / pow(light_dist, 0.5f);
	vec4 diffuse_color = texture2D(color_map, gl_TexCoord[0].st)*gl_LightSource[0].diffuse*kdiff / pow(light_dist, 0.5f);
	
	vec4 specular_color = gl_FrontMaterial.specular*gl_LightSource[0].specular*kspec / pow(light_dist, 1.0f);
	
	gl_FragColor = diffuse_color + specular_color;
	gl_FragColor.x = max(gl_FragColor.x, gl_FrontMaterial.ambient.x);
	gl_FragColor.y = max(gl_FragColor.y, gl_FrontMaterial.ambient.y);
	gl_FragColor.z = max(gl_FragColor.z, gl_FrontMaterial.ambient.z);
	
}