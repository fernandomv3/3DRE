#version 330

in vec2 fUv;
layout(location = 0)out vec4 ao;
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
uniform float gamma;


void main(){
  vec3 origin = texture(fbposition,fUv).xyz;
  vec3 normal = normalize(texture(fbnormal,fUv)).xyz;
  vec2 noiseScale = vec2(screenWidth,screenHeight)/vec2(4.0);
  float depth = texture(fbdepth,fUv).x;

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
    offset.xy /= offset.w;
    offset.xy = offset.xy * 0.5 + 0.5;
    
    float sampleDepth = texture(fbposition, offset.xy).z;
    float rangeCheck= smoothstep(0.0,1.0,ssaoRadius / abs(origin.z - sampleDepth));
    occlusion += rangeCheck * step(sample.z,sampleDepth);
  }
  occlusion = 1-(occlusion / float(ssaoKernelSize));
  ao= vec4(pow(occlusion,4.0));
}