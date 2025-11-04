#include "CoRHandlers.hpp"
#include "../Render/GLObject.hpp"
#include "../Render/HandObject.hpp"

// Interface

Handler::~Handler() {}

// ABSHANDLER METHODS

AbsHandler::AbsHandler() : nexthandler(nullptr) {}

Handler *AbsHandler::setNext(Handler *_handler) {
  nexthandler = _handler;
  return _handler;
}

GLSimpleMesh *AbsHandler::handle(GLSimpleMesh *request) {
  if (nexthandler) {
    return nexthandler->handle(request);
  }
  return {};
}

AbsHandler::~AbsHandler() { delete nexthandler; }

// TRANSFORMATION HANDLER

TransformationHandler::TransformationHandler(Matrix<float> m)
    : transformMatrix(m) {}

GLSimpleMesh *TransformationHandler::handle(GLSimpleMesh *request) {

  std::vector<Vertex> *points = request->getPoints();
  for (int i = 0; i < points->size(); i++) {
    points->at(i).position = transformMatrix * points->at(i).position;
  }
  return AbsHandler::handle(request);
}

// PCA HANDLER

PCAHandler::PCAHandler(Matrix<float> m) : TransformationHandler(m) {}

GLSimpleMesh *PCAHandler::handle(GLSimpleMesh *request) {

  std::vector<Vertex> *points = request->getPoints();

  std::vector<Matrix<double>> vec;
  for (int i = 0; i < points->size(); i++) {
    Matrix<double> temp(points->at(i).position, 1);
    vec.push_back(temp);
  }

  Matrix<double> stacked = Matrix<double>::stackVectors(vec, 1);
  mean = Matrix<double>::columnMean(stacked, false);
  cov = Matrix<double>::covariance(stacked);
  eigen = Matrix<double>::symmetricSVD(cov);

  Matrix<double> transformRes = (eigen[0]) /**eigen[1]*/;
  transformMatrix = Matrix<float>::eye(3);
  for (int i = 0; i < transformRes.cols(); i++) {
    for (int j = 0; j < transformRes.rows(); j++) {
      transformMatrix[i][j] = (float)transformRes[i][j];
    }
  }
  transformMatrix = transformMatrix.transpose();
  // transformMatrix.swapCols(0,1,0,transformMatrix.rows());
  for (int i = 0; i < points->size(); i++) {
    points->at(i).position =
        transformMatrix * (points->at(i).position - mean.toVec3());
  }

  return AbsHandler::handle(request);
}

Matrix<double> *PCAHandler::getCov() { return &cov; }

Matrix<double> *PCAHandler::getMean() { return &mean; }

std::vector<Matrix<double>> *PCAHandler::getEigen() { return &eigen; }

// PCAINVERSE HANDLER

PCAInverseHandler::PCAInverseHandler(PCAHandler *_pcaHandler)
    : pcaHandler(_pcaHandler) {}

GLSimpleMesh *PCAInverseHandler::handle(GLSimpleMesh *request) {

  std::vector<Vertex> *points = request->getPoints();
  std::vector<Triangle> *triangles = request->getTriangles();
  Matrix<double> *mean = pcaHandler->getMean();

  std::vector<Matrix<double>> *eigenVV = pcaHandler->getEigen();
  Matrix<double> transformRes =
      /*eigenVV->at(1).inverse() **/ eigenVV->at(0).transpose();
  transformMatrix = Matrix<float>::eye(3);
  for (int i = 0; i < transformRes.cols(); i++) {
    for (int j = 0; j < transformRes.rows(); j++) {
      transformMatrix[i][j] = (float)transformRes[i][j];
    }
  }

  transformMatrix = transformMatrix.transpose();

  for (int i = 0; i < points->size(); i++) {
    points->at(i).position =
        transformMatrix * (points->at(i).position) + mean->toVec3();
  }

  // transform and reclassify
  for (int i = 0; i < triangles->size(); i++) {
    triangles->at(i).vertices[0]->position =
        transformMatrix * (triangles->at(i).vertices[0]->position) +
        mean->toVec3();

    triangles->at(i).vertices[1]->position =
        transformMatrix * (triangles->at(i).vertices[1]->position) +
        mean->toVec3();

    triangles->at(i).vertices[2]->position =
        transformMatrix * (triangles->at(i).vertices[2]->position) +
        mean->toVec3();

    float score = triangles->at(i).classify();
    glm::vec3 color = Triangle::getColorFromGradient(score);

    triangles->at(i).vertices[0]->color = color;
    triangles->at(i).vertices[1]->color = color;
    triangles->at(i).vertices[2]->color = color;
  }

  return AbsHandler::handle(request);
}

