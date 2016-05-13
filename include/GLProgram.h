#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include <string>
#include <unordered_map>
#include <vector>

struct shader{
  std::string name;
  std::string type;
  std::string source;
  int shaderObject = 0;
};
typedef struct shader Shader;

class GLProgram{
private:
  int program;
  std::unordered_map<std::string,int> uniforms;
  std::unordered_map<std::string,int> attrLoc;
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
  std::unordered_map<std::string,int>& getAttrLoc();
};

#endif