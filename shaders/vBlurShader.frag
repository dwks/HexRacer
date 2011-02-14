varying vec4 position;

uniform sampler2D colorMap;
uniform int viewHeight;

void main() {

	vec2 middle_pos = vec2(position.x*0.5+0.5, position.y*0.5+0.5);
	
	float offset = 1.0/float(viewHeight)*1.25;
	
	vec2 bottom_pos = middle_pos;
	bottom_pos.y -= offset;
	
	vec2 top_pos = middle_pos;
	top_pos.y += offset;
	
	gl_FragColor = texture2D(colorMap, middle_pos)*0.375 + texture2D(colorMap, bottom_pos)*0.3125 + texture2D(colorMap, top_pos)*0.3125;

}