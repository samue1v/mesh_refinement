#ifndef GL_OBJECT_H
#define GL_OBJECT_H

#include <stdlib.h>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVersionFunctionsFactory>
#include <QDir>
#include <QString>
#include <vector>
#include <QObject>
#include <string>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include "GLProgram.hpp"
#include "../Logic/Triangle.hpp"
#include "../Logic/CoRHandlers.hpp"
#include "../Logic/Frames.hpp"





class Scene;

class GLInterface{
public:
  virtual void init() = 0;
  virtual void draw() = 0;
};

class GLObject : public QObject, public GLInterface{
  Q_OBJECT
  public:
  GLObject(Scene * scene = 0,QOpenGLContext * context = 0);
  GLObject(const GLObject & copyObj);
  GLObject(GLObject && other) = delete;
  // It should have an interface for draw and init but not for now
  virtual ~GLObject();
  virtual void draw();
  virtual void init();
  virtual void restoreOriginal();

  GLObject & operator=(GLObject&& other) = delete;
  GLObject & operator=(const GLObject& copyObj);

  public:
  void addNewVertex(glm::vec3 pos, glm::vec3 color,bool isActive);

  bool isPointInside(glm::vec3 p);

  void execHandlers();

  std::vector<Vertex> * getLocalPoints();

  std::vector<Triangle> * getTriangles();

  std::vector<std::pair<uint,uint>> * getLines();

  std::vector<uint> * getLocalIndexes();

  void addFrame(Frame f);

  std::vector<Frame> getFrames();

  GLProgram & getProgram(uint idx);

  bool nextFrame();

  bool previousFrame();

  Triangle getCurrentTriangle();

  int getCurrentFrame();

  Scene * getScene();

  AbsHandler * getBaseHandler();

  void setbaseHandler(AbsHandler *);

  int pickTriangle(glm::vec3 point);

  void setVisible(bool vis);

  bool isVisible();

  int getGlobalIndexFromVertex(Vertex v);

  glm::vec3 getCenter();

  glm::vec3 maxValAxis();
  
  glm::vec3 minValAxis();

  //Vertex getGlobalVertexFromIndex(uint idx);

  uint addVertexToScene(Vertex v);
  public:

  int sourceFilePointsSize;
  int sourceFileLinesSize;
  int sourceFileTrianglesSize;

  std::string name;




  protected:
  std::vector<GLProgram> program;
  QOpenGLFunctions_3_3_Core * f;
  QOpenGLContext * currentContext;
  std::vector<uint> VBO;
  std::vector<uint> VAO;
  std::vector<uint> EBO;

  std::vector<Vertex> localPoints;
  // indexes of global(Scene) points
  std::vector<std::pair<uint,uint>> lines;
  //triangle vector
  std::vector<Triangle> triangles;
  //std::vector<uint> pointIndexes;
  std::vector<uint> localPointIndexes;
  std::vector<Frame> frames;

  AbsHandler * baseHandler;

  int currentFrame;

  bool visibility;

  Scene * currentScene;




};

#endif