// ROTATIONHANDLER METHODS

RotationHandler::RotationHandler(Matrix<float> m) : TransformationHandler(m) {}

RotationHandler::RotationHandler(float degree) {
  radians = degree * (M_PI / 180.0);
  transformMatrix =
      Matrix<float>(3, 3,
                    {cosf(radians), -sinf(radians), 0.f, sinf(radians),
                     cosf(radians), 0.f, 0.f, 0.f, 1.f});
}

GLSimpleMesh *RotationHandler::handle(GLSimpleMesh *request) {
  std::vector<Vertex> *points = request->getPoints();
  for (int i = 0; i < points->size(); i++) {
    points->at(i).position = transformMatrix * points->at(i).position;
  }
  return AbsHandler::handle(request);
}

// SCALE METHODS

ScaleHandler::ScaleHandler(Matrix<float> m) : TransformationHandler(m) {}

ScaleHandler::ScaleHandler(float factor) {
  scaleFactor = factor;
  transformMatrix =
      Matrix<float>(3, 3, {factor, 0.f, 0.f, 0.f, factor, 0.f, 0.f, 0.f, 1.f});
}

GLSimpleMesh *ScaleHandler::handle(GLSimpleMesh *request) {
  std::vector<Vertex> *points = request->getPoints();
  for (int i = 0; i < points->size(); i++) {
    points->at(i).position = transformMatrix * points->at(i).position;
  }
  return AbsHandler::handle(request);
}

// NORM HANDLER

NormalizationHandler::NormalizationHandler() : TransformationHandler() {}

GLSimpleMesh *NormalizationHandler::handle(GLSimpleMesh *request) {
  std::vector<Vertex> *points = request->getPoints();
  float xMax = -INFINITY;
  float yMax = -INFINITY;
  for (int i = 0; i < points->size(); i++) {
    glm::vec3 currentPoint = points->at(i).position;
    xMax = std::abs(currentPoint.x) > xMax ? std::abs(currentPoint.x) : xMax;
    yMax = std::abs(currentPoint.y) > yMax ? std::abs(currentPoint.y) : yMax;
  }
  float factor = xMax >= yMax ? 1.f / xMax : 1.f / yMax;
  transformMatrix =
      Matrix<float>(3, 3, {factor, 0.f, 0.f, 0.f, factor, 0.f, 0.f, 0.f, 1.f});
  for (int i = 0; i < points->size(); i++) {
    points->at(i).position = transformMatrix * points->at(i).position;
  }
  return AbsHandler::handle(request);
}

QuadTreeHandler::QuadTreeHandler() : TransformationHandler() {}

GLSimpleMesh *QuadTreeHandler::handle(GLSimpleMesh *request) {
  HandObject *h = dynamic_cast<HandObject *>(request);
  h->getQuadTree()->initQuadTree();
  h->getQuadTree()->init();
  return AbsHandler::handle(request);
}

// TRIANGULATIONHANDLER METHODS

