#include "Renderer.h"
#include <iostream>

Renderer::Renderer(int width, int height){
  this->height = height;
  this->width = width;
  this->readFramebuffer = nullptr;
  this->writeFramebuffer = nullptr;
}

Renderer::~Renderer(){
  for(auto& v : this->vao){
    for(auto vbo : v.second.vbo){
      glDeleteBuffers(1,&(vbo.second.buffer));
    }
    glDeleteVertexArrays(1,&(v.second.vao));
  }
  for(auto& p : this->programs){
    glDeleteProgram(p.second.getProgram());
  }
  for(auto& t : this->textures){
    glDeleteTextures(1,&(t.second.texture));
  }
}

Vao& Renderer::initGeometryBuffers(const Geometry& geom){
  auto &bufferObj = this->vao[geom.getUUID()];
  if (bufferObj.vao == 0) glGenVertexArrays(1,&bufferObj.vao);
  if(bufferObj.vbo.empty()){
    auto geomAttr =geom.getAttributes("init");
    for(auto attr : geomAttr){
      std::string name = std::get<0>(attr);
      uint buf = makeBuffer(
        name == "index" ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER,
        std::get<1>(attr),
        std::get<2>(attr)
      );
      bufferObj.vbo[name].numComponents = std::get<3>(attr);
      bufferObj.vbo[name].type = this->GLType[std::get<4>(attr)];
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
    auto tex = mat.getTextures("init");
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

/*Renderer& Renderer::initWriteFramebuffer(){
  if(texUnits.empty()){  
    auto tex = fb->getRenderTargets("init");
    for(auto t : tex){
      auto& texObj = this->textures[t.second->getUUID()];
      if(texObj.texture == 0){
        texObj.texture = makeTexture(*(t.second));
      }
    }
  }
  return *this;
}*/

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

std::unordered_map<std::string,int>& Renderer::getUniformLocations(GLProgram& prog,Scene& scene,Camera& cam,Object3D& obj,Geometry& geom,Material& mat){
  int program = prog.getProgram();
  auto& uniforms = prog.getUniforms();
  if(uniforms.empty()){
    auto camUniformData = cam.getUniforms("location");
    auto objUniformData = obj.getUniforms("location");
    auto matUniformData = mat.getUniforms("location");
    auto globalUniformData = this->getUniforms("location");
    auto sceneUniformData = scene.getUniforms("location");
    auto tex = mat.getTextures("location");
    for(auto u : camUniformData) uniforms[std::get<0>(u)] = glGetUniformLocation(program,std::get<0>(u).c_str());
    for(auto u : objUniformData) uniforms[std::get<0>(u)] = glGetUniformLocation(program,std::get<0>(u).c_str());
    for(auto u : matUniformData) uniforms[std::get<0>(u)] = glGetUniformLocation(program,std::get<0>(u).c_str());
    for(auto u : globalUniformData) uniforms[std::get<0>(u)] = glGetUniformLocation(program,std::get<0>(u).c_str());
    for(auto u : sceneUniformData) uniforms[std::get<0>(u)] = glGetUniformLocation(program,std::get<0>(u).c_str());
    for(auto t : tex) uniforms[t.first] = glGetUniformLocation(program,t.first.c_str());
  }
  return uniforms;
}

Renderer& Renderer::setUpCameraUniforms(std::unordered_map<std::string,int>& uniforms,Camera& cam){
  auto uniformData = cam.getUniforms("forward");
  for(auto& u : uniformData){
    updateUniform(uniforms[std::get<0>(u)],std::get<2>(u),std::get<1>(u),std::get<3>(u));
  }
  return *this;
}

Renderer& Renderer::setUpObjectUniforms(std::unordered_map<std::string,int>& uniforms,Object3D& obj){
  auto uniformData = obj.getUniforms("forward");
  for(auto& u : uniformData){
    updateUniform(uniforms[std::get<0>(u)],std::get<2>(u),std::get<1>(u),std::get<3>(u));
  }
  return *this;
}

Renderer& Renderer::setUpMaterialUniforms(std::unordered_map<std::string,int>& uniforms,Material& mat){
  auto uniformData = mat.getUniforms("forward");
  for(auto& u : uniformData){
    updateUniform(uniforms[std::get<0>(u)],std::get<2>(u),std::get<1>(u),std::get<3>(u));
  }
  return *this;
}

Renderer& Renderer::setUpSceneUniforms(std::unordered_map<std::string,int>& uniforms,Scene& scene){
  auto uniformData = scene.getUniforms("forward");
  for(auto& u : uniformData){
    updateUniform(uniforms[std::get<0>(u)],std::get<2>(u),std::get<1>(u),std::get<3>(u));
  }
  return *this;
}

Renderer& Renderer::setUpGlobalUniforms(std::unordered_map<std::string,int>& uniforms){
  auto uniformData = getUniforms("forward");
  for(auto& u : uniformData){
    updateUniform(uniforms[std::get<0>(u)],std::get<2>(u),std::get<1>(u),std::get<3>(u));
  }
  return *this;
}

  Renderer& Renderer::setUpTextureUniforms(std::unordered_map<std::string,int>& uniforms,Material& mat,std::unordered_map<std::string,int>& texUnits){
    auto matTextures = mat.getTextures("forward");
    for(auto& texture : matTextures){
      auto& texObj = textures[texture.second->getUUID()];
      glUniform1i(uniforms[texture.first],texUnits[texture.first]);
      glActiveTexture(GL_TEXTURE0 + texUnits[texture.first]);
      glBindTexture(GLTextureTarget[texture.second->getTarget()],texObj.texture);
      glBindSampler(texUnits[texture.first],texObj.sampler);
    }
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

Renderer& Renderer::render(Scene& scene, Camera& cam,std::string passName){
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

    obj->updateModelMatrix();
    obj->updateNormalModelMatrix();
    setUpObjectUniforms(uniforms,*obj);

    setUpMaterialUniforms(uniforms,*mat);

    setUpTextureUniforms(uniforms,*mat, texUnits);

    setUpSceneUniforms(uniforms,scene);

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

Renderer& Renderer::setReadFramebuffer(std::shared_ptr<Framebuffer> fb){
  this->readFramebuffer = fb;
  return *this;
}

Renderer& Renderer::setWriteFramebuffer(std::shared_ptr<Framebuffer> fb){
  this->writeFramebuffer = fb;
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
  uint target = this->GLTextureTarget[texture.getTarget()];
  uint innerFormat = texture.getGamma() ? this->GLInnerFormat["S"+texture.getFormat()+"8"] : this->GLInnerFormat[texture.getFormat()];
  
  glGenTextures(1,(GLuint*)&tex);
  glBindTexture(target,tex);
  glTexImage2D(
    target, 0, innerFormat,
    texture.getWidth(), texture.getHeight(), 0,
    this->GLFormat[texture.getFormat()], this->GLType[texture.getType()], texture.getImage()
  );
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, this->GLFiltering[texture.getFiltering().second]);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, this->GLFiltering[texture.getFiltering().second]);
  glTexParameteri(target, GL_TEXTURE_WRAP_S,this->GLWrapping[std::get<0>(texture.getWrapping())]);
  glTexParameteri(target, GL_TEXTURE_WRAP_T,this->GLWrapping[std::get<1>(texture.getWrapping())]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, texture.getNMipmaps());
  if(texture.getNMipmaps() > 1) glGenerateMipmap(target);
  return tex;
}

uint Renderer::makeSampler(const Texture& texture){
  uint sampler;
  glGenSamplers(1,&sampler);
  glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, this->GLFiltering[texture.getFiltering().second]);
  glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, this->GLFiltering[texture.getFiltering().first]);
  glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, this->GLWrapping[std::get<0>(texture.getWrapping())]);
  glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, this->GLWrapping[std::get<1>(texture.getWrapping())]);
  return sampler;
}

