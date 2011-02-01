//Drawing paint cells

//varying vec3 eyeNormal;
varying vec4 position;
varying float kOrient;

varying vec4 vertColor;

void main()
{
	vec3 eyeNormal = normalize(gl_NormalMatrix * gl_Normal);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	position = gl_ModelViewMatrix * gl_Vertex;
	
	vec3 view = normalize(position.xyz);
	kOrient = abs(dot(view, eyeNormal));
	kOrient *= 1.5;
	kOrient = pow(kOrient, 0.5);
	kOrient = min(kOrient, 1.0);
	
	vertColor = gl_Color*kOrient;

}
