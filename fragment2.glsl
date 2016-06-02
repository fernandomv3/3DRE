#version 330

in vec2 fUv;
out vec4 color;
uniform sampler2D fbcolor;

void main(){
	vec4 diffuseColor = texture(fbcolor,fUv);
	color = diffuseColor;
}