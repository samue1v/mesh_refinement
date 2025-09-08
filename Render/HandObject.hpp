#ifndef HANDOBJECT_HPP
#define HANDOBJECT_HPP
#include "../Logic/Frames.hpp"
#include "../Logic/Triangle.hpp"
#include "../Logic/vec3.hpp"
#include "GLObject.hpp"
#include "QuadTree.hpp"
#include <QObject>
#include <vector>

class HandObject : public GLTriMesh {
  Q_OBJECT
public:
  HandObject(Scene *scene = 0, QOpenGLContext *context = 0);
  HandObject(const HandObject &copyObj) = delete;
  HandObject(const HandObject &&moveObj) = delete;
  ~HandObject();

  HandObject &operator=(const HandObject &copyObj) = delete;
  HandObject &operator=(const HandObject &&moveObj) = delete;

  void init() override;
  void draw() override;
  void restoreOriginal() override;
  bool isQuadVisible();
  void setQuadVisible();
  QuadTree *getQuadTree();

private:
  QuadTree *tree;
};

#endif
