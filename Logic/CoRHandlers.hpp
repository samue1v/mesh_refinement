#ifndef COR_HANDLERS_H
#define COR_HANDLERS_H

#include "AdjList.hpp"
#include "Frames.hpp"
#include "Matrix.hpp"
#include "Util.hpp"
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <queue>
#include <unordered_set>

class GLSimpleMesh;
class GLTriMesh;

// Handler Interface
class Handler {
public:
  virtual Handler *setNext(Handler *) = 0;
  virtual GLSimpleMesh *handle(GLSimpleMesh *) = 0;
  virtual ~Handler() = 0;
};

class AbsHandler : public Handler {

public:
  AbsHandler();
  virtual ~AbsHandler();
  Handler *setNext(Handler *_handler) override;
  GLSimpleMesh *handle(GLSimpleMesh *request) override;

private:
  Handler *nexthandler;
};

class TransformationHandler : public AbsHandler {
public:
  TransformationHandler(Matrix<float> m = Matrix<float>());
  GLSimpleMesh *handle(GLSimpleMesh *request) override;

protected:
  Matrix<float> transformMatrix;
};

class PCAHandler : public TransformationHandler {
public:
  PCAHandler(Matrix<float> m = Matrix<float>());
  GLSimpleMesh *handle(GLSimpleMesh *request) override;

  Matrix<double> *getMean();
  Matrix<double> *getCov();
  std::vector<Matrix<double>> *getEigen();

private:
  Matrix<double> mean;
  Matrix<double> cov;
  std::vector<Matrix<double>> eigen;
};

class PCAInverseHandler : public TransformationHandler {
public:
  PCAInverseHandler(PCAHandler *_pcaHandler = nullptr);
  GLSimpleMesh *handle(GLSimpleMesh *request) override;

private:
  PCAHandler *pcaHandler;
};

class RotationHandler : public TransformationHandler {

public:
  RotationHandler(float degree = 0.f);
  RotationHandler(Matrix<float> m = Matrix<float>());
  GLSimpleMesh *handle(GLSimpleMesh *request) override;

private:
  float radians;
};

class ScaleHandler : public TransformationHandler {

public:
  ScaleHandler(float factor = 1.f);
  ScaleHandler(Matrix<float> m = Matrix<float>());
  GLSimpleMesh *handle(GLSimpleMesh *request) override;

private:
  float scaleFactor;
};

class NormalizationHandler : public TransformationHandler {

public:
  NormalizationHandler();
  GLSimpleMesh *handle(GLSimpleMesh *request) override;
};

class TriangulationHandler : public AbsHandler {

public:
  GLSimpleMesh *handle(GLSimpleMesh *request) override;
  void makeTriangulation(GLTriMesh *obj);

private:
  void check(const std::pair<uint, uint> &edge);
  bool isDone(const std::pair<uint, uint> &edge);
  int delaunay(const std::pair<uint, uint> &edge);
  uint euclidian(Frame &frame1, const std::pair<uint, uint> &edge);
  glm::vec3 computeRightVertex(const std::pair<uint, uint> &edge);
  float computeRightTriangleHeight(const std::pair<uint, uint> &edge);
  bool doIntersectOtherEdges(const std::pair<glm::vec3, glm::vec3> &e1,
                             const std::pair<glm::vec3, glm::vec3> &e2);
  bool doIntersectOtherEdges(const std::pair<uint, uint> &baseEdge,
                             const std::pair<glm::vec3, glm::vec3> &e1,
                             const std::pair<glm::vec3, glm::vec3> &e2);
  bool doTresPass(uint idxP0, uint idxP1, uint idxP2);
  bool doTresPass(uint idxP0, uint idxP1, glm::vec3 pRet);
  void fillNeighbours(uint p0, uint p1, uint p2);
  void smoothTriangulation();
  void prepareFrames();
  void classifyTtriangles();
  void updateEdgeMap();
  void debug();
  glm::vec3 computeCircunsCenter(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);
  float computeCircunsRadius(glm::vec3 p0, glm::vec3 center);

private:
  AdjList<char> adjList;
  std::queue<std::pair<uint, uint>> queue;
  GLTriMesh *obj;
};

class QuadTreeHandler : public TransformationHandler {

public:
  QuadTreeHandler();
  GLSimpleMesh *handle(GLSimpleMesh *request) override;
};

class RemeshBadRegions : public AbsHandler {

public:
  RemeshBadRegions();
  GLSimpleMesh *handle(GLSimpleMesh *request) override;

private:
  void BFSMesh(GLSimpleMesh *mesh, uint triangle_idx);

private:
  std::vector<uint> sortedIndices;
  std::unordered_set<std::pair<uint, uint>, KeyHasher, KeyEquals> boundaryEdges;

  std::unordered_set<uint> regionTriangles;
  GLSimpleMesh *targetMesh;
};

#endif
