#version 330

uniform mat4 worldMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
layout(location = 0) in vec3 vPosition;

void main(){
  gl_Position =  projectionMatrix * worldMatrix * modelMatrix *vec4(vPosition,1.0);
}