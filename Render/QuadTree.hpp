#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include "GLObject.hpp"
#include <glm/vec3.hpp>
#include <iostream>
#include <queue>
#include <random>
#include <vector>

enum NodeType { Mid, Leaf };

enum PointLabel { TL, BR, TR, BL };

enum dir { dr, dl, du, dd };

class Square {
public:
  Square();
  Square(glm::vec3 tl, glm::vec3 br);
  void computeSquarePoints();
  Vertex getPoint(int idx);
  glm::vec3 getCenter();

private:
  std::vector<Vertex> points;
};

class Node {
public:
  Node(Node *_parent = nullptr, Square *sq = nullptr);
  ~Node();
  void setParent(Node *node);
  Node *getParent();
  Node *getChildAt(int idx);
  Square *getSquare();
  void setSquare(Square *_square);
  NodeType getStatus();
  int getSeq();
  void setStatus(int s);
  void setChild(Node *node);
  bool isPointInside(glm::vec3 p);
  void organizeChildren();

private:
  Node *parent;
  std::vector<Node *> children;
  Square *square;
  NodeType status;
  int seq;
};

class QuadTree : public GLSimpleMesh {

public:
  QuadTree(QOpenGLContext *context = nullptr, GLSimpleMesh *_glObj = nullptr,
           int maxDepth = -1);
  ~QuadTree();
  void initQuadTree();
  void makeQuadTree(Node *child, int depth);
  bool classify(Node *node);
  void subDivide(Node *node);
  void applyRule21();
  void calcBox(Node *node, glm::vec3 TLB, glm::vec3 BRF, uint8_t depth);
  void divideBox(const glm::vec3 &TL, const glm::vec3 &BR, glm::vec3 &new_TL,
                 glm::vec3 &new_BR, uint8_t depth);
  glm::vec3 getRootCenter();
  float getRootHeight();

  void init() override;
  void draw() override;

private:
  Node *getNeighbour(Node *root, int dir);
  void getMaxDimensionAndCenter(float *dmax, glm::vec3 *center);
  bool shouldSubdivide(Node *base, Node *nb, int dir);
  void fillRenderDataRec(Node *root);
  void fillRenderData(Node *root);
  void insertPoints();
  void updateLeaves();
  void updateLeavesRecursive(Node *root);

private:
  GLSimpleMesh *glObj;
  Node *rootNode;
  int depth;
  std::vector<Node *> leaves;
  std::vector<std::vector<int>> directions;
};

#endif
