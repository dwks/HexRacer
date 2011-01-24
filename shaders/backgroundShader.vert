//Static cube map seen at infinity

varying vec4 position;
varying mat3 cameraNormalMatrix;

uniform mat4 cameraMatrix;

void main()
{	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	position = gl_ModelViewMatrix * gl_Vertex;
	
	cameraNormalMatrix[0][0] = -cameraMatrix[0][0];
	cameraNormalMatrix[0][1] = cameraMatrix[0][1];
	cameraNormalMatrix[0][2] = cameraMatrix[0][2];
	cameraNormalMatrix[1][0] = -cameraMatrix[1][0];
	cameraNormalMatrix[1][1] = cameraMatrix[1][1];
	cameraNormalMatrix[1][2] = cameraMatrix[1][2];
	cameraNormalMatrix[2][0] = cameraMatrix[2][0];
	cameraNormalMatrix[2][1] = -cameraMatrix[2][1];
	cameraNormalMatrix[2][2] = -cameraMatrix[2][2];
	
	//vec3 cameraPos = vec3(cameraMatrix[3][0], cameraMatrix[3][1], cameraMatrix[3][2]);

}
