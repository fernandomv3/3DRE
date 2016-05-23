#version 330

in vec4 fNormal;
in vec2 fUv;
layout(location = 0) out vec4 color;
uniform float gamma;
uniform float time;
uniform int screenWidth;
uniform int screenHeight;
uniform sampler2D colorMap;

struct Material{
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform Material material;

void main(){
  vec4 diffuseColor = texture(colorMap,fUv);
  color = diffuseColor;
}