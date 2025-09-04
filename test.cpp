#include <iostream>
#include "vec3.hpp"


float classify(Vec3 v1 , Vec3 v2 , Vec3 v3){
    float area = cross((v1 - v2), (v3 - v2)).len();
    float l1 = (v1 - v2).len();
    float l2 = (v3 - v2).len();
    float l3 = (v3 - v1).len();

    float inscritRadius = area / (l1+l2+l3);
    float sinI = area / ((v1 - v2).len() * (v3 - v2).len());
    
    float circunRadius = 0.5 * l3/sinI;

    return circunRadius / inscritRadius;
    
}



int main(){
  float r = classify(Vec3(3,0,0),Vec3(6,0,0),Vec3(6,4,0));
  float e = classify(Vec3(3,0,0)*7,Vec3(6,0,0)*7,Vec3(4.5,2.59,0)*7);  
  float s = classify(Vec3(3,4,0),Vec3(5,7,0),Vec3(13.5,8.5,0));  

  std::cout<<r << std::endl;
  std::cout<<e << std::endl;
  std::cout<<s << std::endl;
  
}

