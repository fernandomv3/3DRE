#version 330

in vec4 fNormal;
in vec4 fWorldPosition;
in vec2 fUv;
in vec4 fTangent;
in vec4 fDepthMapPosition;
layout(location = 0) out vec4 color;
uniform float gamma;
uniform float time;
uniform int screenWidth;
uniform int screenHeight;
uniform sampler2D normalMap;
uniform sampler2D colorMap;
uniform sampler2D fbdepthMap;
uniform vec4 ambient;

struct Material{
	vec4 diffuse;
	vec4 specular;
	float shininess;
};
struct Light{
	vec4 color;
	vec4 position;
	float attenuation;
	float intensity;
	mat4 depthMatrix;
};

float warp (in float value,in float factor){
	return (value + factor ) / (1+ clamp(factor,0,1));
}

float calculateCosAngIncidence(in vec4 direction, in vec4 normal,in float warpFactor){
	//use warpFactor = 0 for no warp
	float cosAngIncidence = dot(normal,direction);
	cosAngIncidence = warp(cosAngIncidence,warpFactor);
	cosAngIncidence = clamp(cosAngIncidence,0,1);
	return cosAngIncidence;
}

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

float calculateShadowFactor(in vec4 lightSpacePos){
  vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
  vec3 uv = 0.5 * projCoords + 0.5;
  if(uv.x > 1.0 || uv.y > 1.0 || uv.x < 0.0 || uv.y < 0.0) return 1.0;
  float depth = texture(fbdepthMap,uv.xy).x;
  if (depth < uv.z + 0.00001){
    return 0.3;
  }
  return 1.0;
}

uniform Material material;
uniform Light light;

void main(){
	vec4 normal = calculateNormal(fNormal,fTangent,fUv,normalMap);
	vec4 diffuseColor = texture(colorMap,fUv);
	float cosAng = calculateCosAngIncidence(normalize(light.position),normalize(normal),0.0);
	float shadowFactor = calculateShadowFactor(fDepthMapPosition);
	color = /*shadowFactor */ light.color * diffuseColor *cosAng;
	//color = pow(color,vec4(1/gamma,1/gamma,1/gamma,1.0));
}