GLSimpleMesh *TriangulationHandler::handle(GLSimpleMesh *request) {
  this->obj = dynamic_cast<GLTriMesh*>(request);
  std::vector<std::pair<uint, uint>> *lines = request->getLines();
  for (std::pair<uint, uint> l : *lines) {
    queue.push(l);
  }

  // for(int i=0;i<lines->size();i++){ //made this change so it would not ...
  for (int i = 0; i < request->sourceFileLinesSize; i++) {
    adjList.add({lines->at(i).first, lines->at(i).second});
  }

  makeTriangulation(this->obj);
  //for(int _ = 0;_<4;_++){
  //  smoothTriangulation();
  //}
  classifyTtriangles();
  prepareFrames();
  //debug();
  adjList.clear();
  return AbsHandler::handle(request);
}

void TriangulationHandler::makeTriangulation(GLTriMesh *obj) {
  //  adiciona as arestas na fila
  // vai esvaziando a fila, enquanto vai adicionando as novas arestas geradas
  std::vector<Vertex> *points = obj->getPoints();
  std::vector<std::pair<uint, uint>> *lines = obj->getLines();
  std::vector<Triangle> *triangles = obj->getTriangles();
  while (!queue.empty()) {
    auto currentEdge = queue.front();
    if (isDone(currentEdge)) {
      queue.pop();
      continue;
    };
    // Frame newFrame = obj->getFrames().back();
    int i = delaunay(
        currentEdge); // euclidian(newFrame,currentEdge);//delaunay(currentEdge);
    if (i < 0) {
      std::cout << "erro na aresta " << currentEdge.first << ", "
                << currentEdge.second << std::endl;
      break;
    }
    std::pair<int, int> newEdge1(currentEdge.first, i);
    std::pair<int, int> newEdge2(i, currentEdge.second);
    if (adjList.check(newEdge1) == '0') {
      lines->push_back(newEdge1);
    }
    if (adjList.check(newEdge2) == '0') {
      lines->push_back(newEdge2);
    }
    check(newEdge1);
    check(newEdge2);
    check(currentEdge);

    if (!isDone(newEdge1)) {
      queue.push(newEdge1);
    }
    if (!isDone(newEdge2)) {
      queue.push(newEdge2);
    }
    fillNeighbours(currentEdge.first, i, currentEdge.second);
    Triangle t(currentEdge.first, i, currentEdge.second,
               points->at(currentEdge.first), points->at(i),
               points->at(currentEdge.second));

    triangles->push_back(t);
  }
}

void TriangulationHandler::classifyTtriangles() {

  std::vector<Triangle> *triangles = obj->getTriangles();
  for (auto &t : *triangles) {
    t.classify();
  }
}

void TriangulationHandler::smoothTriangulation() {

  std::vector<Vertex> *points = obj->getPoints();
  std::vector<glm::vec3> newPositions(points->size());
  float lambda = 0.5f;

  for (size_t i = 0; i < points->size(); ++i) {
    const Vertex &vi = points->at(i);
    if (!vi.isActive || vi.neighbours.empty()) {
      newPositions[i] = vi.position;
      continue;
    }

    glm::vec3 displacement(0.0f);
    float weightSum = 0.0f;

    for (uint32_t j : vi.neighbours) {
      const glm::vec3 &vj = points->at(j).position;
      //float dist2 = glm::dot(vi.position - vj, vi.position - vj);
      //if (dist2 == 0.0f)
      //  continue;

      float w = 1.0f;// / dist2;
      displacement += w * (vj - vi.position);
      weightSum += w;
    }

    if (weightSum > 0.0f)
      newPositions[i] = vi.position + lambda * (displacement / weightSum);
    else
      newPositions[i] = vi.position;

    newPositions[i].z = 0.0f;
  }

  for (size_t i = 0; i < points->size(); ++i)
    points->at(i).position = newPositions[i];
}

void TriangulationHandler::prepareFrames() {

  std::vector<Triangle> *triangles = obj->getTriangles();
  std::vector<Vertex> *points = obj->getPoints();
  for (int i = 0; i < triangles->size(); i++) {
    Triangle &currentTriangle = triangles->at(i);
    Frame newFrame = obj->getFrames().back();
    newFrame.TriangleEnd += 1;
    newFrame.center =
        computeCircunsCenter(points->at(currentTriangle.indexes[0]).position,
                             points->at(currentTriangle.indexes[1]).position,
                             points->at(currentTriangle.indexes[2]).position);
    newFrame.radius = computeCircunsRadius(
        points->at(currentTriangle.indexes[0]).position, newFrame.center);
    obj->addFrame(newFrame);
  }
}

