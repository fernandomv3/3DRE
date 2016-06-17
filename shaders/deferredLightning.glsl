#version 330

in vec2 fUv;
out vec4 color;
uniform sampler2D fbposition;
uniform sampler2D fbnormal;
uniform sampler2D fbdiffuse;
uniform sampler2D fbspecular;
uniform int screenWidth;
uniform int screenHeight;
uniform float gamma;

void main(){
    color = texture(fbspecular,fUv);
}