#ifndef COR_HANDLERS_H
#define COR_HANDLERS_H

#include "Matrix.hpp"
#include "Util.hpp"
#include "Frames.hpp"
#include "AdjList.hpp"
#include <queue>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

class GLObject;

//Handler Interface
class Handler{
  public:
  virtual Handler* setNext(Handler *) = 0;
  virtual GLObject* handle(GLObject *) = 0;
  virtual ~Handler() = 0;
};


class AbsHandler : public Handler{

  public:
  AbsHandler();
  virtual ~AbsHandler();
  Handler * setNext(Handler * _handler) override;
  GLObject * handle(GLObject * request) override;
  
  private:
  Handler * nexthandler;

};

class TransformationHandler : public AbsHandler{
  public:
  TransformationHandler(Matrix<float> m = Matrix<float>());
  GLObject * handle(GLObject * request) override;

  protected:
  Matrix<float> transformMatrix;
};

class PCAHandler : public TransformationHandler{
  public:
  PCAHandler(Matrix<float> m = Matrix<float>());
  GLObject * handle(GLObject * request) override;

  Matrix<double> * getMean();
  Matrix<double> * getCov();
  std::vector<Matrix<double>> * getEigen();


  private:
  Matrix<double> mean;
  Matrix<double> cov;
  std::vector<Matrix<double>> eigen;
};

class PCAInverseHandler : public TransformationHandler{
  public:
  PCAInverseHandler(PCAHandler * _pcaHandler = nullptr);
  GLObject * handle(GLObject * request) override;

  private:
  PCAHandler * pcaHandler;
};


class RotationHandler : public TransformationHandler{

  public:
  RotationHandler(float degree = 0.f);
  RotationHandler(Matrix<float> m = Matrix<float>());
  GLObject * handle(GLObject * request) override;


  private:
  float radians;


};

class ScaleHandler : public TransformationHandler{

  public:
  ScaleHandler(float factor = 1.f);
  ScaleHandler(Matrix<float> m = Matrix<float>());
  GLObject * handle(GLObject * request) override;


  private:
  float scaleFactor;


};

class NormalizationHandler : public TransformationHandler{

  public:
  NormalizationHandler();
  GLObject * handle(GLObject * request) override;
};

class TriangulationHandler : public AbsHandler{

  public:
  GLObject * handle(GLObject * request) override;
  void makeTriangulation(GLObject * obj);

  private:
  void check(const std::pair<uint,uint> & edge);
  bool isDone(const std::pair<uint,uint> & edge);
  int delaunay(const std::pair<uint,uint> & edge);
  uint euclidian(Frame & frame1,const std::pair<uint,uint> & edge);
  glm::vec3 computeRightVertex(const std::pair<uint,uint> & edge);
  //uint addNewVertex(glm::vec3 vec);
  float computeRightTriangleHeight(const std::pair<uint,uint> & edge);
  bool doIntersectOtherEdges(const std::pair<glm::vec3,glm::vec3> & e1,const std::pair<glm::vec3,glm::vec3> & e2);
  bool doIntersectOtherEdges(const std::pair<uint,uint> & baseEdge,const std::pair<glm::vec3,glm::vec3> & e1,const std::pair<glm::vec3,glm::vec3> & e2);
  bool doTresPass(uint idxP0,uint idxP1,uint idxP2);
  bool doTresPass(uint idxP0,uint idxP1, glm::vec3 pRet);
  void fillNeighbours(uint p0, uint p1, uint p2);
  void smoothTriangulation();
  void prepareFrames();
  void classifyTtriangles();
  void debug();
  glm::vec3 computeCircunsCenter(glm::vec3 p0,glm::vec3 p1, glm::vec3 p2);
  float computeCircunsRadius(glm::vec3 p0, glm::vec3 center);
  private:
  AdjList adjList;
  std::queue<std::pair<uint,uint>> queue;
  GLObject * obj;
};

class QuadTreeHandler : public TransformationHandler{

  public:
  QuadTreeHandler();
  GLObject * handle(GLObject * request) override;
};

#endif
