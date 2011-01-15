// basic vertex shader

varying vec3 normal;
varying vec3 trueNormal;
varying vec4 position;

//attribute vec3 tangent;
//attribute vec3 bitangent;

void main()
{	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	normal = normalize(gl_NormalMatrix * gl_Normal);
	trueNormal = normalize(gl_Normal);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	position = gl_ModelViewMatrix * gl_Vertex;

}
