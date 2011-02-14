varying vec3 eyeNormal;
varying vec4 position;

varying vec4 shadowPosition;

void main()
{	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	position = gl_ModelViewMatrix * gl_Vertex;
	
	shadowPosition = gl_TextureMatrix[3] * gl_Vertex;
	
	eyeNormal = normalize(gl_NormalMatrix * gl_Normal);
}