// basic vertex shader

varying vec3 eyeNormal;
varying vec3 objectNormal;
varying vec4 position;

varying vec3 eyeTangent; 
varying vec3 eyeBitangent;

varying vec4 diffuseColor;
varying vec4 ambientColor;

attribute vec3 tangent;
attribute vec3 bitangent;

uniform int numLights;

void main()
{	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	eyeNormal = normalize(gl_NormalMatrix * gl_Normal);
	objectNormal = normalize(gl_Normal);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	position = gl_ModelViewMatrix * gl_Vertex;
	
	eyeTangent = normalize(gl_NormalMatrix * tangent);
	eyeBitangent = normalize(gl_NormalMatrix * bitangent);
	
	//Calculate Diffuse and Ambient Lighting-------------------------------------------------------===
	diffuseColor = vec4(0.0, 0.0, 0.0, 1.0);
	ambientColor = vec4(0.0, 0.0, 0.0, 0.0);
	
	if (numLights > 0) {
		float light_dist = length((position-gl_LightSource[0].position).xyz);
		float attenuation = 1.0/(1.0 + gl_LightSource[0].quadraticAttenuation*light_dist*light_dist);
		if (attenuation >= 0.001) {
		
			attenuation = min(attenuation, 1.0);
			vec3 light = normalize((position-gl_LightSource[0].position).xyz);
			
			float kdiff = -dot(light, eyeNormal);
			kdiff = max(kdiff, 0.0);
			
			diffuseColor += gl_LightSource[0].diffuse*kdiff*attenuation;
			ambientColor += gl_LightSource[0].ambient*attenuation;
		}
	}
	if (numLights > 1) {
		float light_dist = length((position-gl_LightSource[1].position).xyz);
		float attenuation = 1.0/(1.0 + gl_LightSource[1].quadraticAttenuation*light_dist*light_dist);
		if (attenuation >= 0.001) {
		
			attenuation = min(attenuation, 1.0);
			vec3 light = normalize((position-gl_LightSource[1].position).xyz);
			
			float kdiff = -dot(light, eyeNormal);
			kdiff = max(kdiff, 0.0);
			
			diffuseColor += gl_LightSource[1].diffuse*kdiff*attenuation;
			ambientColor += gl_LightSource[1].ambient*attenuation;
		}
	}

	


}
