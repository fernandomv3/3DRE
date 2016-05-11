#include "Renderer.h"
#include <iostream>

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

GLProgram& Renderer::initProgram(const Material& mat){
  auto &program = this->programs[mat.getUUID()];
  if(program.getProgram() == 0){
    for(auto s : mat.getShaders()){
      program.addShader(s.first,s.second);
    }
    program.makeProgram();
  }
  auto &attrLoc = program.getAttrLoc();
  attrLoc["vPosition"] = glGetAttribLocation(program.getProgram(),"vPosition");
  return program;
}

Renderer& Renderer::setUpVertexAttributes(GLProgram& prog, const Vao& vao){
  glBindBuffer(GL_ARRAY_BUFFER,vao.vertex);
  int loc = prog.getAttrLoc()["vPosition"];
  glVertexAttribPointer(
    loc,
    3,
    GL_FLOAT,
    GL_FALSE,
    0,
    (void*)0
  );
  glEnableVertexAttribArray(loc);
  return *this;
}

Renderer& Renderer::render(const Scene& scene, const Camera& cam){
  //Mat4 world = cam.getWorldMatrix();
  //Mat4 projection = cam.getProjectionMatrix();
  for(auto obj : scene.getObjects()){
    auto mesh = std::static_pointer_cast<Mesh>(obj);
    auto geom = mesh->getGeometry();
    auto mat = mesh->getMaterial();

    auto bufferObj =  initGeometryBuffers(*geom);
    auto program = initProgram(*mat);

    glUseProgram(program.getProgram());

    glBindVertexArray(bufferObj.vao);

    setUpVertexAttributes(program,bufferObj);

    glBindBuffer(GL_ARRAY_BUFFER,bufferObj.vertex);
    int numVertices = geom->getVertices().size() / 3;
    glDrawArrays(
      GL_TRIANGLES,
      0,
      numVertices
    );
    glBindVertexArray(0);
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