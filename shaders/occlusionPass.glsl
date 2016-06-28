#version 330

in vec2 fUv;
layout(location = 0)out float ao;
uniform sampler2D fbposition;
uniform sampler2D fbnormal;
uniform int screenWidth;
uniform int screenHeight;
uniform sampler2D fbdepth;
uniform mat4 projectionMatrix;

uniform vec4 ssaoKernel[128];
uniform int ssaoKernelSize;
uniform float ssaoRadius;
uniform sampler2D ssaoNoise;

void main(){
  vec3 origin = texture(fbposition,fUv).xyz;
  vec3 normal = normalize(texture(fbnormal,fUv)).xyz;

  vec2 noiseScale = vec2(screenWidth,screenHeight)/vec2(4.0);
  vec3 rvec = texture(ssaoNoise, fUv * noiseScale).xyz;
  vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
  vec3 bitangent = cross(normal, tangent);
  mat3 tbn = mat3(tangent, bitangent, normal);
  
  float occlusion = 0.0;
  for(int i=0; i < ssaoKernelSize;i++){
    vec3 sample = tbn * ssaoKernel[i].xyz;
    sample = origin + sample * ssaoRadius;

    vec4 offset = vec4(sample, 1.0);
    offset = projectionMatrix * offset;
    offset.xyz /= offset.w;
    offset.xyz = offset.xyz * 0.5 + 0.5;
    
    float sampleDepth = -texture(fbposition, offset.xy).w;
    float rangeCheck= smoothstep(0.0,1.0,ssaoRadius / abs(origin.z - sampleDepth));
    occlusion += (sampleDepth >= sample.z ? 1.0 : 0.0) * rangeCheck;
  }
  occlusion = 1.0 - (occlusion / ssaoKernelSize);
  ao= occlusion;
}