//basic fragment shader

varying vec3 eyeNormal;
varying vec3 objectNormal;
varying vec4 position;

varying vec3 eyeTangent; 
varying vec3 eyeBitangent;

uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform int hasTexture [3];

vec3 view;
vec3 reflection;
vec3 light;
float light_dist;

void main() {

	vec3 normal;
	if (hasTexture[1] == 1) {
		//Use normal map
		vec3 nmap = (texture2D(normalMap, gl_TexCoord[0].st).xyz) * 2.0 - 1.0;
		normal = normalize( eyeTangent*nmap.x + eyeBitangent*nmap.y + eyeNormal*nmap.z );
	}
	else {
		normal = eyeNormal;
	}

	view = normalize(position.xyz);
	light_dist = length((position-gl_LightSource[0].position).xyz);
	light = normalize((position-gl_LightSource[0].position).xyz);
	reflection = normalize(reflect(view, normal));

	float kdiff = -dot(light, normal);
	kdiff = max(kdiff, 0.0);
	
	float kspec =-dot(reflection,light);
	kspec = max(kspec, 0.0);
	kspec = pow(kspec, gl_FrontMaterial.shininess);
	if (hasTexture[1] == 1) {
		kspec *= texture2D(normalMap, gl_TexCoord[0].st).w;
	}
	
	float attenuation = 1.0/pow(light_dist, 0.5);
	
	vec4 diffuse_color;

	//Use the texture color as diffuse color if it exists
	if (hasTexture[0] == 1)
		diffuse_color = texture2D(colorMap, gl_TexCoord[0].st)*gl_LightSource[0].diffuse*kdiff*attenuation;
	else
		diffuse_color = gl_FrontMaterial.diffuse*gl_LightSource[0].diffuse*kdiff*attenuation;

	vec4 specular_color = gl_FrontMaterial.specular*gl_LightSource[0].specular*kspec*attenuation;
	
	gl_FragColor = diffuse_color + specular_color;
	gl_FragColor.x = max(gl_FragColor.x, gl_FrontMaterial.ambient.x);
	gl_FragColor.y = max(gl_FragColor.y, gl_FrontMaterial.ambient.y);
	gl_FragColor.z = max(gl_FragColor.z, gl_FrontMaterial.ambient.z);
	
}