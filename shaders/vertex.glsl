#version 330

uniform mat4 worldMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalModelMatrix;

struct Light{
	vec4 color;
	vec4 position;
	float attenuation;
	float intensity;
	mat4 depthMatrix;
};

uniform Light light;

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUv;
layout(location = 3) in vec3 vTangent;

out vec4 fNormal;
out vec2 fUv;
out vec4 fWorldPosition;
out vec4 fTangent;
out vec4 fDepthMapPosition;

void main(){
  fNormal = worldMatrix * modelMatrix * vec4(vNormal,0.0);
  fTangent = worldMatrix * modelMatrix * vec4(vTangent,0.0);
  fUv = vUv;
  fDepthMapPosition = light.depthMatrix * modelMatrix * vec4(vPosition,1.0);
  fWorldPosition = worldMatrix * modelMatrix * vec4(vPosition,1.0);
  gl_Position =  projectionMatrix * worldMatrix * modelMatrix * vec4(vPosition,1.0);
}