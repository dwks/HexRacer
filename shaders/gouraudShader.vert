// Color map, normal map and glow map with gouraud shading + phong shading specularity

varying vec4 vertexColor;

uniform int numLights;

void main()
{	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	vec3 eyeNormal = normalize(gl_NormalMatrix * gl_Normal);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	vec4 position = gl_ModelViewMatrix * gl_Vertex;
	
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

}
