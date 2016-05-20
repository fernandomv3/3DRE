#include "Renderer.h"
#include <iostream>

Renderer::Renderer(int width, int height){
  this->height = height;
  this->width = width;
}

Vao& Renderer::initGeometryBuffers(const Geometry& geom){
  auto &bufferObj = this->vao[geom.getUUID()];
  if (bufferObj.vao == 0) glGenVertexArrays(1,&bufferObj.vao);
  auto geomAttr =geom.getAttributes();
  for(auto attr : geomAttr){
    std::string name = std::get<0>(attr);
    std::type_index t = std::get<4>(attr);
    int elementSize = 0;
    if (t == std::type_index(typeid(float))){
      elementSize = sizeof(GLfloat);
      bufferObj.vbo[name].type = GL_FLOAT;
    }else if(t == std::type_index(typeid(ushort))){
      elementSize = sizeof(GLushort);
      bufferObj.vbo[name].type =GL_UNSIGNED_SHORT;
    }
    uint buf = makeBuffer(
      std::get<0>(attr) == "index" ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER,
      std::get<1>(attr),
      std::get<2>(attr) * elementSize
    );
    bufferObj.vbo[name].numComponents = std::get<3>(attr);
    bufferObj.vbo[name].buffer = buf;
  }
  return bufferObj;
}

GLProgram& Renderer::initProgram(const Material& mat, const Geometry& geom){
  auto &program = this->programs[mat.getUUID()];
  auto &bufferObj = this->vao[geom.getUUID()];
  if(program.getProgram() == 0){
    for(auto s : mat.getShaders()){
      std::string src = program.getSourceFromFile(s.second);
      program.addShader(s.first.substr(0,s.first.size()-4),src);
    }
    program.makeProgram();
    auto &attrLoc = program.getAttrLoc();
    for(auto vbo : bufferObj.vbo){
      if(vbo.first == "index") continue;
      attrLoc[vbo.first] = glGetAttribLocation(program.getProgram(),vbo.first.data());
    }
  }
  return program;
}

Renderer& Renderer::setUpVertexAttributes(GLProgram& prog, Vao& vao){
  if(!(vao.initialized)){
    for(auto vbo : vao.vbo){
      if(vbo.first == "index")continue;
      glBindBuffer(GL_ARRAY_BUFFER,vbo.second.buffer);
      int loc = prog.getAttrLoc()[vbo.first];
      glVertexAttribPointer(
        loc,
        vbo.second.numComponents,
        vbo.second.type,
        GL_FALSE,
        0,
        (void*)0
      );
      glEnableVertexAttribArray(loc);  
    }
    vao.initialized = true;
  }
  return *this;
}

std::unordered_map<std::string,int>& Renderer::getUniformLocations(GLProgram& prog,const Scene& scene,const Camera& cam,Object3D& obj,Geometry& geom,Material& mat){
  int program = prog.getProgram();
  auto& uniforms = prog.getUniforms();
  if(uniforms.empty()){
    uniforms["worldMatrix"] = glGetUniformLocation(program,"worldMatrix");
    uniforms["projectionMatrix"] = glGetUniformLocation(program,"projectionMatrix");
    uniforms["modelMatrix"] = glGetUniformLocation(program,"modelMatrix");
    uniforms["normalModelMatrix"] = glGetUniformLocation(program,"normalModelMatrix");
    uniforms["gamma"] = glGetUniformLocation(program,"gamma");
    uniforms["time"] = glGetUniformLocation(program,"time");
    uniforms["screenWidth"] = glGetUniformLocation(program,"screenWidth");
    uniforms["screenHeight"] = glGetUniformLocation(program,"screenHeight");
    uniforms["material.diffuse"] = glGetUniformLocation(program,"material.diffuse");
    uniforms["material.specular"] = glGetUniformLocation(program,"material.specular");
    uniforms["material.shininess"] = glGetUniformLocation(program,"material.shininess");
  }
  return uniforms;
}

