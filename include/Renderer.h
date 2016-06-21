#ifndef RENDERER_H
#define RENDERER_H
#include "GL/glew.h"

#include "Scene.h"
#include "Camera.h"
#include "GLProgram.h"
#include "Framebuffer.h"

#include <unordered_map>
#include <typeindex>
#include <string>
#include <memory>
#include <stack>

struct vbo{
  uint buffer;
  char numComponents;
  uint type;
};
typedef struct vbo Vbo;

struct vao{
  uint vao = 0;
  bool initialized = false;
  std::unordered_map<std::string,Vbo> vbo;
};
typedef struct vao Vao;

struct gltexture{
  uint texture =0;
  uint sampler = 0;
};
typedef struct gltexture GLTexture;

class Renderer{
private:
  std::stack<int> freeTexUnits;
  std::unordered_map<std::string,Vao> vao;
  std::unordered_map<std::string,GLProgram> programs;
  std::unordered_map<std::string,GLTexture> textures;

  std::shared_ptr<Framebuffer> writeFramebuffer;
  std::shared_ptr<Framebuffer> readFramebuffer;

  float time;
  int height;
  int width;

  std::unordered_map<std::type_index, uint> GLType{
    {std::type_index(typeid(unsigned char)), GL_UNSIGNED_BYTE},
    {std::type_index(typeid(char)), GL_BYTE},
    {std::type_index(typeid(unsigned short)), GL_UNSIGNED_SHORT},
    {std::type_index(typeid(short)), GL_SHORT},
    {std::type_index(typeid(uint)), GL_UNSIGNED_INT},
    {std::type_index(typeid(int)), GL_INT},
    {std::type_index(typeid(float)), GL_FLOAT}
  };
  std::unordered_map<std::string, uint> GLTextureTarget{
    {"2D",GL_TEXTURE_2D},
    {"CUBE+X",GL_TEXTURE_CUBE_MAP_POSITIVE_X},
    {"CUBE-X",GL_TEXTURE_CUBE_MAP_NEGATIVE_X},
    {"CUBE+Y",GL_TEXTURE_CUBE_MAP_POSITIVE_Y},
    {"CUBE-Y",GL_TEXTURE_CUBE_MAP_NEGATIVE_Y},
    {"CUBE+Z",GL_TEXTURE_CUBE_MAP_POSITIVE_Z},
    {"CUBE-Z",GL_TEXTURE_CUBE_MAP_NEGATIVE_Z}
  };
  std::unordered_map<std::string, uint> GLFormat{
    {"R",GL_RED},
    {"RG",GL_RG},
    {"RGB",GL_RGB},
    {"BGR",GL_BGR},
    {"RGBA",GL_RGBA},
    {"BGRA",GL_BGRA},
    {"intR",GL_RED_INTEGER},
    {"intRG",GL_RG_INTEGER},
    {"intRGB",GL_RGB_INTEGER},
    {"intBGR",GL_BGR_INTEGER},
    {"intRGBA",GL_RGBA_INTEGER},
    {"intBGRA",GL_BGRA_INTEGER},
    {"depth",GL_DEPTH_COMPONENT},
    {"depth_stencil",GL_DEPTH_STENCIL}
  };
  std::unordered_map<std::string, uint> GLInnerFormat{
    {"depth",GL_DEPTH_COMPONENT},
    {"depth_stencil",GL_DEPTH_STENCIL},
    {"R",GL_RED},
    {"RG",GL_RG},
    {"RGB",GL_RGB},
    {"RGBA",GL_RGBA},
    {"R8",GL_R8},
    {"RG8",GL_RG8},
    {"RGB8",GL_RGB8},
    {"RGBA8",GL_RGBA8},
    {"SRGB8",GL_SRGB8},
    {"SRGBA8",GL_SRGB8_ALPHA8},
    {"RGBA16F",GL_RGBA16F}
  };
  std::unordered_map<std::string, uint> GLFiltering{
    {"nearest",GL_NEAREST},
    {"linear",GL_LINEAR},
    {"linear_mipmap_linear",GL_LINEAR_MIPMAP_LINEAR},
    {"nearest_mipmap_nearest",GL_NEAREST_MIPMAP_NEAREST},
    {"nearest_mipmap_linear",GL_NEAREST_MIPMAP_LINEAR},
    {"linear_mipmap_nearest",GL_LINEAR_MIPMAP_NEAREST}
  };
  std::unordered_map<std::string, uint> GLWrapping{
    {"edge",GL_CLAMP_TO_EDGE},
    {"border",GL_CLAMP_TO_BORDER},
    {"mirror",GL_MIRRORED_REPEAT},
    {"repeat",GL_REPEAT}
  };
public:
  Renderer(int width,int height);
  ~Renderer();
  int getFreeTextureUnit();
  Renderer& releaseTextureUnit(int unit);
  Renderer& setTime(float ms);
  Renderer& setReadFramebuffer(std::shared_ptr<Framebuffer> fb);
  Renderer& setWriteFramebuffer(std::shared_ptr<Framebuffer> fb);
  Renderer& render(Scene& scene,Camera& camera,std::string passName);
  Vao& initGeometryBuffers(const Geometry& geom);
  Renderer& setUpVertexAttributes(GLProgram& prog,Vao& vao);
  GLProgram& initProgram(const Material& mat, const Geometry& geom);
  std::unordered_map<std::string,int>& initTextures(const Material& mat,const Scene& scene);
  Renderer& initWriteFramebuffer();
  std::unordered_map<std::string,int>& initReadFramebuffer(const Material& mat);
  std::unordered_map<std::string,int>& getUniformLocations(std::string passName,GLProgram& prog,Scene& scene,Camera& cam,Object3D& obj,Geometry& geom,Material& mat);
  Renderer& setUpCameraUniforms(std::unordered_map<std::string,int>& uniforms,Camera& cam);
  Renderer& setUpObjectUniforms(std::unordered_map<std::string,int>& uniforms,Object3D& obj);
  Renderer& setUpMaterialUniforms(std::unordered_map<std::string,int>& uniforms,Material& mat);
  Renderer& setUpSceneUniforms(std::string passName,std::unordered_map<std::string,int>& uniforms,Scene& scene);
  Renderer& setUpGlobalUniforms(std::unordered_map<std::string,int>& uniforms);
  Renderer& setUpTextureUniforms(std::unordered_map<std::string,int>& uniforms,Material& mat,Scene& scene,std::unordered_map<std::string,int>& texUnits);
  Renderer& setUpReadFramebufferUniforms(std::unordered_map<std::string,int>& uniforms,std::unordered_map<std::string,int>& texUnits);
  Renderer& drawGeometry(const Geometry& geom, Vao& vao);
  uint makeTexture(const Texture& texture);
  uint makeBuffer(GLenum target, const void* data, int size, GLenum usage = GL_STATIC_DRAW);
  uint makeSampler(const Texture& texture);
  Renderer& updateUniform(int location,int count,const std::string type, void* data);
  virtual std::vector< std::tuple<std::string,std::string,int,void*> >getUniforms(std::string passName);
};

#endif