#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <memory>
#include <typeindex>

class Texture{
private:
  std::string uuid;
  std::string sourceFile;
  std::unique_ptr<char[]>image;
  int height;
  int width;
  int nMipmaps;
  std::tuple<std::string,std::string,std::string> wrapping;
  std::pair<std::string,std::string> filtering;
  std::string target;
  std::string format;
  std::string innerFormat;
  bool gamma;
  bool loaded;
  std::type_index type = std::type_index(typeid(unsigned short));
public:
  Texture(const std::string& sourceFile);
  Texture(int width, int height);
  const std::string& getUUID()const;
  const std::string& getSourceFile()const;
  const char* getImage()const;
  Texture& loadFile();
  int getHeight()const;
  int getWidth()const;
  bool getGamma()const;
  int getNMipmaps()const;
  std::tuple<std::string,std::string,std::string> getWrapping()const;
  std::pair<std::string,std::string> getFiltering()const;
  std::string getTarget()const;
  bool getLoaded()const;
  std::string getFormat()const;
  std::string getInnerFormat()const;
  std::type_index getType()const;

  Texture& setNMipmaps(int nMipmaps);
  Texture& setGamma(bool gamma);
  Texture& setWrapping(std::tuple<std::string,std::string,std::string> wrapping);
  Texture& setFiltering(std::pair<std::string,std::string> filtering);
  Texture& setTarget(std::string target);
  Texture& setLoaded(bool loaded);
  Texture& setFormat(std::string format);
  Texture& setInnerFormat(std::string innerFormat);
  Texture& setType(std::type_index type);
};
#endif