void TriangulationHandler::debug() {

  std::vector<Vertex> *points = obj->getPoints();

  std::cout << "Points size:" << points->size() << std::endl;
  for (auto p : *points) {
    std::cout << p.neighbours.size() << std::endl;
  }
}

void TriangulationHandler::fillNeighbours(uint p0, uint p1, uint p2) {
  std::vector<uint> vertices = {p0, p1, p2};
  std::vector<Vertex> *points = obj->getPoints();
  for (int i = 0; i < 3; i++) {
    Vertex &v = points->at(vertices[i]);
    v.neighbours.insert(vertices[(i + 1) % 3]);
    v.neighbours.insert(vertices[(i + 2) % 3]);
  }
}

void TriangulationHandler::check(const std::pair<uint, uint> &edge) {
  adjList.add(edge);
}

bool TriangulationHandler::isDone(const std::pair<uint, uint> &edge) {
  return adjList.check(edge) >= '2' ? true : false;
}

/* uint TriangulationHandler::delaunay(const std::pair<uint, uint>& edge) {
  int bestVertex = -1;
  float bestAngle = 1;
  bool found = false;
  float tolerance = 0.0;
  std::vector<Vertex> * points = this->obj->getLocalPoints();
  std::vector<uint> * indexes = this->obj->getLocalIndexes();
  while (!found){
    for (int i = 0; i < points->size(); i++) {
      tolerance = 0.f;
      bool valid = true;
      if( i == edge.first || i == edge.second) continue;
      if (Misc::Util::crossCompare(points->at(edge.first).position,
points->at(edge.second).position, points->at(i).position)) { for (int j = 0; j <
points->size(); j++) { if ( i != j
              && j != edge.first
              && j != edge.second
              && Misc::Util::crossCompare(points->at(edge.first).position,
points->at(edge.second).position, points->at(j).position)
              && Misc::Util::pointInCircle(points->at(edge.first).position,
points->at(edge.second).position, points->at(i).position,
points->at(j).position,tolerance)
             )
          {
            valid = false;
            Misc::Util::pointInCircle(points->at(edge.first).position,
points->at(edge.second).position, points->at(i).position,
points->at(j).position,tolerance); break;
          }
        }
        if (valid && !doIntersectOtherEdges(edge,
std::make_pair(points->at(edge.second).position,points->at(i).position)
,std::make_pair(points->at(i).position,points->at(edge.first).position))){ float
currentAngle = Misc::Util::angle( points->at(edge.first).position,
points->at(i).position, points->at(edge.second).position); if (currentAngle <=
bestAngle) { bestAngle = currentAngle; bestVertex = i; found = true;
          }
        }
      }
    }
    if (!found){
        tolerance+=0.1;
    }
    if (tolerance >= 1){
        std::cout<<"tolerancia maxima atingida\n";
        return bestVertex;
    }
  }
  return bestVertex;
} */

