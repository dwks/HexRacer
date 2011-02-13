//Drawing paint cells

varying vec4 vertColor;
uniform float fadePlanes [2];

void main()
{
	vec3 eyeNormal = normalize(gl_NormalMatrix * gl_Normal);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	vec4 position = gl_ModelViewMatrix * gl_Vertex;
	
	vec3 view = normalize(position.xyz);
	float kOrient = abs(dot(view, eyeNormal));
	kOrient *= 1.5;
	kOrient = pow(kOrient, 0.5);
	kOrient = min(kOrient, 1.0);
	
	vertColor = gl_Color*kOrient;
	
	float depth = abs(position.z);
	
	vertColor.w = gl_Color.w;
	
	if (depth > fadePlanes[0] && fadePlanes[1] != fadePlanes[0])
		vertColor.w *= 1.0-(depth-fadePlanes[0])/(fadePlanes[1]-fadePlanes[0]);
		
}
