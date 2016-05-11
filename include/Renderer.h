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
public:
  Renderer(){};
  Renderer& render(const Scene& scene,const Camera& camera);
  Vao& initGeometryBuffers(const Geometry& geom);
  GLProgram& initProgram(const Material& mat);
  Renderer& setUpVertexAttributes(GLProgram& prog, const Vao& vao);
};

uint makeBuffer(GLenum target, const void* data, int size, GLenum usage = GL_STATIC_DRAW);

#endif