int TriangulationHandler::delaunay(const std::pair<uint, uint> &edge) {
  int bestVertex = -1;
  float bestAngle = 1;
  bool found = false;
  float tolerance = 0.0;
  std::vector<Vertex> *points = this->obj->getPoints();
  std::vector<uint> *indexes = this->obj->getIndexes();
  while (!found) {
    for (int i = 0; i < points->size(); i++) {
      bool valid = true;
      if (i == edge.first || i == edge.second)
        continue;
      if (Misc::Util::crossCompare(points->at(edge.first).position,
                                   points->at(edge.second).position,
                                   points->at(i).position)) {
        for (int j = 0; j < points->size(); j++) {
          if (i != j && j != edge.first && j != edge.second &&
              Misc::Util::crossCompare(points->at(edge.first).position,
                                       points->at(edge.second).position,
                                       points->at(j).position) &&
              Misc::Util::pointInCircle(points->at(edge.first).position,
                                        points->at(edge.second).position,
                                        points->at(i).position,
                                        points->at(j).position, tolerance)) {
            valid = false;
            Misc::Util::pointInCircle(points->at(edge.first).position,
                                      points->at(edge.second).position,
                                      points->at(i).position,
                                      points->at(j).position, tolerance);
            break;
          }
        }
        if (valid && !doIntersectOtherEdges(
                         edge,
                         std::make_pair(points->at(edge.second).position,
                                        points->at(i).position),
                         std::make_pair(points->at(i).position,
                                        points->at(edge.first).position))) {
          float currentAngle = Misc::Util::angle(
              points->at(edge.first).position, points->at(i).position,
              points->at(edge.second).position);
          if (currentAngle <= bestAngle) {
            bestAngle = currentAngle;
            bestVertex = i;
            found = true;
          }
        }
      }
    }
    // if (!found){
    //     tolerance+=0.1;
    // }
    if (tolerance >= 1) {
      std::cout << "tolerancia maxima atingida\n";
      return bestVertex;
    }
    tolerance += 0.5;
  }
  return bestVertex;
}

uint TriangulationHandler::euclidian(Frame &frame1,
                                     const std::pair<uint, uint> &edge) {
  int bestVertex = -1;
  float bestAngle = 1;
  glm::vec3 rightVertex = computeRightVertex(edge);
  float radius = computeRightTriangleHeight(edge);
  std::vector<Vertex> *points = this->obj->getPoints();
  std::vector<uint> *indexes = this->obj->getIndexes();
  std::pair<glm::vec3, glm::vec3> rightEdgetemp1 =
      std::make_pair(points->at(edge.first).position, rightVertex);
  std::pair<glm::vec3, glm::vec3> rightEdgetemp2 =
      std::make_pair(rightVertex, points->at(edge.second).position);
  for (int i = 0; i < points->size(); i++) {
    if (i == edge.first || i == edge.second)
      continue;
    std::pair<glm::vec3, glm::vec3> tempEdge1 =
        std::make_pair(points->at(edge.first).position, points->at(i).position);
    std::pair<glm::vec3, glm::vec3> tempEdge2 = std::make_pair(
        points->at(i).position, points->at(edge.second).position);
    if (Misc::Util::crossCompare(points->at(edge.first).position,
                                 points->at(edge.second).position,
                                 points->at(i).position) &&
        Misc::Util::pointInCircle(points->at(i).position, rightVertex,
                                  radius) &&
        !doIntersectOtherEdges(edge, tempEdge1, tempEdge2) &&
        !doTresPass(edge.first, edge.second, i) &&
        !isDone(std::make_pair<uint>(i, edge.first)) &&
        !isDone(std::make_pair<uint>(i, edge.second))) {
      float currentAngle = Misc::Util::angle(points->at(edge.first).position,
                                             points->at(i).position,
                                             points->at(edge.second).position);
      if (currentAngle <= bestAngle) {
        bestAngle = currentAngle;
        bestVertex = i;
        frame1.caso = 1;
      }
    }
  }
  if (bestVertex == -1) {
    if (!doIntersectOtherEdges(edge, rightEdgetemp1, rightEdgetemp2) &&
        !doTresPass(edge.first, edge.second, rightVertex)) {
      this->obj->addNewVertex(rightVertex, glm::vec3(0, 0, 0), 1);
      uint idx = indexes->back();
      float currentAngle =
          Misc::Util::angle(points->at(edge.first).position, rightVertex,
                            points->at(edge.second).position);
      bestAngle = currentAngle;
      bestVertex = idx;
      frame1.caso = 2;
    }
  }
  if (bestVertex == -1) {
    for (int j = 0; j < points->size(); j++) {
      if (j == edge.first || j == edge.second)
        continue;
      std::pair<glm::vec3, glm::vec3> tempEdge3 = std::make_pair(
          points->at(edge.first).position, points->at(j).position);
      std::pair<glm::vec3, glm::vec3> tempEdge4 = std::make_pair(
          points->at(j).position, points->at(edge.second).position);
      if (Misc::Util::crossCompare(points->at(edge.first).position,
                                   points->at(edge.second).position,
                                   points->at(j).position) &&
          !doIntersectOtherEdges(edge, tempEdge3, tempEdge4) &&
          !doTresPass(edge.first, edge.second, j) &&
          !isDone(std::make_pair<uint>(j, edge.first)) &&
          !isDone(std::make_pair<uint>(j, edge.second))) {
        float currentAngle = Misc::Util::angle(
            points->at(edge.first).position, points->at(j).position,
            points->at(edge.second).position);
        if (currentAngle <= bestAngle) {
          bestAngle = currentAngle;
          bestVertex = j;
          frame1.caso = 3;
        }
      }
    }
  }
  return bestVertex;
}

