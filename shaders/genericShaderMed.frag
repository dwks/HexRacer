// Color map, normal map and glow map with gouraud shading + phong shading specularity

varying vec3 eyeNormal;
varying vec3 objectNormal;
varying vec4 position;

varying vec3 eyeTangent; 
varying vec3 eyeBitangent;

varying vec4 vertexColor;

uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D glowMap;
uniform int hasTexture [3];
uniform int numLights;

void main() {

	vec3 normal;
	if (hasTexture[1] == 1) {
		//Use normal map
		vec3 nmap = (texture2D(normalMap, gl_TexCoord[0].st).xyz) * 2.0 - 1.0;
		normal = normalize( eyeTangent*(-nmap.x) + eyeBitangent*nmap.y + eyeNormal*nmap.z );
	}
	else
		normal = eyeNormal;
	
	vec3 view = normalize(position.xyz);
	vec3 reflection = normalize(reflect(view, normal));
	
	vec4 specular_color = vec4(0.0, 0.0, 0.0, 0.0);
	
	//Specular calculations----------------------------------------------------------------------------------------
	
	if (numLights > 0) {
		float light_dist = length((position-gl_LightSource[0].position).xyz);
		
		float attenuation = 1.0/(gl_LightSource[0].constantAttenuation + gl_LightSource[0].quadraticAttenuation*light_dist*light_dist);
		if (attenuation >= 0.004) {
		
			attenuation = min(attenuation, 1.0);
			vec3 light = normalize((position-gl_LightSource[0].position).xyz);
			
			float kspec =-dot(reflection,light);
			kspec = max(kspec, 0.0);
			kspec = pow(kspec, gl_FrontMaterial.shininess);
			
			specular_color += gl_LightSource[0].specular*kspec*attenuation;
		}
	}
	
	if (numLights > 1) {
		float light_dist = length((position-gl_LightSource[1].position).xyz);
		
		float attenuation = 1.0/(gl_LightSource[1].constantAttenuation + gl_LightSource[1].quadraticAttenuation*light_dist*light_dist);
		if (attenuation >= 0.004) {
		
			attenuation = min(attenuation, 1.0);
			vec3 light = normalize((position-gl_LightSource[1].position).xyz);
			
			float kspec =-dot(reflection,light);
			kspec = max(kspec, 0.0);
			kspec = pow(kspec, gl_FrontMaterial.shininess);
			
			specular_color += gl_LightSource[1].specular*kspec*attenuation;
		}
	}

	//Apply Material----------------------------------------------------------------------------------------

	vec4 diffuse_base;
	//Use the texture color as diffuse base if it exists
	if (hasTexture[0] == 1)
		diffuse_base = texture2D(colorMap, gl_TexCoord[0].st);
	else
		diffuse_base = gl_FrontMaterial.diffuse;
	
	specular_color *= gl_FrontMaterial.specular;
	
	if (hasTexture[1] == 1)
		specular_color *= texture2D(normalMap, gl_TexCoord[0].st).w;
	
	vec4 ambient_base;
	if (hasTexture[2] == 1)
		ambient_base = texture2D(glowMap, gl_TexCoord[0].st);
	else
		ambient_base = gl_FrontMaterial.ambient;
	
	gl_FragColor = specular_color + vertexColor*diffuse_base;
	gl_FragColor.x += (1.0-gl_FragColor.x)*ambient_base.x;
	gl_FragColor.y += (1.0-gl_FragColor.y)*ambient_base.y;
	gl_FragColor.z += (1.0-gl_FragColor.z)*ambient_base.z;
}
