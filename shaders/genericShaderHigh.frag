//basic fragment shader

varying vec3 eyeNormal;
varying vec3 objectNormal;
varying vec4 position;

varying vec3 eyeTangent; 
varying vec3 eyeBitangent;

uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform int hasTexture [3];
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
	
	vec4 diffuse_color = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 specular_color = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 ambient_color = vec4(0.0, 0.0, 0.0, 0.0);
	
	//Lighting calculations----------------------------------------------------------------------------------------
	
	for (int i = 0; i < numLights; i++) {
	
		float light_dist = length((position-gl_LightSource[i].position).xyz);
		
		float attenuation = 1.0/(1.0 + gl_LightSource[i].quadraticAttenuation*light_dist*light_dist);
		if (attenuation >= 0.001) {
		
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
	
	if (hasTexture[1] == 1) {
		specular_color *= texture2D(normalMap, gl_TexCoord[0].st).w;
	}
	
	vec4 ambient_base;
	ambient_base = gl_FrontMaterial.ambient;
	
	gl_FragColor = diffuse_color + specular_color + ambient_color;
	if (gl_FragColor.x < ambient_base.x)
		gl_FragColor.x += (1.0-gl_FragColor.x)*ambient_base.x;
	if (gl_FragColor.y < ambient_base.y)
		gl_FragColor.y += (1.0-gl_FragColor.y)*ambient_base.y;
	if (gl_FragColor.z < ambient_base.z)
		gl_FragColor.z += (1.0-gl_FragColor.z)*ambient_base.z;
}