#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include <string>
#include <unordered_map>
#include <vector>
#include <stack>

struct shader{
  std::string name;
  std::string type;
  std::string source;
  int shaderObject = 0;
};
typedef struct shader Shader;

struct attrib{
  int location = -1;
  bool initialized = false;
};
typedef struct attrib Attrib;

class GLProgram{
private:
  int program;
  std::stack<int> freeTexUnits;
  std::unordered_map<std::string,int> uniforms;
  std::unordered_map<std::string,Attrib> attrLoc;
  std::unordered_map<std::string,int> texUnits;
  std::unordered_map<std::string,int> outputLoc;
  std::vector<Shader> shaders;
public:
  GLProgram();
  int getProgram()const;
  std::string getSourceFromFile(std::string filename);
  GLProgram& addShader(std::string type, std::string src);
  int compileShader(Shader& shader);
  std::unordered_map<std::string,int>& getUniforms();
  std::vector<Shader>& getShaders();
  int makeProgram();
  std::unordered_map<std::string,Attrib>& getAttrLoc();
  std::unordered_map<std::string,int>& getOutputLoc();
  std::unordered_map<std::string,int>& getTexUnits();
  int getFreeTextureUnit();
  GLProgram& releaseTextureUnit(int unit);
};

#endif