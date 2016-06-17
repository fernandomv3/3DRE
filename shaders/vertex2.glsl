#version 330

layout(location = 0) in vec3 vPosition;
layout(location = 2) in vec2 vUv;

out vec2 fUv;

void main(){
  fUv = vUv;
  gl_Position = vec4(vPosition,1.0);
}