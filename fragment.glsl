#version 330

in vec4 fNormal;
in vec4 fWorldPosition;
//in vec2 fUv;
layout(location = 0) out vec4 color;
uniform float gamma;
uniform float time;
uniform int screenWidth;
uniform int screenHeight;
//uniform sampler2D colorMap;
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

uniform Material material;
uniform Light light;

void main(){
	//vec4 diffuseColor = texture(colorMap,fUv);
	float cosAng = calculateCosAngIncidence(normalize(light.position),normalize(fNormal),0.0);

	color = light.color * material.diffuse *cosAng;
	color = pow(color,vec4(1/gamma,1/gamma,1/gamma,1.0));
	//color = (normalize(fWorldPosition) +1)/2;
}