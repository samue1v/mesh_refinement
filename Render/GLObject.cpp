#include "GLObject.hpp"
#include "Scene.hpp"

GLDrawable::GLDrawable(QOpenGLContext *context) : visibility(true) {
  if (context != nullptr) {
    currentContext = context;
    f = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(context);
  } else {
    currentContext = nullptr;
    f = nullptr;
  }
}

GLDrawable::~GLDrawable(){
  delete f;
}

bool GLDrawable::isVisible() { return visibility; }

void GLDrawable::setVisible(const bool vis) { visibility = vis; }

GLSimpleMesh::GLSimpleMesh(QOpenGLContext *context,
                           Scene *_currentScene)
    : GLDrawable(context), baseHandler(nullptr), currentScene(_currentScene) {}

std::vector<Vertex> *GLSimpleMesh::getPoints() { return &points; }

std::vector<uint> *GLSimpleMesh::getIndexes() { return &indexes; }

std::vector<Triangle> *GLSimpleMesh::getTriangles() { return &triangles; }

std::vector<std::pair<uint, uint>> *GLSimpleMesh::getLines() { return &lines; }

glm::vec3 GLSimpleMesh::getCenter() {
  glm::vec3 res({0.f, 0.f, 0.f});
  glm::vec3 maxV = maxValAxis();
  glm::vec3 minV = minValAxis();
  glm::vec3 dif = (maxV - minV);
  res[0] = minV[0] + dif[0] / 2.f;
  res[1] = minV[1] + dif[1] / 2.f;
  return res;
}

glm::vec3 GLSimpleMesh::maxValAxis() {
  float maxFloat = 2e22;
  glm::vec3 max({-maxFloat, -maxFloat, -maxFloat});
  for (Vertex v : points) {
    glm::vec3 p = v.position;
    for (int i = 0; i < 3; i++) {
      if (p[i] > max[i]) {
        max[i] = p[i];
      }
    }
  }

  return max;
}

glm::vec3 GLSimpleMesh::minValAxis() {
  float maxFloat = 2e22;
  glm::vec3 min({maxFloat, maxFloat, maxFloat});
  for (Vertex v : points) {
    glm::vec3 p = v.position;
    for (int i = 0; i < 3; i++) {
      if (p[i] < min[i]) {
        min[i] = p[i];
      }
    }
  }

  return min;
}

void GLSimpleMesh::addNewVertex(glm::vec3 pos, glm::vec3 color, bool isActive) {
  Vertex newVertex = {pos, color, isActive};
  points.push_back(newVertex);
  uint newIdx = indexes.size();
  indexes.push_back(newIdx);
}

bool GLSimpleMesh::isPointInside(glm::vec3 p) {

  glm::vec3 global_min = minValAxis();
  glm::vec3 global_max = maxValAxis();

  glm::vec3 res = global_max - global_min;
  float dmax = std::max({res.x, res.y, res.z});
  std::vector<glm::vec3> limitPoints;
  glm::vec3 p_right({p.x + dmax + 1, p.y, p.z});
  glm::vec3 p_down({p.x, p.y - dmax - 1, p.z});
  glm::vec3 p_left({p.x - dmax - 1, p.y, p.z});
  glm::vec3 p_up({p.x, p.y + dmax + 1, p.z});

  limitPoints.push_back(p_right);
  limitPoints.push_back(p_down);
  limitPoints.push_back(p_left);
  limitPoints.push_back(p_up);

  for (glm::vec3 lp : limitPoints) {
    int intersect = 0;
    for (int i = 0; i < sourceFileLinesSize; i++) {
      std::pair<uint, uint> curEdge = lines.at(i);
      if (Misc::Util::doIntersect(p, lp, points.at(curEdge.first).position,
                                  points.at(curEdge.second).position)) {
        intersect += 1;
        // break;
      }
    }
    if (intersect % 2 == 0) {
      return false;
    }
  }

  return true;
}

AbsHandler *GLSimpleMesh::getBaseHandler() { return baseHandler; }

void GLSimpleMesh::setbaseHandler(AbsHandler *newHandler) {
  baseHandler = newHandler;
}

void GLSimpleMesh::execHandlers() {
  if (baseHandler != nullptr) {
    baseHandler->handle(this);
  }
}

