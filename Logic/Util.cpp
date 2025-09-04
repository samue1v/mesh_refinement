#include "Util.hpp"
#include "Matrix.hpp"

bool Misc::Util::onSegment(glm::vec3 p, glm::vec3 q, glm::vec3 r) 
{ 
  if ((q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && 
      q.y < std::max(p.y, r.y) && q.y > std::min(p.y, r.y)) 
      || 
      (q.x < std::max(p.x, r.x) && q.x > std::min(p.x, r.x) && 
      q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))){ 
    return true; 
  }
  return false; 
} 

// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are collinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int Misc::Util::orientation(glm::vec3 p, glm::vec3 q, glm::vec3 r) 
{ 
    float val = (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y); 
    //float val = glm::cross(r - q, p - r).z;
    //float val = glm::cross(r - q,p - r).z;
    //if (almostZero(val)) return 0;  // collinear 
  //if (val == 0) return 0;
  //if(Misc::Util::almostZero(val,7)) return 0;
  if(Misc::Util::almostZero(val,12)) return 0;
  return (val > 0)? 1: 2; // clock or counterclock wise 
} 

  // The main function that returns true if line segment 'p1q1' 
  // and 'p2q2' intersect. 
bool Misc::Util::doIntersect(glm::vec3 p1, glm::vec3 q1, glm::vec3 p2, glm::vec3 q2) { 


      
  // Find the four orientations needed for general and 
  // special cases 
  int o1 = orientation(p1, q1, p2); 
  int o2 = orientation(p1, q1, q2); 
  int o3 = orientation(p2, q2, p1); 
  int o4 = orientation(p2, q2, q1); 

  // General case 
  if(o1!=0 && o2!=0 && o3!=0 && o4!=0){
    if (o1 != o2 && o3 != o4) return true; 
  }

  // Special Cases 
  // p1, q1 and p2 are collinear and p2 lies on segment p1q1 
  if (o1 == 0 && onSegment(p1, p2, q1)) return true; 

  // p1, q1 and q2 are collinear and q2 lies on segment p1q1 
  if (o2 == 0 && onSegment(p1, q2, q1)) return true; 

  // p2, q2 and p1 are collinear and p1 lies on segment p2q2 
  if (o3 == 0 && onSegment(p2, p1, q2)) return true; 

   // p2, q2 and q1 are collinear and q1 lies on segment p2q2 
  if (o4 == 0 && onSegment(p2, q1, q2)) return true; 

  return false; // Doesn't fall in any of the above cases 
  }




  float Misc::Util::angle(glm::vec3 p, glm::vec3 r, glm::vec3 q){
    return glm::dot(glm::normalize(p - r),glm::normalize(q - r));

  }


//devem estar ordenados
bool Misc::Util::crossCompare(glm::vec3 p, glm::vec3 c, glm::vec3 n){
  float s = glm::cross(c - p, n - c).z;
  if(s>=0){
    return true;
  }
  return false;
  }


// Using 3 points of a circunscribed triangle
bool Misc::Util::pointInCircle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 p, float tolerance){
  //float deter = Matrix<float>(4,4,{a[0], a[1], a[0]*a[0]+a[1]*a[1], 1,
  //                                            b[0], b[1], b[0]*b[0]+b[1]*b[1], 1,
  //                                            c[0], c[1], c[0]*c[0]+c[1]*c[1], 1,
  //                                            p[0], p[1], p[0]*p[0]+p[1]*p[1], 1}).det();
  //experimentando mudar a tolerancia.

  float deter = Matrix<float>(3,3,{           a[0] - p[0], a[1] - p[1], (a[0]-p[0])*(a[0]-p[0])+(a[1]-p[1])*(a[1]-p[1]),
                                              b[0] - p[0], b[1] - p[1], (b[0]-p[0])*(b[0]-p[0])+(b[1]-p[1])*(b[1]-p[1]),
                                              c[0] - p[0], c[1] - p[1], (c[0]-p[0])*(c[0]-p[0])+(c[1]-p[1])*(c[1]-p[1])}).det();
  //if(std::abs(deter) < 0.1f) {deter = 0.f;}
  return deter > tolerance ? true : false;
}

// Using center and radius
bool Misc::Util::pointInCircle(glm::vec3 point, glm::vec3 center, float radius){
    //bool isInside = radius - glm::length(point - center)>= 10e-5 ? 1 : 0;
    bool isInside = glm::length(point - center) <=radius ? 1 : 0;
    return isInside;
}

bool Misc::Util::almostZero(double num,int tol){
  bool isZero = std::abs(num) <= exp10(-tol) ? true : false;
  return isZero;
}

bool Misc::Util::pointInsideTriangle(glm::vec3 p, glm::vec3 p0, glm::vec3 p1,glm::vec3 p2){
  glm::vec3 pp0,pp1,pp2; 
  pp0 = p0-p;
  pp1 = p1-p;
  pp2 = p2-p;
  float res1 = (pp0.x*pp1.y) - (pp0.y*pp1.x);
  float res2 = (pp1.x*pp2.y) - (pp1.y*pp2.x);
  float res3 = (pp2.x*pp0.y) - (pp2.y*pp0.x);
  if(res1>0 && res2 > 0 && res3 > 0){
    return true;
  }
  return false;
}


glm::vec3 Misc::Util::edgeMiddle(glm::vec3 p1, glm::vec3 p2){
  glm::vec3 mp = (p1+p2)/2.f;
  return mp;
}


float Misc::Util::edgeLength(glm::vec3 p1, glm::vec3 p2){
  float w = p1.x - p2.x;
  float h = p1.y - p2.y;
  return sqrtf(w*w + h*h);
}