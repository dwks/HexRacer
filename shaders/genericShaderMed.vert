// Color map, normal map and glow map with gouraud shading + phong shading specularity

varying vec3 eyeNormal;
varying vec4 position;

varying vec3 eyeTangent; 
varying vec3 eyeBitangent;

varying vec4 light0diffuse;
varying vec4 vertexColor;

varying vec4 shadowPosition;

attribute vec3 tangent;
attribute vec3 bitangent;

uniform int numLights;

void main()
{	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	eyeNormal = normalize(gl_NormalMatrix * gl_Normal);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	position = gl_ModelViewMatrix * gl_Vertex;
	
	eyeTangent = normalize(gl_NormalMatrix * tangent);
	eyeBitangent = normalize(gl_NormalMatrix * bitangent);
	
	shadowPosition = gl_TextureMatrix[3] * gl_Vertex;
	
	//Calculate Diffuse and Ambient Lighting----------------------------------------------------------
	
	light0diffuse = vec4(0.0, 0.0, 0.0, 0.0);
	vertexColor = vec4(0.0, 0.0, 0.0, 0.0);
	float light_dist;
	float attenuation;
	vec3 light;
	float kdiff;
	
	if (numLights > 0) {
		light_dist = length((position-gl_LightSource[0].position).xyz);
		attenuation = min(1.0/(gl_LightSource[0].constantAttenuation + gl_LightSource[0].quadraticAttenuation*light_dist*light_dist), 1.0);
		if (attenuation >= 0.004) {
			light = normalize((position-gl_LightSource[0].position).xyz);
			kdiff = max(-dot(light, eyeNormal), 0.0);
			light0diffuse = gl_LightSource[0].diffuse*kdiff*attenuation;
			vertexColor += (gl_LightSource[0].ambient) *attenuation;
		}
	}
	if (numLights > 1) {
		light_dist = length((position-gl_LightSource[1].position).xyz);
		attenuation = min(1.0/(gl_LightSource[1].constantAttenuation + gl_LightSource[1].quadraticAttenuation*light_dist*light_dist), 1.0);
		if (attenuation >= 0.004) {
			light = normalize((position-gl_LightSource[1].position).xyz);
			kdiff = max(-dot(light, eyeNormal), 0.0);
			vertexColor += (gl_LightSource[1].diffuse*kdiff + gl_LightSource[1].ambient) *attenuation;
		}
	}
	if (numLights > 2) {
		light_dist = length((position-gl_LightSource[2].position).xyz);
		attenuation = min(1.0/(gl_LightSource[2].constantAttenuation + gl_LightSource[2].quadraticAttenuation*light_dist*light_dist), 1.0);
		if (attenuation >= 0.004) {
			light = normalize((position-gl_LightSource[2].position).xyz);
			kdiff = max(-dot(light, eyeNormal), 0.0);
			vertexColor += (gl_LightSource[2].diffuse*kdiff + gl_LightSource[2].ambient) *attenuation;
		}
	}
	if (numLights > 3) {
		light_dist = length((position-gl_LightSource[3].position).xyz);
		attenuation = min(1.0/(gl_LightSource[3].constantAttenuation + gl_LightSource[3].quadraticAttenuation*light_dist*light_dist), 1.0);
		if (attenuation >= 0.004) {
			light = normalize((position-gl_LightSource[3].position).xyz);
			kdiff = max(-dot(light, eyeNormal), 0.0);
			vertexColor += (gl_LightSource[3].diffuse*kdiff + gl_LightSource[3].ambient) *attenuation;
		}
	}

}
