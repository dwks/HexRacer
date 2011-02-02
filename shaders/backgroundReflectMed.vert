// Background reflection with phong specularity

varying vec3 eyeNormal;
varying vec4 position;

//varying vec3 eyeTangent; 
//varying vec3 eyeBitangent;

varying vec4 vertexColor;
varying mat3 cameraNormalMatrix;

attribute vec3 tangent;
attribute vec3 bitangent;

uniform int numLights;
uniform mat4 cameraMatrix;

void main()
{	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	eyeNormal = normalize(gl_NormalMatrix * gl_Normal);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	position = gl_ModelViewMatrix * gl_Vertex;
	
	//Calculate Diffuse and Ambient Lighting----------------------------------------------------------
	
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
			vertexColor += (gl_LightSource[0].diffuse*kdiff + gl_LightSource[0].ambient) *attenuation;
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
	
	cameraNormalMatrix[0][0] = -cameraMatrix[0][0];
	cameraNormalMatrix[0][1] = cameraMatrix[0][1];
	cameraNormalMatrix[0][2] = cameraMatrix[0][2];
	cameraNormalMatrix[1][0] = -cameraMatrix[1][0];
	cameraNormalMatrix[1][1] = cameraMatrix[1][1];
	cameraNormalMatrix[1][2] = cameraMatrix[1][2];
	cameraNormalMatrix[2][0] = cameraMatrix[2][0];
	cameraNormalMatrix[2][1] = -cameraMatrix[2][1];
	cameraNormalMatrix[2][2] = -cameraMatrix[2][2];
}
