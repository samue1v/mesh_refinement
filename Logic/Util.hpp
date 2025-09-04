#ifndef UTIL_H
#define UTIL_H

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <cmath>

namespace Misc{

  static float SQRT3 = sqrtf(3.f);

  class Util{
  public:
    static bool onSegment(glm::vec3 p, glm::vec3 q, glm::vec3 r);
    static int orientation(glm::vec3 p, glm::vec3 q, glm::vec3 r);
    static bool doIntersect(glm::vec3 p1, glm::vec3 q1, glm::vec3 p2, glm::vec3 q2); 
    static bool crossCompare(glm::vec3 p, glm::vec3 c, glm::vec3 n);
    static bool pointInCircle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 p, float tolerance);
    static bool pointInCircle(glm::vec3 point, glm::vec3 center, float radius);
    static float angle(glm::vec3 p, glm::vec3 r, glm::vec3 q);
    static bool almostZero(double num,int tol = 15);
    static bool pointInsideTriangle(glm::vec3 p, glm::vec3 v0, glm::vec3 v1,glm::vec3 v2);
    static glm::vec3 edgeMiddle(glm::vec3 p1, glm::vec3 p2);
    static float edgeLength(glm::vec3 p1, glm::vec3 p2);
  };
  


  
};

#endif