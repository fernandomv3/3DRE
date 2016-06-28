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
uniform sampler2D specularMap;
uniform mat4 projectionMatrix;

struct Material{
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

vec4 calculateNormal(in vec4 vNormal,in vec4 vTangent, in vec2 uv ,in sampler2D normalMap){
  vec3 normal = normalize(vNormal).xyz;
  vec3 tangent = normalize(vTangent).xyz;
  tangent = normalize(tangent - dot(tangent,normal) * normal);
  vec3 bitangent = -cross(tangent,normal);
  vec3 texNormal = 255 * texture(normalMap,uv).xyz /128 - vec3(1.0,1.0,1.0);
  mat3 TBN = mat3(tangent, bitangent, normal);
  texNormal = TBN * texNormal;
  texNormal = normalize(texNormal);
  return vec4(texNormal,0.0);
}

float linearizeDepth(in float depth, in mat4 projMatrix) {
  return projMatrix[3][2] / (depth - projMatrix[2][2]);
}

uniform Material material;

void main(){
	position = fWorldPosition;
	normal = calculateNormal(fNormal,fTangent,fUv,normalMap);
	diffuse = material.diffuse;
	specular = texture(specularMap,fUv);
	specular.w = material.shininess;
}