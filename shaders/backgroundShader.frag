//Static cube map seen at infinity

varying vec4 position;
varying mat3 cameraNormalMatrix;

uniform samplerCube cubeMap;

void main() {
	vec3 worldView = normalize(cameraNormalMatrix * position.xyz);
	gl_FragColor = textureCube(cubeMap, worldView);
}