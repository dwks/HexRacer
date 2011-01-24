// Color map, normal map and glow map with phong shading

varying vec3 eyeNormal;
varying vec3 objectNormal;
varying vec4 position;

varying vec3 eyeTangent; 
varying vec3 eyeBitangent;

varying mat3 cameraNormalMatrix;

attribute vec3 tangent;
attribute vec3 bitangent;

uniform mat4 cameraMatrix;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	eyeNormal = normalize(gl_NormalMatrix * gl_Normal);
	objectNormal = normalize(gl_Normal);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	position = gl_ModelViewMatrix * gl_Vertex;
	
	eyeTangent = normalize(gl_NormalMatrix * tangent);
	eyeBitangent = normalize(gl_NormalMatrix * bitangent);
	
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
