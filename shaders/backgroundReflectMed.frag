// Background reflection with phong specularity

varying vec3 eyeNormal;
varying vec4 position;

varying vec4 light0diffuse;
varying vec4 vertexColor;
varying mat3 cameraNormalMatrix;

varying vec4 shadowPosition;

uniform samplerCube cubeMap;

uniform sampler2D colorMap;
uniform sampler2D shadowMap;
uniform int hasTexture [4];
uniform int numLights;

void main() {

	vec3 view = normalize(position.xyz);
	vec3 normal = eyeNormal;
	vec3 reflection = normalize(reflect(view, normal));
	vec3 worldReflect = normalize(cameraNormalMatrix * reflection);
	
	//Specular calculations----------------------------------------------------------------------------------------
	
	float light0factor = 0.0;
	vec4 specular_color = vec4(0.0, 0.0, 0.0, 0.0);
	float light_dist;
	float attenuation;
	vec3 light;
	float kspec;
	
	float distanceFromLight = 1.0;
	vec4 shadowCoordinateWdivide = vec4(0.0, 0.0, 0.0, 0.0);
	if (hasTexture[3] == 1) {
		shadowCoordinateWdivide = shadowPosition / shadowPosition.w;
		shadowCoordinateWdivide.z -= 0.0005;
		distanceFromLight = texture2D(shadowMap, shadowCoordinateWdivide.xy).z;
	}
	
	if (numLights > 0 && distanceFromLight > shadowCoordinateWdivide.z ) {
		light_dist = length((position-gl_LightSource[0].position).xyz);
		attenuation = min(1.0/(gl_LightSource[0].constantAttenuation + gl_LightSource[0].quadraticAttenuation*light_dist*light_dist), 1.0);
		if (attenuation >= 0.004) {
			light = normalize((position-gl_LightSource[0].position).xyz);
			kspec =-dot(reflection,light);
			kspec = max(pow(kspec, gl_FrontMaterial.shininess), 0.0);
			specular_color += gl_LightSource[0].specular*kspec*attenuation;
		}
		light0factor = 1.0;
	}
	
	if (numLights > 1) {
		light_dist = length((position-gl_LightSource[1].position).xyz);
		attenuation = min(1.0/(gl_LightSource[1].constantAttenuation + gl_LightSource[1].quadraticAttenuation*light_dist*light_dist), 1.0);
		if (attenuation >= 0.004) {
			light = normalize((position-gl_LightSource[1].position).xyz);
			kspec =-dot(reflection,light);
			kspec = max(pow(kspec, gl_FrontMaterial.shininess), 0.0);
			specular_color += gl_LightSource[1].specular*kspec*attenuation;
		}
	}
	
	if (numLights > 2) {
		light_dist = length((position-gl_LightSource[0].position).xyz);
		attenuation = min(1.0/(gl_LightSource[0].constantAttenuation + gl_LightSource[0].quadraticAttenuation*light_dist*light_dist), 1.0);
		if (attenuation >= 0.004) {
			light = normalize((position-gl_LightSource[0].position).xyz);
			kspec =-dot(reflection,light);
			kspec = max(pow(kspec, gl_FrontMaterial.shininess), 0.0);
			specular_color += gl_LightSource[0].specular*kspec*attenuation;
		}
	}
	if (numLights > 3) {
		light_dist = length((position-gl_LightSource[3].position).xyz);
		attenuation = min(1.0/(gl_LightSource[3].constantAttenuation + gl_LightSource[3].quadraticAttenuation*light_dist*light_dist), 1.0);
		if (attenuation >= 0.004) {
			light = normalize((position-gl_LightSource[3].position).xyz);
			kspec =-dot(reflection,light);
			kspec = max(pow(kspec, gl_FrontMaterial.shininess), 0.0);
			specular_color += gl_LightSource[3].specular*kspec*attenuation;
		}
	}
	
	vec4 diffuse_base;
	//Use the texture color as diffuse base if it exists
	if (hasTexture[0] == 1)
		diffuse_base = texture2D(colorMap, gl_TexCoord[0].st);
	else
		diffuse_base = gl_FrontMaterial.diffuse;
	
	float krefl = (gl_FrontMaterial.shininess-1.0)/12.0;

	gl_FragColor = ((vertexColor+light0diffuse*light0factor) * diffuse_base)*(1.0-krefl) + textureCube(cubeMap, worldReflect)*krefl + (specular_color * gl_FrontMaterial.specular);
}