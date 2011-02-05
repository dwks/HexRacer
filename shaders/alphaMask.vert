varying vec4 position;
varying vec4 vertexColor;

void main() {

	gl_TexCoord[0] = gl_MultiTexCoord0;

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	position = gl_ModelViewMatrix * gl_Vertex;
	
	vertexColor = gl_Color;
}