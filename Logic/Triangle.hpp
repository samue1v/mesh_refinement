#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <QLinearGradient>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <set>
#include <array>

static glm::vec3 red(1,0,0);
static glm::vec3 green(0,1,0);




struct Vertex{
  glm::vec3 position = glm::vec3();
  glm::vec3 color = glm::vec3();
  bool isActive = false;
  std::set<uint> neighbours;
};


class Triangle{

public:
  Triangle() = default;
  Triangle(int idx0, int idx1, int idx2,Vertex & v1, Vertex & v2, Vertex & v3);
  ~Triangle();

  void setPoints(Vertex & v1, Vertex & v2, Vertex & v3);
  void setIndexes(int idx0, int idx1, int idx2);
  float classify();

  
  static glm::vec3 getColorFromGradient(float v);


//private:
  //Vertex * vertices[3];
  std::array<Vertex*, 3> vertices;
  int indexes[3];
  glm::vec3 color;
  float score;
  
};

inline void Triangle::setPoints(Vertex & v1 , Vertex & v2, Vertex & v3){
  vertices[0] = &v1;
  vertices[1] = &v2;
  vertices[2] = &v3;
}

inline void Triangle::setIndexes(int idx0,int idx1,int idx2){
  indexes[0] = idx0;
  indexes[1] = idx1;
  indexes[2] = idx2;
}


#endif