void GLSimpleMesh::init() {

  VAO.reserve(1);
  VAO.resize(1);

  VBO.reserve(1);
  VBO.resize(1);

  EBO.reserve(1);
  EBO.resize(1);

  program.reserve(1);
  program.resize(1);

  for (int i = 0; i < 1; i++) {
    f->glGenVertexArrays(1, &VAO[i]);
    f->glGenBuffers(1, &VBO[i]);
    f->glGenBuffers(1, &EBO[i]);
    program[i] = GLProgram(currentContext);
  }

  program[0].createShaderFromFile("fill_vertex.vert", "fill_frag.frag");

  std::vector<std::pair<glm::vec3, glm::vec3>> vertexBufferPoints;
  for (Vertex &v : points) {
    vertexBufferPoints.push_back({v.position, v.color});
  }

  f->glBindVertexArray(VAO[0]);

  f->glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  f->glBufferData(GL_ARRAY_BUFFER,
                  sizeof(std::pair<glm::vec3, glm::vec3>) *
                      vertexBufferPoints.size(),
                  vertexBufferPoints.data(), GL_DYNAMIC_DRAW);

  f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
  f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indexes.size(),
                  indexes.data(), GL_DYNAMIC_DRAW);

  GLint point_position_attribute =
      f->glGetAttribLocation(program[0].getProgramId(), "position");
  GLint point_color_attribute =
      f->glGetAttribLocation(program[0].getProgramId(), "color_a");
  f->glVertexAttribPointer(point_position_attribute, 3, GL_FLOAT, GL_FALSE,
                           sizeof(std::pair<glm::vec3, glm::vec3>), 0);
  f->glVertexAttribPointer(point_color_attribute, 3, GL_FLOAT, GL_FALSE,
                           sizeof(std::pair<glm::vec3, glm::vec3>),
                           (void *)(sizeof(glm::vec3)));

  f->glEnableVertexAttribArray(point_position_attribute);
  f->glEnableVertexAttribArray(point_color_attribute);

  f->glBindBuffer(GL_ARRAY_BUFFER, 0);         // unbid current VBO
  f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbid current EBO
  f->glBindVertexArray(0);                     // unbind current VAO
}

void GLSimpleMesh::draw() {

  f->glPointSize(4);
  f->glLineWidth(1);

  uint p0ID = program[0].getProgramId();
  f->glUseProgram(p0ID);
  GLuint vmatrix = f->glGetUniformLocation(p0ID, "m_view");
  GLuint pmatrix = f->glGetUniformLocation(p0ID, "m_proj");
  f->glUniformMatrix4fv(vmatrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.)));
  f->glUniformMatrix4fv(pmatrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.)));

  // POINTS
  f->glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

  f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);

  f->glBindVertexArray(VAO[0]);

  f->glDrawElements(GL_POINTS, indexes.size(), GL_UNSIGNED_INT, indexes.data());

  f->glBindVertexArray(0);
  f->glBindBuffer(GL_ARRAY_BUFFER, 0);
  f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLTriMesh::GLTriMesh(Scene *scene, QOpenGLContext *context)
    : GLSimpleMesh(context, scene) {}

GLTriMesh::~GLTriMesh() { delete baseHandler; }

void GLTriMesh::init() {}

void GLTriMesh::draw() {}

void GLTriMesh::addFrame(Frame f) { frames.push_back(f); }

std::vector<Frame> GLTriMesh::getFrames() { return frames; }

GLProgram &GLTriMesh::getProgram(uint idx) { return program[idx]; }

bool GLTriMesh::nextFrame() {
  if (currentFrame == (int)frames.size() - 1) {
    return false;
  }
  currentFrame = std::min(currentFrame + 1, (int)frames.size() - 1);
  return true;
}

bool GLTriMesh::previousFrame() {
  if (currentFrame == 0) {
    return false;
  }
  currentFrame = std::max(currentFrame - 1, 0);
  return true;
}

Triangle GLTriMesh::getCurrentTriangle() {
  return triangles[frames[currentFrame].TriangleEnd - 1];
}

int GLTriMesh::getCurrentFrame() { return currentFrame; }

int GLTriMesh::pickTriangle(glm::vec3 p) {
  glm::vec3 p0, p1, p2;
  bool inside = false;
  for (int i = 0; i < currentFrame; i++) {
    // Vertex * v = triangles[i].getVertices();
    p0 = triangles[i].vertices[0]->position;
    p1 = triangles[i].vertices[1]->position;
    p2 = triangles[i].vertices[2]->position;
    // p0 = v[0].position;
    // p1 = v[1].position;
    // p2 = v[2].position;

    inside = Misc::Util::pointInsideTriangle(p, p0, p2, p1);
    if (inside) {
      return i;
    }
  }
  return -1;
}

void GLTriMesh::restoreOriginal() { currentFrame = 0; }

// get scene index from vertex v
int GLTriMesh::getGlobalIndexFromVertex(Vertex v) {
  int idx = currentScene->getIndexFromVertex(v);
  return idx;
}

////get Vertex at index in vertex idx
// Vertex GLTriMesh::getGlobalVertexFromIndex(uint idx){
//   return currentScene->getVertexFromIndex(globalPointIndexes.at(idx));
// }

uint GLTriMesh::addVertexToScene(Vertex v) {
  uint idx = currentScene->addVertex(v);
  return idx;
}

Scene *GLTriMesh::getScene() { return currentScene; }
