#include "Geometry.h"
#include "MathUtils.h"

Geometry::Geometry(){
  this->uuid = generateUUID();
}

std::string Geometry::getUUID()const{ return uuid; }

const std::vector<float>& Geometry::getVertices()const{ return vertices; }
const std::vector<int>& Geometry::getElements()const{ return elements; }
const std::vector<float>& Geometry::getNormals()const{ return normals; }
const std::vector<float>& Geometry::getTexCoords()const{ return texCoords; }
const std::vector<float>& Geometry::getTangents()const{ return tangents; }
const std::vector<float>& Geometry::getBitangents()const{ return bitangents; }

Geometry& Geometry::setVertices(std::vector<float> vertices){
  this->vertices = vertices;
  return *this;
}
Geometry& Geometry::setElements(std::vector<int> elements){
  this->elements = elements;
  return *this;
}
Geometry& Geometry::setNormals(std::vector<float> normals){
  this->normals = normals;
  return *this;
}
Geometry& Geometry::setTexCoords(std::vector<float> texCoords){
  this->texCoords = texCoords;
  return *this;
}
Geometry& Geometry::setTangents(std::vector<float> tangents){
  this->tangents = tangents;
  return *this;
}
Geometry& Geometry::setBitangents(std::vector<float> bitangents){
  this->bitangents = bitangents;
  return *this;
}

std::vector< std::tuple<std::string,void*,int,int,std::type_index> > Geometry::getAttributes() const{
  std::vector< std::tuple<std::string,void*,int,int,std::type_index> > vec;
  
  if(!elements.empty())
    vec.push_back(std::make_tuple("index",(void*)elements.data(),elements.size(),1,std::type_index(typeid(ushort))));
  if(!vertices.empty())
    vec.push_back(std::make_tuple("vPosition",(void*)vertices.data(),vertices.size(),3,std::type_index(typeid(float))));
  if(!normals.empty())
    vec.push_back(std::make_tuple("vNormal",(void*)normals.data(),normals.size(),3,std::type_index(typeid(float))));
  if(!texCoords.empty())
    vec.push_back(std::make_tuple("vUv",(void*)texCoords.data(),texCoords.size(),2,std::type_index(typeid(float))));
  if(!tangents.empty())
    vec.push_back(std::make_tuple("vTangent",(void*)tangents.data(),tangents.size(),3,std::type_index(typeid(float))));
  return vec;
}

Geometry loadDataFromFile(std::string filename){ return Geometry(); }

Geometry quadGeometry(int size){
  float vertices[18] = {
    1.0, -1.0, 0.0,
    1.0, 1.0, 0.0,
    -1.0, 1.0, 0.0,
    -1.0, -1.0, 0.0,
    1.0, -1.0, 0.0,
    -1.0, 1.0, 0.0
  };
  for(int i=0;i<18;i++){ vertices[i] *= size; }
  float normals[18] = {
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0
  };

  Geometry geom = Geometry();
  geom.setVertices(std::vector<float>(vertices,vertices+18));
  geom.setNormals(std::vector<float>(normals,normals+18));
  return geom;
}