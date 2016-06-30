#version 330

in vec2 fUv;
layout(location = 0)out vec4 ssao;
uniform sampler2D fbocclusion;
uniform int screenWidth;
uniform int screenHeight;

void main(){
  vec2 size = 1.0/ vec2(textureSize(fbocclusion,0));
  float result = 0.0;
  vec2 hlim = vec2(-1.5);
  for(int i = -2; i < 2; i++){
    for(int j=-2; j< 2; j++){
      vec2 offset = vec2(i,j) * size;
      result += texture(fbocclusion,fUv +offset).x;
    }
  }
  ssao= vec4(result/25.0);
  //ssao = texture(fbocclusion,fUv);
}