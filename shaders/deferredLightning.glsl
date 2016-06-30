#version 330

in vec2 fUv;
layout(location = 0)out vec4 color;
uniform sampler2D fbposition;
uniform sampler2D fbnormal;
uniform sampler2D fbdiffuse;
uniform sampler2D fbspecular;
uniform int screenWidth;
uniform int screenHeight;
uniform float gamma;
uniform float time;
uniform sampler2D depthMap;
uniform vec4 ambient;
uniform mat4 inverseWorldMatrix;
uniform mat4 worldMatrix;
uniform sampler2D fbssao;

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

float calculateBlinnPhongTerm(in vec4 direction,vec4 normal, in vec4 viewDirection, in float shininess, out float cosAngIncidence){
  cosAngIncidence = calculateCosAngIncidence(normal,direction,0);
  vec4 halfAngle = normalize(direction + viewDirection);
  float blinnPhongTerm = dot(normal, halfAngle);
  blinnPhongTerm = clamp(blinnPhongTerm, 0, 1);
  blinnPhongTerm = cosAngIncidence != 0.0 ? blinnPhongTerm : 0.0;
  blinnPhongTerm = pow(blinnPhongTerm, shininess);
  return blinnPhongTerm;
}

float calculateShadowFactor(in vec4 lightSpacePos){
  vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
  vec3 uv = 0.5 * projCoords + 0.5;
  if(uv.x > 1.0 || uv.y > 1.0 || uv.x < 0.0 || uv.y < 0.0) return 1.0;
  float depth = texture(depthMap,uv.xy).x;
  if (depth < uv.z - 0.00001){
    return 0.4;
  }
  return 1.0;
}

uniform Light light;

void main(){
    vec4 worldPosition = texture(fbposition,fUv);
    vec4 lightSpace = light.depthMatrix * inverseWorldMatrix * worldPosition;
    float shadowFactor = calculateShadowFactor(lightSpace);
    vec4 diffuse = texture(fbdiffuse,fUv);
    vec4 normal = normalize(texture(fbnormal,fUv));
    vec4 specular = texture(fbspecular,fUv);
    float shininess = specular.w;
    specular.w = 0.0;
    vec4 viewDirection = normalize(-worldPosition);
    float cosAng;
    float blinnPhongTerm = calculateBlinnPhongTerm(normalize(light.position),normal,viewDirection,20,cosAng);
    color = shadowFactor * light.color * diffuse *cosAng;
    color += shadowFactor *  light.color * specular * blinnPhongTerm;
    color =  diffuse*texture(fbssao,fUv);
    color = pow(color,vec4(1/gamma,1/gamma,1/gamma,1.0));
}