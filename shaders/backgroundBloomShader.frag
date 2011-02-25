varying vec4 position;
varying mat3 cameraNormalMatrix;

uniform samplerCube cubeMap;

void main() {
	vec3 worldView = normalize(cameraNormalMatrix * position.xyz);
	//worldView.x = -worldView.x;
	gl_FragColor = textureCube(cubeMap, worldView);
	float bright_factor = pow( max( max(gl_FragColor.x, gl_FragColor.y), gl_FragColor.z), 5.0 );
	gl_FragColor = gl_FragColor*bright_factor;
}