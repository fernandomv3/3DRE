#include "GLProgram.h"

#include <GL/glew.h>

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <memory>

GLProgram::GLProgram(){
  this->program = 0;
  this->uniforms = std::unordered_map<std::string,int>();
  for(int i = 16/*GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS*/ -1; i>=0; i--){
    this->freeTexUnits.push(i);
  }
}

int GLProgram::getFreeTextureUnit(){
  int res = freeTexUnits.top();
  freeTexUnits.pop();
  return res;
}
GLProgram& GLProgram::releaseTextureUnit(int unit){
  freeTexUnits.push(unit);
  return *this;
}

int GLProgram::getProgram()const{
  return this->program;
}

GLProgram& GLProgram::addShader(std::string type, std::string src){
  Shader s;
  s.type = type;
  s.source = src;
  shaders.push_back(s);
  return *this;
}

std::string GLProgram::getSourceFromFile(std::string filename){
  std::ifstream t(filename);
  return std::string((std::istreambuf_iterator<char>(t)),
                   std::istreambuf_iterator<char>());
}

int GLProgram::compileShader(Shader& shader){
  int length = shader.source.length();
  int shaderOk;

  if(shader.source.empty()) return 0;
  
  int type;
  if(shader.type == "vertex") 
    type = GL_VERTEX_SHADER;
  else if(shader.type == "fragment") 
    type = GL_FRAGMENT_SHADER;
  else if(shader.type == "tessControl") 
    type = GL_TESS_CONTROL_SHADER;
  else if(shader.type == "tessEval") 
    type = GL_TESS_EVALUATION_SHADER;
  else if(shader.type == "geometry") 
    type = GL_GEOMETRY_SHADER;
  else if(shader.type == "compute") 
    type = GL_COMPUTE_SHADER;
  else
    return 0;

  shader.shaderObject = glCreateShader(type);
  const char* c_source = shader.source.c_str();
  glShaderSource(shader.shaderObject, 1, (const GLchar**)&c_source, (GLint*)&length);
  glCompileShader(shader.shaderObject);
  glGetShaderiv(shader.shaderObject, GL_COMPILE_STATUS,&shaderOk);
  if(!shaderOk){
    int maxLength = 0;
    std::cerr << "Failed to compile:" << std::endl;
    glGetShaderiv(shader.shaderObject, GL_INFO_LOG_LENGTH, &maxLength);
    auto infoLog = std::unique_ptr<char[]>(new char[maxLength]);
    glGetShaderInfoLog(shader.shaderObject, maxLength, &maxLength, &infoLog[0]);
    std::string ilog = std::string(infoLog.get());
    std::cerr << ilog << std::endl;
    glDeleteShader(shader.shaderObject);
    return 0; 
  }
  return shader.shaderObject;
}

std::unordered_map<std::string,int>& GLProgram::getUniforms(){
  return this->uniforms;
}

std::vector<Shader>& GLProgram::getShaders(){
  return this->shaders;
}

std::unordered_map<std::string,Attrib>& GLProgram::getAttrLoc(){ return attrLoc; }

std::unordered_map<std::string,int>& GLProgram::getOutputLoc(){ return outputLoc; }

std::unordered_map<std::string,int>& GLProgram::getTexUnits() { return texUnits; }

int GLProgram::makeProgram(){
  int programOk;
  int programValid;
  this->program = glCreateProgram();
  for( auto& shader : this->shaders){
    if (shader.shaderObject == 0){
      this->compileShader(shader);
    }
    glAttachShader(this->program, shader.shaderObject);
  }
  glLinkProgram(this->program);
  glGetProgramiv(this->program, GL_LINK_STATUS, &programOk);
  glValidateProgram(program);
  glGetProgramiv(this->program, GL_VALIDATE_STATUS, &programValid);
  if (!programOk || !programValid) {
    int maxLength = 0;
    std::cerr << "Failed to link shader program:"<< std::endl;
    glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &maxLength);
    auto infoLog = std::unique_ptr<char[]>(new char[maxLength]);
    glGetProgramInfoLog(this->program, maxLength, &maxLength, &infoLog[0]);
    std::string ilog = std::string(infoLog.get());
    std::cerr << ilog << std::endl;
    glDeleteProgram(this->program);
    for( auto shader : this->shaders){
      glDetachShader(this->program,shader.shaderObject);
      glDeleteShader(shader.shaderObject);
    }
    return 0;
  }
  for( auto shader : this->shaders){
   glDetachShader(this->program,shader.shaderObject);
   glDeleteShader(shader.shaderObject);
  }
  return this->program;
}