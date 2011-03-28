varying vec4 position;uniform sampler2D glowMap;uniform int hasTexture [4];void main() {		if (hasTexture[2] == 1)		gl_FragColor = texture2D(glowMap, gl_TexCoord[0].st);	else		gl_FragColor = gl_FrontMaterial.ambient;			float depth_factor = max(pow(max(abs(position.z)-30.0, 0.0), 2.0)*0.01, 1.0);	float bright_factor = pow( max( max(gl_FragColor.x, gl_FragColor.y), gl_FragColor.z), 2.5 );	gl_FragColor = gl_FragColor*bright_factor/depth_factor;	gl_FragColor.w = 1.0;}