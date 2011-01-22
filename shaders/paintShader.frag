//Drawing paint cells

varying vec3 eyeNormal;
varying vec4 position;

uniform vec4 playerColor;

void main() {

	vec3 view = normalize(position.xyz);

	float korient = abs(dot(view, eyeNormal));
	korient = pow(korient, 0.5);
	
	gl_FragColor = playerColor * korient;

}