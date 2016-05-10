#include "Renderer.h"

Vao& Renderer::initGeometryBuffers(const Geometry& geom){
  auto &bufferObj = this->vao[geom.getUUID()];
  if (bufferObj.vao == 0) glGenVertexArrays(1,&bufferObj.vao);
  if( (bufferObj.vertex == 0) && !geom.getVertices().empty()){
    uint buf = makeBuffer(
      GL_ARRAY_BUFFER,
      geom.getVertices().data(),
      geom.getVertices().size() * sizeof(GLfloat)
    );
    bufferObj.vertex = buf;
  }
  if( (bufferObj.index == 0) && !geom.getElements().empty()){
    uint buf = makeBuffer(
      GL_ELEMENT_ARRAY_BUFFER,
      geom.getElements().data(),
      geom.getElements().size() * sizeof(GLushort)
    );
    bufferObj.index = buf;
  }
  if( (bufferObj.normal == 0) && !geom.getNormals().empty()){
    uint buf = makeBuffer(
      GL_ARRAY_BUFFER,
      geom.getNormals().data(),
      geom.getNormals().size() * sizeof(GLfloat)
    );
    bufferObj.normal = buf;
  }
  if( (bufferObj.uv == 0) && !geom.getTexCoords().empty()){
    uint buf = makeBuffer(
      GL_ARRAY_BUFFER,
      geom.getTexCoords().data(),
      geom.getTexCoords().size() * sizeof(GLfloat)
    );
    bufferObj.uv = buf;
  }
  if( (bufferObj.tangent == 0) && !geom.getTangents().empty()){
    uint buf = makeBuffer(
      GL_ARRAY_BUFFER,
      geom.getTangents().data(),
      geom.getTangents().size() * sizeof(GLfloat)
    );
    bufferObj.tangent = buf;
  }
  return bufferObj;
}

Renderer& Renderer::render(const Scene& scene, const Camera& cam){
  //Mat4 world = cam.getWorldMatrix();
  //Mat4 projection = cam.getProjectionMatrix();

  for(auto obj : scene.getObjects()){
    auto mesh = std::static_pointer_cast<Mesh>(obj);
    auto geom = mesh->getGeometry();
    auto mat = mesh->getMaterial();

    auto bufferObj =  initGeometryBuffers(*geom);
    glBindVertexArray(bufferObj.vao);
    //setUpVertexAttributes();
  }

  return *this;
}

uint makeBuffer(GLenum target, const void* data, int size, GLenum usage){
  uint buf;
  glGenBuffers(1,&buf);
  glBindBuffer(target,buf);
  glBufferData(target,size,data,usage);
  return buf;
}