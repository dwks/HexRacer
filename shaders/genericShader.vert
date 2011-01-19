// basic vertex shader

varying vec3 eyeNormal;
varying vec3 objectNormal;
varying vec4 position;

varying vec3 eyeTangent; 
varying vec3 eyeBitangent;

attribute vec3 tangent;
attribute vec3 bitangent;

void main()
{	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	eyeNormal = normalize(gl_NormalMatrix * gl_Normal);
	objectNormal = normalize(gl_Normal);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	position = gl_ModelViewMatrix * gl_Vertex;
	
	eyeTangent = normalize(gl_NormalMatrix * tangent);
	eyeBitangent = normalize(gl_NormalMatrix * bitangent);


}