Renderer& Renderer::updateUniform(int location,int count, const std::string type, void* data){
  if(type.length() == 4){
    if(type == "m2fv") {glUniformMatrix2fv(location,count,GL_FALSE,static_cast<float*>(data)); return *this;}
    if(type == "m3fv") {glUniformMatrix3fv(location,count,GL_FALSE,static_cast<float*>(data)); return *this;}
    if(type == "m4fv") {glUniformMatrix4fv(location,count,GL_FALSE,static_cast<float*>(data)); return *this;}
  }
  else if(type.length() == 2){
    if(type == "1f") {glUniform1f(location,static_cast<float*>(data)[0]); return *this;}
    if(type == "2f") {glUniform2f(location,static_cast<float*>(data)[0],static_cast<float*>(data)[1]); return *this;}
    if(type == "3f") {glUniform3f(location,static_cast<float*>(data)[0],static_cast<float*>(data)[1],static_cast<float*>(data)[2]); return *this;}
    if(type == "4f") {glUniform4f(location,static_cast<float*>(data)[0],static_cast<float*>(data)[1],static_cast<float*>(data)[2],static_cast<float*>(data)[3]); return *this;}
    
    if(type == "1i") {glUniform1i(location,static_cast<int*>(data)[0]); return *this;}
    if(type == "2i") {glUniform2i(location,static_cast<int*>(data)[0],static_cast<int*>(data)[1]); return *this;}
    if(type == "3i") {glUniform3i(location,static_cast<int*>(data)[0],static_cast<int*>(data)[1],static_cast<int*>(data)[2]); return *this;}
    if(type == "4i") {glUniform4i(location,static_cast<int*>(data)[0],static_cast<int*>(data)[1],static_cast<int*>(data)[2],static_cast<int*>(data)[3]); return *this;}
    
    if(type == "1u") {glUniform1ui(location,static_cast<uint*>(data)[0]); return *this;}
    if(type == "2u") {glUniform2ui(location,static_cast<uint*>(data)[0],static_cast<uint*>(data)[1]); return *this;}
    if(type == "3u") {glUniform3ui(location,static_cast<uint*>(data)[0],static_cast<uint*>(data)[1],static_cast<uint*>(data)[2]); return *this;}
    if(type == "4u") {glUniform4ui(location,static_cast<uint*>(data)[0],static_cast<uint*>(data)[1],static_cast<uint*>(data)[2],static_cast<uint*>(data)[3]); return *this;}
  }
  else if (type.length() == 3){
    if(type == "1fv") {glUniform1fv(location,count,static_cast<float*>(data)); return *this;}
    if(type == "2fv") {glUniform2fv(location,count,static_cast<float*>(data)); return *this;}
    if(type == "3fv") {glUniform3fv(location,count,static_cast<float*>(data)); return *this;}
    if(type == "4fv") {glUniform4fv(location,count,static_cast<float*>(data)); return *this;}
    
    if(type == "1iv") {glUniform1iv(location,count,static_cast<int*>(data)); return *this;}
    if(type == "2iv") {glUniform2iv(location,count,static_cast<int*>(data)); return *this;}
    if(type == "3iv") {glUniform3iv(location,count,static_cast<int*>(data)); return *this;}
    if(type == "4iv") {glUniform4iv(location,count,static_cast<int*>(data)); return *this;}
    
    if(type == "1uv") {glUniform1uiv(location,count,static_cast<uint*>(data)); return *this;}
    if(type == "2uv") {glUniform2uiv(location,count,static_cast<uint*>(data)); return *this;}
    if(type == "3uv") {glUniform3uiv(location,count,static_cast<uint*>(data)); return *this;}
    if(type == "4uv") {glUniform4uiv(location,count,static_cast<uint*>(data)); return *this;}
  }
  else{
    if(type == "m3x2fv") {glUniformMatrix3x2fv(location,count,GL_FALSE,static_cast<float*>(data)); return *this;}
    if(type == "m2x3fv") {glUniformMatrix2x3fv(location,count,GL_FALSE,static_cast<float*>(data)); return *this;}
    if(type == "m2x4fv") {glUniformMatrix2x4fv(location,count,GL_FALSE,static_cast<float*>(data)); return *this;}
    if(type == "m4x2fv") {glUniformMatrix4x2fv(location,count,GL_FALSE,static_cast<float*>(data)); return *this;}
    if(type == "m3x4fv") {glUniformMatrix3x4fv(location,count,GL_FALSE,static_cast<float*>(data)); return *this;}
    if(type == "m4x3fv") {glUniformMatrix4x3fv(location,count,GL_FALSE,static_cast<float*>(data)); return *this;}
  }
  return *this;
}

std::vector< std::tuple<std::string,std::string,int,void*> > Renderer::getUniforms(std::string passName){
  std::vector< std::tuple<std::string,std::string,int,void*> > res;
  res.push_back(std::make_tuple("time","1f",1,&time));
  res.push_back(std::make_tuple("screenWidth","1i",1,&width));
  res.push_back(std::make_tuple("screenHeight","1i",1,&height));
  return res;
}