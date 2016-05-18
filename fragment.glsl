#version 330

in vec4 fNormal;
layout(location = 0) out vec4 color;
uniform float gamma;
uniform float time;
uniform int screenWidth;
uniform int screenHeight;

struct Material{
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform Material material;

void main(){
  vec3 col = vec3(sin(time),cos(time),0.0);
  //col = pow(col,vec3(1/gamma));
  col = (col*0.5) + 0.5;
  //color = pow((normalize(fNormal)*0.5)+0.5,vec4(gamma,gamma,gamma,1.0));
  color = vec4(gl_FragCoord.x/screenWidth,gl_FragCoord.y/screenHeight,0.0,0.0);
  color = material.diffuse;
}