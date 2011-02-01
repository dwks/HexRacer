// Background reflection with phong specularity

varying vec3 eyeNormal;
varying vec3 objectNormal;
varying vec4 position;

//varying vec3 eyeTangent; 
//varying vec3 eyeBitangent;

varying vec4 vertexColor;
//varying vec4 diffuseColor;
//varying vec4 ambientColor;

varying mat3 cameraNormalMatrix;

uniform samplerCube cubeMap;

uniform sampler2D colorMap;
//uniform sampler2D normalMap;
uniform int hasTexture [3];
uniform int numLights;

void main() {

	vec3 view = normalize(position.xyz);
	vec3 normal;
	//if (hasTexture[1] == 1) {
	//	vec3 nmap = (texture2D(normalMap, gl_TexCoord[0].st).xyz) * 2.0 - 1.0;
	//	normal = normalize( eyeTangent*(-nmap.x) + eyeBitangent*nmap.y + eyeNormal*nmap.z );
	//}
	//else
		normal = eyeNormal;
	vec3 reflection = normalize(reflect(view, normal));
	vec3 worldReflect = normalize(cameraNormalMatrix * reflection);
	
	//vec4 diffuse_color = diffuseColor;
	vec4 specular_color = vec4(0.0, 0.0, 0.0, 0.0);
	//vec4 ambient_color = ambientColor;
	
	//Specular calculations----------------------------------------------------------------------------------------
	
	if (numLights > 0) {
		float light_dist = length((position-gl_LightSource[0].position).xyz);
		float attenuation = min(1.0/(gl_LightSource[0].constantAttenuation + gl_LightSource[0].quadraticAttenuation*light_dist*light_dist), 1.0);
		if (attenuation >= 0.004) {
			vec3 light = normalize((position-gl_LightSource[0].position).xyz);
			float kspec =-dot(reflection,light);
			kspec = max(pow(kspec, gl_FrontMaterial.shininess),0.0);
			specular_color += gl_LightSource[0].specular*kspec*attenuation;
		}
	}
	
	vec4 diffuse_base;
	//Use the texture color as diffuse base if it exists
	if (hasTexture[0] == 1)
		diffuse_base = texture2D(colorMap, gl_TexCoord[0].st);
	else
		diffuse_base = gl_FrontMaterial.diffuse;
	
	//diffuse_color *= diffuse_base;
	specular_color *= gl_FrontMaterial.specular;
	//ambient_color *= diffuse_base;
	
	float krefl = (gl_FrontMaterial.shininess-1.0)/12.0;
	//if (hasTexture[1] == 1) {
	//	float shine_factor = texture2D(normalMap, gl_TexCoord[0].st).w;
	//	specular_color *= shine_factor;
	//	krefl *= shine_factor;
	//}
	
	gl_FragColor = (vertexColor * diffuse_base)*(1.0-krefl) + textureCube(cubeMap, worldReflect)*krefl + specular_color;
}