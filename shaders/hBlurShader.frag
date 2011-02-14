varying vec4 position;

uniform sampler2D colorMap;
uniform int viewWidth;

void main() {

	vec2 middle_pos = vec2(position.x*0.5+0.5, position.y*0.5+0.5);
	
	float offset = 1.0/float(viewWidth)*1.25;
	
	vec2 left_pos = middle_pos;
	left_pos.x -= offset;
	
	vec2 right_pos = middle_pos;
	right_pos.x += offset;
	
	gl_FragColor = texture2D(colorMap, middle_pos)*0.375 + texture2D(colorMap, left_pos)*0.3125 + texture2D(colorMap, right_pos)*0.3125;

}