Renderer& Renderer::setUpCameraUniforms(std::unordered_map<std::string,int>& uniforms,const Camera& cam){
  glUniformMatrix4fv(
    uniforms["worldMatrix"],
    1,
    GL_FALSE,
    cam.getWorldMatrix().getElements().data()
  );
  glUniformMatrix4fv(
    uniforms["projectionMatrix"],
    1,
    GL_FALSE,
    cam.getProjectionMatrix().getElements().data()
  );
  glUniform1f(
    uniforms["gamma"],
    1.0/cam.getGamma()
  );
  return *this;
}

Renderer& Renderer::setUpObjectUniforms(std::unordered_map<std::string,int>& uniforms,Object3D& obj){
  obj.updateModelMatrix();
  obj.updateNormalModelMatrix();
  glUniformMatrix4fv(
    uniforms["modelMatrix"],
    1,
    GL_FALSE,
    obj.getModelMatrix().getElements().data()
  );
  glUniformMatrix4fv(
    uniforms["normalModelMatrix"],
    1,
    GL_FALSE,
    obj.getNormalModelMatrix().getElements().data()
  );
  return *this;
}

Renderer& Renderer::setUpMaterialUniforms(std::unordered_map<std::string,int>& uniforms,Material& mat){
  glUniform4fv(
    uniforms["material.diffuse"],
    1,
    mat.getDiffuseColor().getElements().data()
  );
  glUniform4fv(
    uniforms["material.specular"],
    1,
    mat.getSpecularColor().getElements().data()
  );
  glUniform1f(
    uniforms["material.shininess"],
    mat.getShininess()
  );
  return *this;
}

Renderer& Renderer::setUpGlobalUniforms(std::unordered_map<std::string,int>& uniforms){
  glUniform1f(
    uniforms["time"],
    this->time
  );
  glUniform1i(
    uniforms["screenWidth"],
    this->width
  );
  glUniform1i(
    uniforms["screenHeight"],
    this->height
  );
  return *this;
}

Renderer& Renderer::drawGeometry(const Geometry& geom, Vao& vao){
  if (!geom.getElements().empty()){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vao.vbo["index"].buffer);
    glDrawElements(
      GL_TRIANGLES,
      geom.getElements().size(),
      GL_UNSIGNED_SHORT,
      (void*)0
    );
  }else{
    glBindBuffer(GL_ARRAY_BUFFER,vao.vbo["vPosition"].buffer);
    int numVertices = geom.getVertices().size() / 3;
    glDrawArrays(
      GL_TRIANGLES,
      0,
      numVertices
    );
  }
  return *this;
}

Renderer& Renderer::render(const Scene& scene, Camera& cam){
  cam.updateWorldMatrix();
  for(auto obj : scene.getObjects()){
    auto mesh = std::static_pointer_cast<Mesh>(obj);
    auto geom = mesh->getGeometry();
    auto mat = mesh->getMaterial();

    auto& bufferObj =  initGeometryBuffers(*geom);

    auto& program = initProgram(*mat,*geom);
    auto &uniforms = getUniformLocations(program,scene,cam,*obj,*geom,*mat);

    glUseProgram(program.getProgram());

    glBindVertexArray(bufferObj.vao);

    setUpVertexAttributes(program,bufferObj);

    setUpCameraUniforms(uniforms,cam);

    setUpObjectUniforms(uniforms,*obj);

    setUpMaterialUniforms(uniforms,*mat);

    setUpGlobalUniforms(uniforms);

    drawGeometry(*geom,bufferObj);

    glBindVertexArray(0);
  }

  return *this;
}

Renderer& Renderer::setTime(float ms){
  this->time = ms;
  return *this;
}

uint makeBuffer(GLenum target, const void* data, int size, GLenum usage){
  uint buf;
  glGenBuffers(1,&buf);
  glBindBuffer(target,buf);
  glBufferData(target,size,data,usage);
  return buf;
}