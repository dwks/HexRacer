// Background reflection with Normal Mapping

varying vec3 eyeNormal;
varying vec4 position;

varying vec3 eyeTangent; 
varying vec3 eyeBitangent;

varying mat3 cameraNormalMatrix;

uniform samplerCube cubeMap;
uniform mat4 cameraMatrix;

uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform int hasTexture [4];
uniform int numLights;

void main() {

	vec3 normal;
	if (hasTexture[1] == 1) {
		//Use normal map
		vec3 nmap = (texture2D(normalMap, gl_TexCoord[0].st).xyz) * 2.0 - 1.0;
		normal = normalize( eyeTangent*(-nmap.x) + eyeBitangent*nmap.y + eyeNormal*nmap.z );
	}
	else {
		normal = eyeNormal;
	}
	
	vec3 view = normalize(position.xyz);
	vec3 reflection = normalize(reflect(view, normal));

	//Worldspace calculations----------------------------------------------------------------------------------------
	
	vec3 worldReflect = normalize(cameraNormalMatrix * reflection);
	
	//Lighting calculations----------------------------------------------------------------------------------------
	
	vec4 diffuse_color = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 specular_color = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 ambient_color = vec4(0.0, 0.0, 0.0, 0.0);
	
	for (int i = 0; i < 8; i++) {
	
		if (i < numLights) {
	
			float light_dist = length((position-gl_LightSource[i].position).xyz);
			
			float attenuation = 1.0/(gl_LightSource[i].constantAttenuation + gl_LightSource[i].quadraticAttenuation*light_dist*light_dist);
			if (attenuation >= 0.004) {
			
				attenuation = min(attenuation, 1.0);
				vec3 light = normalize((position-gl_LightSource[i].position).xyz);

				float kdiff = -dot(light, normal);
				kdiff = max(kdiff, 0.0);
				
				float kspec =-dot(reflection,light);
				kspec = max(kspec, 0.0);
				kspec = pow(kspec, gl_FrontMaterial.shininess);
				
				diffuse_color += gl_LightSource[i].diffuse*kdiff*attenuation;
				specular_color += gl_LightSource[i].specular*kspec*attenuation;
				ambient_color += gl_LightSource[i].ambient*attenuation;
			}
			
		}
		
	}
	
	//Apply Material----------------------------------------------------------------------------------------

	vec4 diffuse_base;
	//Use the texture color as diffuse base if it exists
	if (hasTexture[0] == 1)
		diffuse_base = texture2D(colorMap, gl_TexCoord[0].st);
	else
		diffuse_base = gl_FrontMaterial.diffuse;
	
	diffuse_color *= diffuse_base;
	specular_color *= gl_FrontMaterial.specular;
	ambient_color *= diffuse_base;
	
	float krefl = (gl_FrontMaterial.shininess-1.0)/12.0;
	
	if (hasTexture[1] == 1) {
		float shine_factor = texture2D(normalMap, gl_TexCoord[0].st).w;
		specular_color *= shine_factor;
		krefl *= shine_factor;
	}
	
	gl_FragColor = (diffuse_color + ambient_color)*(1.0-krefl) + textureCube(cubeMap, worldReflect)*krefl + specular_color;
}