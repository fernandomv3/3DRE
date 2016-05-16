#version 330

in vec4 fNormal;
layout(location = 0) out vec4 color;
uniform float gamma;
uniform float time;

void main(){
  vec3 col = vec3(sin(time),cos(time),0.0);
  col = (col*0.5) + 0.5;
  color = pow((normalize(fNormal)*0.5)+0.5,vec4(gamma,gamma,gamma,1.0));
}