glm::vec3
TriangulationHandler::computeRightVertex(const std::pair<uint, uint> &edge) {
  std::vector<Vertex> *points = this->obj->getPoints();
  glm::vec3 edgeVec =
      (points->at(edge.second).position - points->at(edge.first).position);
  float edgeVeclen = glm::length(edgeVec);
  glm::vec3 unitEdgeVec = glm::normalize(edgeVec);
  glm::vec3 middlePoint =
      points->at(edge.first).position + unitEdgeVec * (edgeVeclen / 2.0f);

  glm::vec3 perpVecCC(-unitEdgeVec.y, unitEdgeVec.x, unitEdgeVec.z);
  float height = computeRightTriangleHeight(edge);
  glm::vec3 newPoint = middlePoint + (perpVecCC * height);
  return newPoint;
}

glm::vec3 TriangulationHandler::computeCircunsCenter(glm::vec3 p0, glm::vec3 p1,
                                                     glm::vec3 p2) {
  float t = p0.x * p0.x + p0.y * p0.y - p1.x * p1.x - p1.y * p1.y;
  float u = p0.x * p0.x + p0.y * p0.y - p2.x * p2.x - p2.y * p2.y;
  float j = (p0.x - p1.x) * (p0.y - p2.y) - (p0.x - p2.x) * (p0.y - p1.y);

  float cx = (-(p0.y - p1.y) * u + (p0.y - p2.y) * t) / (2.f * j);
  float cy = ((p0.x - p1.x) * u - (p0.x - p2.x) * t) / (2.f * j);

  return {cx, cy, 0};
}

float TriangulationHandler::computeCircunsRadius(glm::vec3 p0,
                                                 glm::vec3 center) {
  return Misc::Util::edgeLength(p0, center);
}

// uint TriangulationHandler::addNewVertex(glm::vec3 vec){
//   std::vector<Vertex> * points = this->obj->getLocalPoints();
//   std::vector<uint> * indexes = this->obj->getLocalIndexes();
//   Vertex newVertex = {vec,glm::vec3(0,0,0)};
//   points->push_back(newVertex);
//   uint newIdx = indexes->size();
//   indexes->push_back(newIdx);
//   return newIdx;
// }

float TriangulationHandler::computeRightTriangleHeight(
    const std::pair<uint, uint> &edge) {
  std::vector<Vertex> *points = this->obj->getPoints();
  float edgeVeclen = glm::length(points->at(edge.second).position -
                                 points->at(edge.first).position);
  return Misc::SQRT3 * edgeVeclen / 2.f;
}

