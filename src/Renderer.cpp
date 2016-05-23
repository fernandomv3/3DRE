#include "Renderer.h"
#include <iostream>

Renderer::Renderer(int width, int height){
  this->height = height;
  this->width = width;
}

Renderer::~Renderer(){
  for(auto v : this->vao){
    for(auto vbo : v.second.vbo){
      glDeleteBuffers(1,&(vbo.second.buffer));
    }
    glDeleteVertexArrays(1,&(v.second.vao));
  }
  for(auto p : this->programs){
    glDeleteProgram(p.second.getProgram());
  }
}

Vao& Renderer::initGeometryBuffers(const Geometry& geom){
  auto &bufferObj = this->vao[geom.getUUID()];
  if (bufferObj.vao == 0) glGenVertexArrays(1,&bufferObj.vao);
  if(bufferObj.vbo.empty()){
    auto geomAttr =geom.getAttributes();
    for(auto attr : geomAttr){
      std::string name = std::get<0>(attr);
      bufferObj.vbo[name].type = this->GLType[std::get<4>(attr)];
      uint buf = makeBuffer(
        std::get<0>(attr) == "index" ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER,
        std::get<1>(attr),
        std::get<2>(attr)
      );
      bufferObj.vbo[name].numComponents = std::get<3>(attr);
      bufferObj.vbo[name].buffer = buf;
    }
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

std::unordered_map<std::string,int>& Renderer::initTextures(const Material& mat){
  auto &program = this->programs[mat.getUUID()];
  auto& texUnits = program.getTexUnits();
  if(texUnits.empty()){  
    auto tex = mat.getTextures();
    for(auto t : tex){
      auto& texObj = this->textures[t.second->getUUID()];
      if(t.second->getSourceFile() != "") t.second->loadFile();
      texObj.texture = makeTexture(*(t.second));
      texObj.sampler = makeSampler(*(t.second));
      texUnits[t.first] = program.getFreeTextureUnit();
    }
  }
  return texUnits;
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
    uniforms["colorMap"] = glGetUniformLocation(program,"colorMap");
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

  Renderer& Renderer::setUpTextureUniforms(std::unordered_map<std::string,int>& uniforms,Material& mat,std::unordered_map<std::string,int>& texUnits){
    auto& texObj = textures[mat.getColorMap()->getUUID()];
    glUniform1i(uniforms["colorMap"],texUnits["colorMap"]);
    glActiveTexture(GL_TEXTURE0 + texUnits["colorMap"]);
    glBindTexture(GL_TEXTURE_2D,texObj.texture);
    glBindSampler(texUnits["colorMap"],texObj.sampler);
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

    auto& texUnits = initTextures(*mat);

    auto& uniforms = getUniformLocations(program,scene,cam,*obj,*geom,*mat);

    glUseProgram(program.getProgram());

    glBindVertexArray(bufferObj.vao);

    setUpVertexAttributes(program,bufferObj);

    setUpCameraUniforms(uniforms,cam);

    setUpObjectUniforms(uniforms,*obj);

    setUpMaterialUniforms(uniforms,*mat);

    setUpTextureUniforms(uniforms,*mat, texUnits);

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

uint Renderer::makeBuffer(GLenum target, const void* data, int size, GLenum usage){
  uint buf;
  glGenBuffers(1,&buf);
  glBindBuffer(target,buf);
  glBufferData(target,size,data,usage);
  return buf;
}

uint Renderer::makeTexture(const Texture& texture){
  uint tex;
  uint target = texture.getDimensions() == 2 ? GL_TEXTURE_2D : GL_TEXTURE_3D;
  uint format = texture.getAlpha() ? GL_RGBA : GL_RGB;
  uint innerFormat = texture.getGamma() ? GL_SRGB8 : GL_RGB8;
  
  glGenTextures(1,(GLuint*)&tex);
  glBindTexture(target,tex);
  glTexImage2D(
    target, 0, innerFormat,
    texture.getWidth(), texture.getHeight(), 0,
    format, this->GLType[texture.getType()], texture.getImage()
  );
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(target, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  glTexParameteri(target, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);
  return tex;
}

uint Renderer::makeSampler(const Texture& texture){
  uint sampler;
  glGenSamplers(1,&sampler);
  glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  return sampler;
}