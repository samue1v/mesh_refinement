#ifndef GL_OBJECT_H
#define GL_OBJECT_H

#include "../Logic/CoRHandlers.hpp"
#include "../Logic/Frames.hpp"
#include "../Logic/Triangle.hpp"
#include "../Logic/AdjList.hpp"
#include "GLProgram.hpp"
#include <QDir>
#include <QObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVersionFunctionsFactory>
#include <QString>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <stdlib.h>
#include <string>
#include <vector>

class Scene;

class GLInterface {
public:
  virtual ~GLInterface() = default;
  virtual void init() = 0;
  virtual void draw() = 0;
};

class GLDrawable : public QObject, public GLInterface {
  Q_OBJECT
public:
  GLDrawable(QOpenGLContext *context);
  GLDrawable(const GLDrawable &) = delete;
  GLDrawable &operator=(const GLDrawable &) = delete;
  ~GLDrawable();

  bool isVisible();
  void setVisible(const bool vis);

  virtual void draw() = 0;
  virtual void init() = 0;

public:
  QOpenGLContext *currentContext;
  bool visibility;
protected:
  std::vector<GLProgram> program;
  std::string shader;
  QOpenGLFunctions_3_3_Core *f;
  std::vector<uint> VBO;
  std::vector<uint> VAO;
  std::vector<uint> EBO;
};

// Simple mesh interface for primitives and simple objects.
class GLSimpleMesh : public GLDrawable {
public:
  GLSimpleMesh(QOpenGLContext *context, Scene *_scene);
  GLSimpleMesh(const GLSimpleMesh &copyObj) = delete;
  GLSimpleMesh(GLSimpleMesh &&other) = delete;
  ~GLSimpleMesh();
  virtual void draw();
  virtual void init();

  GLSimpleMesh &operator=(GLSimpleMesh &&other) = delete;
  GLSimpleMesh &operator=(const GLSimpleMesh &copyObj) = delete;

  void addNewVertex(glm::vec3 pos, glm::vec3 color, bool isActive);

  bool isPointInside(glm::vec3 p);

  std::vector<Vertex> *getPoints();

  std::vector<uint> *getIndexes();

  std::vector<std::pair<uint, uint>> *getLines();

  glm::vec3 getCenter();

  glm::vec3 maxValAxis();

  glm::vec3 minValAxis();

  AbsHandler *getBaseHandler();

  void execHandlers();

  void setbaseHandler(AbsHandler *);  

  std::vector<Triangle> *getTriangles();

public:
  std::vector<Vertex> points;
  std::vector<uint32_t> indexes;
  std::vector<std::pair<uint, uint>> lines;
  std::vector<Triangle> triangles;
  AdjList<std::vector<uint>> edgeFaceMap;
  AbsHandler *baseHandler;

public:
  int sourceFilePointsSize;
  int sourceFileLinesSize;
  int sourceFileTrianglesSize;

  std::string name;

  Scene *currentScene;
};

// Mesh interface for scene and complex algortihms
class GLTriMesh : public GLSimpleMesh {
public:
  GLTriMesh(Scene *scene, QOpenGLContext *context);
  ~GLTriMesh();
  GLTriMesh(const GLTriMesh &copyObj) = delete;
  GLTriMesh(GLTriMesh &&other) = delete;

  virtual void draw();
  virtual void init();
  virtual void restoreOriginal();

  GLTriMesh &operator=(GLTriMesh &&other) = delete;
  GLTriMesh &operator=(const GLTriMesh &copyObj) = delete;

public:



  void addFrame(Frame f);

  std::vector<Frame> getFrames();

  GLProgram &getProgram(uint idx);

  bool nextFrame();

  bool previousFrame();

  Triangle getCurrentTriangle();

  int getCurrentFrame();

  Scene *getScene();

  int pickTriangle(glm::vec3 point);

  int getGlobalIndexFromVertex(Vertex v);

  // Vertex getGlobalVertexFromIndex(uint idx);

  uint addVertexToScene(Vertex v);

protected:
  // triangle vector
  std::vector<Frame> frames;


  int currentFrame;
};

#endif
