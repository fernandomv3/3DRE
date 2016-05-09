#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <string>

class Geometry{
private:
  std::string uuid;
  std::vector<float> vertices;
  std::vector<int> elements;
  std::vector<float> normals;
  std::vector<float> texCoords;
  std::vector<float> tangents;
  std::vector<float> bitangents;
public:
  Geometry();

  std::string getUUID()const;

  const std::vector<float>& getVertices()const;
  const std::vector<int>& getElements()const;
  const std::vector<float>& getNormals()const;
  const std::vector<float>& getTexCoords()const;
  const std::vector<float>& getTangents()const;
  const std::vector<float>& getBitangents()const;

  Geometry& setVertices(std::vector<float> vertices);
  Geometry& setElements(std::vector<int> elements);
  Geometry& setNormals(std::vector<float> normals);
  Geometry& setTexCoords(std::vector<float> texCoords);
  Geometry& setTangents(std::vector<float> tangents);
  Geometry& setBitangents(std::vector<float> bitangents);
};
Geometry loadDataFromFile(std::string filename);
Geometry quadGeometry(int size =1);

#endif