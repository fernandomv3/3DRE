#version 330

uniform mat4 worldMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalModelMatrix;

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUv;

out vec4 fNormal;
out vec2 fUv;

void main(){
  fNormal = worldMatrix * normalModelMatrix * vec4(vNormal,0.0);
  fUv = vUv;
  gl_Position =  projectionMatrix * worldMatrix * modelMatrix *vec4(vPosition,1.0);
}