bool TriangulationHandler::doIntersectOtherEdges(
    const std::pair<glm::vec3, glm::vec3> &e1,
    const std::pair<glm::vec3, glm::vec3> &e2) {
  bool intersect = false;
  std::vector<Vertex> *points = this->obj->getPoints();
  std::vector<std::pair<uint, uint>> *lines = this->obj->getLines();
  for (int i = 0; i < lines->size(); i++) {
    std::pair<uint, uint> currentEdge = lines->at(i);
    bool e1Intersect = Misc::Util::doIntersect(
        e1.first, e1.second, points->at(currentEdge.first).position,
        points->at(currentEdge.second).position);
    bool e2Intersect = Misc::Util::doIntersect(
        e2.first, e2.second, points->at(currentEdge.first).position,
        points->at(currentEdge.second).position);
    if (e1Intersect || e2Intersect) {
      intersect = true;
      break;
    }
  }
  return intersect;
}

bool TriangulationHandler::doIntersectOtherEdges(
    const std::pair<uint, uint> &baseEdge,
    const std::pair<glm::vec3, glm::vec3> &e1,
    const std::pair<glm::vec3, glm::vec3> &e2) {
  bool intersect = false;
  std::vector<Vertex> *points = this->obj->getPoints();
  bool val = false;
  std::vector<std::pair<uint, uint>> *lines = this->obj->getLines();
  // std::cout<< "Procurando interseccao da aresta:";
  for (int i = 0; i < lines->size(); i++) {
    std::pair<uint, uint> currentEdge = lines->at(i);
    glm::vec3 currentEdgeV1 = points->at(currentEdge.first).position;
    glm::vec3 currentEdgeV2 = points->at(currentEdge.second).position;
    // if((currentEdgeV1 == e1.first && currentEdgeV2 == e1.second) ||
    // (currentEdgeV1 == e1.second && currentEdgeV2 == e1.first)) continue;
    // if((currentEdgeV1 == e2.first && currentEdgeV2 == e2.second) ||
    // (currentEdgeV1 == e2.second && currentEdgeV2 == e2.first)) continue;
    // if((baseEdge.first == currentEdge.first && baseEdge.second ==
    // currentEdge.second) || (baseEdge.first == currentEdge.second &&
    // baseEdge.second == currentEdge.first)) continue; if((baseEdge.first ==
    // currentEdge.first || baseEdge.second == currentEdge.second) ||
    // (baseEdge.first == currentEdge.second || baseEdge.second ==
    // currentEdge.first)) continue;
    bool e1Intersect = Misc::Util::doIntersect(
        e1.first, e1.second, points->at(currentEdge.first).position,
        points->at(currentEdge.second).position);
    bool e2Intersect = Misc::Util::doIntersect(
        e2.first, e2.second, points->at(currentEdge.first).position,
        points->at(currentEdge.second).position);
    if (e1Intersect || e2Intersect) {
      intersect = true;
      break;
    }
  }
  return intersect;
}

bool TriangulationHandler::doTresPass(uint idxP0, uint idxP1, uint idxP2) {
  std::vector<Vertex> *points = this->obj->getPoints();
  glm::vec3 p0 = points->at(idxP0).position;
  glm::vec3 p1 = points->at(idxP1).position;
  glm::vec3 p2 = points->at(idxP2).position;
  bool isInside = false;
  glm::vec3 p;
  for (int i = 0; i < points->size(); i++) {
    if (i == idxP0 || i == idxP1 || i == idxP2) {
      continue;
    }
    p = points->at(i).position;
    isInside = Misc::Util::pointInsideTriangle(p, p0, p1, p2);
    if (isInside) {
      return true;
    }
  }
  return false;
}

bool TriangulationHandler::doTresPass(uint idxP0, uint idxP1, glm::vec3 pRet) {
  std::vector<Vertex> *points = this->obj->getPoints();
  glm::vec3 p0 = points->at(idxP0).position;
  glm::vec3 p1 = points->at(idxP1).position;
  bool isInside = false;
  glm::vec3 p;
  for (int i = 0; i < points->size(); i++) {
    if (i == idxP0 || i == idxP1) {
      continue;
    }
    p = points->at(i).position;
    isInside = Misc::Util::pointInsideTriangle(p, p0, p1, pRet);
    if (isInside) {
      return true;
    }
  }
  return false;
}
