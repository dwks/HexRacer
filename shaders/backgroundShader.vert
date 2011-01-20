// basic vertex shader

varying vec3 eyeNormal;
varying vec3 objectNormal;
varying vec4 position;

void main()
{	

	eyeNormal = normalize(gl_NormalMatrix * gl_Normal);
	objectNormal = normalize(gl_Normal);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	position = gl_ModelViewMatrix * gl_Vertex;

}
