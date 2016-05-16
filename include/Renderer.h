#ifndef RENDERER_H
#define RENDERER_h
#include "GL/glew.h"

#include "Scene.h"
#include "Camera.h"
#include "GLProgram.h"

#include <unordered_map>
#include <string>
#include <memory>

struct vao{
  uint vao = 0;
  uint vertex = 0;
  uint index = 0;
  uint normal = 0;
  uint uv = 0;
  uint tangent = 0;
  std::unordered_map<std::string,uint> extra;
};
typedef struct vao Vao;

class Renderer{
private:
  std::unordered_map<std::string,Vao> vao;
  std::unordered_map<std::string,GLProgram> programs;
  float time;
public:
  Renderer(){};
  Renderer& setTime(float ms);
  Renderer& render(const Scene& scene,Camera& camera);
  Vao& initGeometryBuffers(const Geometry& geom);
  GLProgram& initProgram(const Material& mat);
  Renderer& setUpVertexAttributes(GLProgram& prog, const Vao& vao);
  std::unordered_map<std::string,int>& getUniformLocations(GLProgram& prog,const Scene& scene,const Camera& cam,Object3D& obj,Geometry& geom,Material& mat);
  Renderer& setUpCameraUniforms(std::unordered_map<std::string,int>& uniforms,const Camera& cam);
  Renderer& setUpObjectUniforms(std::unordered_map<std::string,int>& uniforms,const Object3D& obj);
  Renderer& setUpGlobalUniforms(std::unordered_map<std::string,int>& uniforms);
};

uint makeBuffer(GLenum target, const void* data, int size, GLenum usage = GL_STATIC_DRAW);

#endif