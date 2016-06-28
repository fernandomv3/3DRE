#version 330

in vec4 fNormal;
in vec4 fWorldPosition;
layout(location = 0) out vec4 position;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 diffuse;
layout(location = 3) out vec4 specular;

struct Material{
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform Material material;
uniform mat4 projectionMatrix;

float linearizeDepth(in float depth, in mat4 projMatrix) {
  return projMatrix[3][2] / (depth - projMatrix[2][2]);
}

void main(){
	position = fWorldPosition;
	normal = normalize(fNormal);
	diffuse = material.diffuse;
	specular = material.specular;
	specular.w = material.shininess;
}