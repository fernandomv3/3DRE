#version 330

layout(location = 0) out vec4 color;
uniform float gamma;

void main(){
  float x = gl_FragCoord.x/800.0;
  float y = gl_FragCoord.y/600.0;
  color = pow(vec4(1-x,1-y,(x+y)/2,1.0),vec4(gamma,gamma,gamma,1.0));
}