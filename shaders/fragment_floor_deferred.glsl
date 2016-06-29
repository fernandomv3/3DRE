#version 330

in vec4 fNormal;
in vec4 fWorldPosition;
in vec2 fUv;
in vec4 fTangent;
layout(location = 0) out vec4 position;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 diffuse;
layout(location = 3) out vec4 specular;

uniform sampler2D normalMap;
uniform sampler2D colorMap;
uniform mat4 projectionMatrix;

struct Material{
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

float linearizeDepth(in float depth, in mat4 projMatrix) {
  return projMatrix[3][2] / (depth - projMatrix[2][2]);
}

vec4 calculateNormal(in vec4 vNormal,in vec4 vTangent, in vec2 uv ,in sampler2D normalMap){
  vec3 normal = normalize(vNormal).xyz;
  vec3 tangent = normalize(vTangent).xyz;
  tangent = normalize(tangent - dot(tangent,normal) * normal);
  vec3 bitangent = -cross(tangent,normal);
  vec3 texNormal = 2.0* texture(normalMap,uv).xyz  - vec3(1.0,1.0,1.0);
  mat3 TBN = mat3(tangent, bitangent, normal);
  texNormal = TBN * texNormal;
  texNormal = normalize(texNormal);
  return vec4(texNormal,0.0);
}
const vec2 scale = vec2(10.0/2.0);
uniform Material material;

void main(){
	position = fWorldPosition;
	normal = calculateNormal(fNormal,fTangent,fUv*scale,normalMap);
  normal = 2.0* texture(normalMap,fUv *scale)  - vec4(1.0);
	diffuse = texture(colorMap,fUv*scale);
	specular = material.specular;
	specular.w = material.shininess;
}