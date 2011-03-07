varying vec3 eyeNormal;varying vec4 position;varying vec3 eyeTangent; varying vec3 eyeBitangent;varying vec4 shadowPosition;uniform sampler2D normalMap;uniform sampler2D glowMap;uniform sampler2D shadowMap;uniform int hasTexture [4];uniform int numLights;void main() {	vec3 normal;	if (hasTexture[1] == 1) {		vec3 nmap = (texture2D(normalMap, gl_TexCoord[0].st).xyz) * 2.0 - 1.0;		normal = normalize( eyeTangent*(-nmap.x) + eyeBitangent*nmap.y + eyeNormal*nmap.z );	}	else {		normal = eyeNormal;	}		if (hasTexture[2] == 1)		gl_FragColor = texture2D(glowMap, gl_TexCoord[0].st);	else		gl_FragColor = gl_FrontMaterial.ambient;			vec3 view = normalize(position.xyz);	vec3 reflection = normalize(reflect(view, normal));			float distanceFromLight = 1.0;	vec4 shadowCoordinateWdivide = vec4(0.0, 0.0, 0.0, 0.0);	if (hasTexture[3] == 1) {		shadowCoordinateWdivide = shadowPosition / shadowPosition.w;		shadowCoordinateWdivide.z = min(shadowCoordinateWdivide.z, 1.0);		shadowCoordinateWdivide.z -= 0.00001;		distanceFromLight = texture2D(shadowMap, shadowCoordinateWdivide.xy).z;	}		vec4 specular_color = vec4(0.0, 0.0, 0.0, 0.0);		if (numLights > 0 && distanceFromLight > shadowCoordinateWdivide.z ) {		float light_dist = length((position-gl_LightSource[0].position).xyz);		float attenuation = min(1.0/(gl_LightSource[0].constantAttenuation + gl_LightSource[0].quadraticAttenuation*light_dist*light_dist), 1.0);		if (attenuation >= 0.004) {			vec3 light = normalize((position-gl_LightSource[0].position).xyz);						float kspec =-dot(reflection,light);			kspec = max( pow(kspec, gl_FrontMaterial.shininess)*float(dot(light, eyeNormal) < 0.0) , 0.0);						specular_color += gl_LightSource[0].specular*kspec*attenuation;		}	}		if (numLights > 1) {		float light_dist = length((position-gl_LightSource[1].position).xyz);		float attenuation = min(1.0/(gl_LightSource[1].constantAttenuation + gl_LightSource[1].quadraticAttenuation*light_dist*light_dist), 1.0);		if (attenuation >= 0.004) {			vec3 light = normalize((position-gl_LightSource[1].position).xyz);						float kspec =-dot(reflection,light);			kspec = max( pow(kspec, gl_FrontMaterial.shininess)*float(dot(light, eyeNormal) < 0.0) , 0.0);						specular_color += gl_LightSource[1].specular*kspec*attenuation;		}	}		specular_color *= gl_FrontMaterial.specular;		if (hasTexture[1] == 1) {		specular_color *= texture2D(normalMap, gl_TexCoord[0].st).w;	}		gl_FragColor += specular_color;			float depth_factor = max(pow(max(abs(position.z)-30.0, 0.0), 2.0)*0.0075, 1.0);	float bright_factor = pow( max( max(gl_FragColor.x, gl_FragColor.y), gl_FragColor.z), 2.5 );	gl_FragColor = gl_FragColor*bright_factor/depth_factor;}