//Static view independent cube map

varying vec3 eyeNormal;
varying vec3 objectNormal;
varying vec4 position;

uniform samplerCube cubeMap;
uniform mat4 cameraMatrix;

void main() {

	vec3 view = normalize(position.xyz);
	
	mat3 cameraNormalMatrix;
	cameraNormalMatrix[0][0] = cameraMatrix[0][0];
	cameraNormalMatrix[1][0] = cameraMatrix[1][0];
	cameraNormalMatrix[2][0] = cameraMatrix[2][0];
	cameraNormalMatrix[0][1] = cameraMatrix[0][1];
	cameraNormalMatrix[1][1] = cameraMatrix[1][1];
	cameraNormalMatrix[2][1] = cameraMatrix[2][1];
	cameraNormalMatrix[0][2] = cameraMatrix[0][2];
	cameraNormalMatrix[1][2] = cameraMatrix[1][2];
	cameraNormalMatrix[2][2] = cameraMatrix[2][2];
	//vec3 cameraPos = vec3(cameraMatrix[3][0], cameraMatrix[3][1], cameraMatrix[3][2]);
	vec3 worldView = normalize(cameraNormalMatrix * view);
	
	gl_FragColor = textureCube(cubeMap, worldView);
}