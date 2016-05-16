#version 330

in vec4 fNormal;
layout(location = 0) out vec4 color;
uniform float gamma;
uniform float time;

void main(){
  vec3 col = vec3(sin(time),cos(time),(sin(time)+cos(time))/2);
  col = (col*0.5) + 0.5;
  color = pow(vec4(col,1.0),vec4(gamma,gamma,gamma,1.0));
}