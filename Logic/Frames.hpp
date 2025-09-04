#ifndef FRAMES_H
#define FRAMES_H
#include "Triangle.hpp"

/* struct Frame{
  std::vector<Triangle> triangles;
  std::vector<Vertex> points;
  std::vector<std::pair<uint,uint>> lines;

  virtual int calcSize(){
    return triangles.size() + lines.size() + points.size();
  }
  virtual int calcBytesSize(){
    return triangles.size()*sizeof(Triangle) + lines.size()*sizeof(std::pair<uint,uint>) 
    + points.size()*sizeof(Vertex);
  }
  virtual int trianglesOffset(){
    return 0;
  }
  virtual int pointsOffset(){
    return triangles.size()*sizeof(Triangle);
  }
  virtual int linesOffset(){
    return triangles.size()*sizeof(Triangle) + points.size()*sizeof(Vertex);
  }
}; */

struct Frame{
  uint PointBegin;
  ulong PointEnd;

  uint LineBegin;
  ulong LineEnd;

  uint TriangleBegin;
  ulong TriangleEnd;

  glm::vec3 center;
  float radius;
  int caso;
};


#endif