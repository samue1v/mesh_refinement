#include "Triangle.hpp"

Triangle::Triangle(int idx0, int idx1, int idx2, Vertex &v1, Vertex &v2,
                   Vertex &v3) {
  setPoints(v1, v2, v3);
  setIndexes(idx0, idx1, idx2);
}

glm::vec3 Triangle::getColorFromGradient(float v) {
  QLinearGradient linearGrad(QPointF(0, 0), QPointF(10, 0));
  linearGrad.setColorAt(0, QColor(237, 0, 0));
  linearGrad.setColorAt(0.75, QColor(186, 237, 0));
  linearGrad.setColorAt(0.5, QColor(237, 217, 0));
  linearGrad.setColorAt(0.25, QColor(237, 130, 0));
  linearGrad.setColorAt(1, QColor(67, 237, 0));
  float segmentLength = 10;
  float pdist = v * 10;
  float ratio = 1 - pdist / segmentLength;
  float red = (ratio * 230 + (1 - ratio) * 104);
  float green = (ratio * 14 + (1 - ratio) * 230);
  float blue = (ratio * 14 + (1 - ratio) * 14);

  return glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

Triangle::~Triangle() {}

float Triangle::classify() {
  float area =
      glm::length(glm::cross((vertices[0]->position - vertices[1]->position),
                             (vertices[2]->position - vertices[1]->position))) /
      2.0f;
  float l1 = glm::length(vertices[0]->position - vertices[1]->position);
  float l2 = glm::length(vertices[2]->position - vertices[1]->position);
  float l3 = glm::length(vertices[2]->position - vertices[0]->position);

  float inscritRadius = area / ((l1 + l2 + l3) / 2.0);

  float sinI = area / (l1 * l2);

  float circunRadius = 0.5 * l3 / sinI;

  float prop = 16.0 * area * area / (l1 * l2 * l3 * (l1 + l2 + l3));

  glm::vec3 res = getColorFromGradient(prop);

  color = res;
  score = prop;

  return prop;
}
