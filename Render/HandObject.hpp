#ifndef HANDOBJECT_HPP
#define HANDOBJECT_HPP
#include <QObject>
#include <vector>
#include "GLObject.hpp"
#include "QuadTree.hpp"
#include "../Logic/Triangle.hpp"
#include "../Logic/Frames.hpp"
#include "../Logic/vec3.hpp"



class HandObject : public GLObject{
Q_OBJECT
  public:
  HandObject(Scene * scene = 0,QOpenGLContext * context = 0);
  HandObject(const HandObject & copyObj );
  HandObject(const HandObject && moveObj) = delete;
  ~HandObject();

  HandObject & operator=(const HandObject & copyObj);
  HandObject & operator=(const HandObject && moveObj) = delete;

  void init() override;
  void draw() override;
  void restoreOriginal() override;
  bool isQuadVisible();
  void setQuadVisible();
  QuadTree * getQuadTree();

  private:
  QuadTree * tree;
  


};


#endif