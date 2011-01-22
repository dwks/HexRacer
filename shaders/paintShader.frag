//Drawing paint cells

//varying vec3 eyeNormal;
varying vec4 position;
varying float kOrient;

uniform vec4 playerColor;

void main() {
	gl_FragColor = playerColor * kOrient;
}