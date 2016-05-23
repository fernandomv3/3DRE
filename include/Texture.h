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
  bool gamma;
  bool alpha;
  int dimensions;
  std::string wrapping;
  std::string filtering;
  std::string target;
  bool loaded;
  std::type_index type = std::type_index(typeid(unsigned char));
public:
  Texture(const std::string& sourceFile);

  const std::string& getUUID()const;
  const std::string& getSourceFile()const;
  const char* getImage()const;
  int getHeight()const;
  int getWidth()const;
  bool getGamma()const;
  Texture& setGamma(bool gamma);
  bool hasAlpha()const;
  Texture& loadFile();
  int getDimensions()const;
  std::string getWrapping()const;
  std::string getFiltering()const;
  std::string getTarget()const;
  bool getLoaded()const;
  bool getAlpha()const;
  std::type_index getType()const;
  Texture& setDimensions(int dimensions);
  Texture& setWrapping(std::string wrapping);
  Texture& setFiltering(std::string filtering);
  Texture& setTarget(std::string target);
  Texture& setLoaded(bool loaded);
};
#endif