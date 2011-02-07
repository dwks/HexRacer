varying vec4 position;
varying vec4 vertexColor;

uniform int usingTexture;
uniform sampler2D renderTexture;
uniform sampler2D alphaMaskTexture;
uniform float viewSize[2];

void main() {

	gl_FragColor = vertexColor;
	
	if (usingTexture == 1)
		gl_FragColor *= texture2D(renderTexture, gl_TexCoord[0].st);
		
	gl_FragColor.w *= texture2D(alphaMaskTexture, vec2( position.x/(viewSize[0])+0.5 , 1.0-(position.y/(viewSize[1])+0.5) )).w;
}