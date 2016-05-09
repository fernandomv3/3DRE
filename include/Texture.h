#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <memory>
class Texture{
private:
  std::string uuid;
  std::string sourceFile;
  std::unique_ptr<char[]>image;
  int height;
  int width;
  bool gamma;
public:
  Texture(const std::string& sourceFile);

  const std::string& getUUID()const;
  const std::string& getSourceFile()const;
  const char* getImage()const;
  int getHeight()const;
  int getWidth()const;
  bool getGamma()const;
  Texture& setGamma(bool gamma);

  Texture& loadFile();
};
#endif