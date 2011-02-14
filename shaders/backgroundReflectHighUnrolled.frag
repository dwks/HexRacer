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
	
	if (numLights > 0) {
		float light_dist = length((position-gl_LightSource[0].position).xyz);
		float attenuation = min(1.0/(gl_LightSource[0].constantAttenuation + gl_LightSource[0].quadraticAttenuation*light_dist*light_dist), 1.0);
		if (attenuation >= 0.004) {
		
			vec3 light = normalize((position-gl_LightSource[0].position).xyz);
			float kdiff = max( -dot(light, normal), 0.0) ;
			
			float kspec =-dot(reflection,light);
			kspec = max( pow(kspec, gl_FrontMaterial.shininess) , 0.0);
			
			diffuse_color += gl_LightSource[0].diffuse*kdiff*attenuation;
			specular_color += gl_LightSource[0].specular*kspec*attenuation;
			ambient_color += gl_LightSource[0].ambient*attenuation;
		}
	}
	
	if (numLights > 1) {
		float light_dist = length((position-gl_LightSource[1].position).xyz);
		float attenuation = min(1.0/(gl_LightSource[1].constantAttenuation + gl_LightSource[1].quadraticAttenuation*light_dist*light_dist), 1.0);
		if (attenuation >= 0.004) {
		
			vec3 light = normalize((position-gl_LightSource[1].position).xyz);
			float kdiff = max( -dot(light, normal), 0.0) ;
			
			float kspec =-dot(reflection,light);
			kspec = max( pow(kspec, gl_FrontMaterial.shininess) , 0.0);
			
			diffuse_color += gl_LightSource[1].diffuse*kdiff*attenuation;
			specular_color += gl_LightSource[1].specular*kspec*attenuation;
			ambient_color += gl_LightSource[1].ambient*attenuation;
		}
	}
	
	if (numLights > 2) {
		float light_dist = length((position-gl_LightSource[2].position).xyz);
		float attenuation = min(1.0/(gl_LightSource[2].constantAttenuation + gl_LightSource[2].quadraticAttenuation*light_dist*light_dist), 1.0);
		if (attenuation >= 0.004) {
		
			vec3 light = normalize((position-gl_LightSource[2].position).xyz);
			float kdiff = max( -dot(light, normal), 0.0) ;
			
			float kspec =-dot(reflection,light);
			kspec = max( pow(kspec, gl_FrontMaterial.shininess) , 0.0);
			
			diffuse_color += gl_LightSource[2].diffuse*kdiff*attenuation;
			specular_color += gl_LightSource[2].specular*kspec*attenuation;
			ambient_color += gl_LightSource[2].ambient*attenuation;
		}
	}
	
	if (numLights > 3) {
		float light_dist = length((position-gl_LightSource[3].position).xyz);
		float attenuation = min(1.0/(gl_LightSource[3].constantAttenuation + gl_LightSource[3].quadraticAttenuation*light_dist*light_dist), 1.0);
		if (attenuation >= 0.004) {
		
			vec3 light = normalize((position-gl_LightSource[3].position).xyz);
			float kdiff = max( -dot(light, normal), 0.0) ;
			
			float kspec =-dot(reflection,light);
			kspec = max( pow(kspec, gl_FrontMaterial.shininess) , 0.0);
			
			diffuse_color += gl_LightSource[3].diffuse*kdiff*attenuation;
			specular_color += gl_LightSource[3].specular*kspec*attenuation;
			ambient_color += gl_LightSource[3].ambient*attenuation;
		}
	}
	
	if (numLights > 4) {
		float light_dist = length((position-gl_LightSource[4].position).xyz);
		float attenuation = min(1.0/(gl_LightSource[4].constantAttenuation + gl_LightSource[4].quadraticAttenuation*light_dist*light_dist), 1.0);
		if (attenuation >= 0.004) {
		
			vec3 light = normalize((position-gl_LightSource[4].position).xyz);
			float kdiff = max( -dot(light, normal), 0.0) ;
			
			float kspec =-dot(reflection,light);
			kspec = max( pow(kspec, gl_FrontMaterial.shininess) , 0.0);
			
			diffuse_color += gl_LightSource[4].diffuse*kdiff*attenuation;
			specular_color += gl_LightSource[4].specular*kspec*attenuation;
			ambient_color += gl_LightSource[4].ambient*attenuation;
		}
	}
	
	if (numLights > 5) {
		float light_dist = length((position-gl_LightSource[5].position).xyz);
		float attenuation = min(1.0/(gl_LightSource[5].constantAttenuation + gl_LightSource[5].quadraticAttenuation*light_dist*light_dist), 1.0);
		if (attenuation >= 0.004) {
		
			vec3 light = normalize((position-gl_LightSource[5].position).xyz);
			float kdiff = max( -dot(light, normal), 0.0) ;
			
			float kspec =-dot(reflection,light);
			kspec = max( pow(kspec, gl_FrontMaterial.shininess) , 0.0);
			
			diffuse_color += gl_LightSource[5].diffuse*kdiff*attenuation;
			specular_color += gl_LightSource[5].specular*kspec*attenuation;
			ambient_color += gl_LightSource[5].ambient*attenuation;
		}
	}
	
	if (numLights > 6) {
		float light_dist = length((position-gl_LightSource[6].position).xyz);
		float attenuation = min(1.0/(gl_LightSource[6].constantAttenuation + gl_LightSource[6].quadraticAttenuation*light_dist*light_dist), 1.0);
		if (attenuation >= 0.004) {
		
			vec3 light = normalize((position-gl_LightSource[6].position).xyz);
			float kdiff = max( -dot(light, normal), 0.0) ;
			
			float kspec =-dot(reflection,light);
			kspec = max( pow(kspec, gl_FrontMaterial.shininess) , 0.0);
			
			diffuse_color += gl_LightSource[6].diffuse*kdiff*attenuation;
			specular_color += gl_LightSource[6].specular*kspec*attenuation;
			ambient_color += gl_LightSource[6].ambient*attenuation;
		}
	}
	
	if (numLights > 7) {
		float light_dist = length((position-gl_LightSource[7].position).xyz);
		float attenuation = min(1.0/(gl_LightSource[7].constantAttenuation + gl_LightSource[7].quadraticAttenuation*light_dist*light_dist), 1.0);
		if (attenuation >= 0.004) {
		
			vec3 light = normalize((position-gl_LightSource[7].position).xyz);
			float kdiff = max( -dot(light, normal), 0.0) ;
			
			float kspec =-dot(reflection,light);
			kspec = max( pow(kspec, gl_FrontMaterial.shininess) , 0.0);
			
			diffuse_color += gl_LightSource[7].diffuse*kdiff*attenuation;
			specular_color += gl_LightSource[7].specular*kspec*attenuation;
			ambient_color += gl_LightSource[7].ambient*attenuation;
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