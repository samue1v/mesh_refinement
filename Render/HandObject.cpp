#include "HandObject.hpp"
#include "Scene.hpp"

HandObject::HandObject(Scene *scene, QOpenGLContext *context)
    : GLTriMesh::GLTriMesh(scene, context) {
  tree = new QuadTree(context, this, 9);
  currentFrame = 0;
}

HandObject::~HandObject() {
  std::cout << "HandObject destructor\n";
  if (tree != nullptr) {
    delete tree;
  }
}

void HandObject::init() {

  Frame firstframe =
      Frame({0, points.size(), 0, lines.size(), 0, 0, glm::vec3(0, 0, 0), 0});
  frames.push_back(firstframe);

  // executing base handler
  execHandlers();

  // pontos,linhas, triangulo e circulo...
  VAO.reserve(5);
  VAO.resize(5);

  VBO.reserve(5);
  VBO.resize(5);

  EBO.reserve(5);
  EBO.resize(5);

  program.reserve(5);
  program.resize(5);

  for (int i = 0; i < 5; i++) {
    f->glGenVertexArrays(1, &VAO[i]);
    f->glGenBuffers(1, &VBO[i]);
    f->glGenBuffers(1, &EBO[i]);
    program[i] = GLProgram(currentContext);
  }

  // points,lines and triangles
  program[0].createShaderFromFile("fill_vertex.vert", "fill_frag.frag");

  // circle
  program[1].createShaderFromFile("circleVert.vert", "circleFrag.frag");

  // wireframe
  program[2].createShaderFromFile("gpe_vertex.vert", "gpe_frag.frag");

  // Points initialization, index 0

  std::vector<std::pair<glm::vec3, glm::vec3>> vertexBufferPoints;
  for (Vertex &v : points) {
    vertexBufferPoints.push_back({v.position, {1.f, 0.f, 0.f}});
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

  // lines
  f->glBindVertexArray(VAO[1]);

  f->glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  f->glBufferData(GL_ARRAY_BUFFER,
                  sizeof(std::pair<glm::vec3, glm::vec3>) *
                      sourceFilePointsSize,
                  vertexBufferPoints.data(), GL_DYNAMIC_DRAW);

  f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
  f->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                  sizeof(std::pair<uint, uint>) * sourceFileLinesSize,
                  lines.data(), GL_DYNAMIC_DRAW);

  GLint line_position_attribute =
      f->glGetAttribLocation(program[0].getProgramId(), "position");
  GLint line_color_attribute =
      f->glGetAttribLocation(program[0].getProgramId(), "color_a");
  f->glVertexAttribPointer(line_position_attribute, 3, GL_FLOAT, GL_FALSE,
                           sizeof(std::pair<glm::vec3, glm::vec3>), 0);
  f->glVertexAttribPointer(line_color_attribute, 3, GL_FLOAT, GL_FALSE,
                           sizeof(std::pair<glm::vec3, glm::vec3>),
                           (void *)(sizeof(glm::vec3)));

  f->glEnableVertexAttribArray(line_position_attribute);
  f->glEnableVertexAttribArray(line_color_attribute);

  f->glBindBuffer(GL_ARRAY_BUFFER, 0);         // unbid current VBO
  f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbid current EBO
  f->glBindVertexArray(0);                     // unbind current VAO

  // fill triangles
  std::vector<std::pair<glm::vec3, glm::vec3>> vertexBufferTri;
  for (Triangle &tri : triangles) {
    vertexBufferTri.push_back({tri.vertices[0]->position, tri.color});
    vertexBufferTri.push_back({tri.vertices[1]->position, tri.color});
    vertexBufferTri.push_back({tri.vertices[2]->position, tri.color});
  }

  f->glBindVertexArray(VAO[2]);

  f->glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
  f->glBufferData(GL_ARRAY_BUFFER,
                  sizeof(std::pair<glm::vec3, glm::vec3>) *
                      vertexBufferTri.size(),
                  vertexBufferTri.data(), GL_DYNAMIC_DRAW);
  // p1x | p1y | p1z | r | g | b | p2x | p2y | p2z | r | g | b | p3x | p3y | p3z
  // | r | g | b | <      12     >

  // f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
  // f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(std::pair<uint,uint>) *
  // handHull.lVec().size() ,handHull.lVec().data(),GL_DYNAMIC_DRAW);

  GLint triangle_point_attribute =
      f->glGetAttribLocation(program[0].getProgramId(), "position");
  GLint triangle_color_attribute =
      f->glGetAttribLocation(program[0].getProgramId(), "color_a");

  f->glVertexAttribPointer(triangle_point_attribute, 3, GL_FLOAT, GL_FALSE,
                           sizeof(std::pair<glm::vec3, glm::vec3>), 0);
  f->glEnableVertexAttribArray(triangle_point_attribute);

  f->glVertexAttribPointer(triangle_color_attribute, 3, GL_FLOAT, GL_FALSE,
                           sizeof(std::pair<glm::vec3, glm::vec3>),
                           (void *)(sizeof(glm::vec3)));
  f->glEnableVertexAttribArray(triangle_color_attribute);

  f->glBindBuffer(GL_ARRAY_BUFFER, 0);         // unbid current VBO
  f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbid current EBO
  f->glBindVertexArray(0);                     // unbind current VAO

  // wireframe
  f->glBindVertexArray(VAO[3]);

  f->glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
  f->glBufferData(GL_ARRAY_BUFFER,
                  sizeof(std::pair<glm::vec3, glm::vec3>) *
                      vertexBufferTri.size(),
                  vertexBufferTri.data(), GL_DYNAMIC_DRAW);

  GLint wireframe_point_attribute =
      f->glGetAttribLocation(program[2].getProgramId(), "position");
  f->glVertexAttribPointer(wireframe_point_attribute, 3, GL_FLOAT, GL_FALSE,
                           sizeof(std::pair<glm::vec3, glm::vec3>), 0);
  f->glEnableVertexAttribArray(wireframe_point_attribute);

  f->glBindBuffer(GL_ARRAY_BUFFER, 0);
  f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  f->glBindVertexArray(0);

  std::cout << "Finished init\n";
}

void HandObject::draw() {

  // std::cout<<"Drawing...\n";
  f->glPointSize(4);
  f->glLineWidth(1);

  // f->glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  // Ver isso!!

  // ViewMatrix set

  uint p0ID = program[0].getProgramId();
  f->glUseProgram(p0ID);
  GLuint vmatrix = f->glGetUniformLocation(p0ID, "m_view");
  GLuint pmatrix = f->glGetUniformLocation(p0ID, "m_proj");
  f->glUniformMatrix4fv(
      vmatrix, 1, GL_FALSE,
      glm::value_ptr(currentScene->getCamera()->getViewMatrix()));
  f->glUniformMatrix4fv(
      pmatrix, 1, GL_FALSE,
      glm::value_ptr(currentScene->getCamera()->getProjMatrix()));

  uint p1ID = program[1].getProgramId();
  f->glUseProgram(p1ID);
  GLuint vmatrix1 = f->glGetUniformLocation(p1ID, "m_view");
  GLuint pmatrix1 = f->glGetUniformLocation(p1ID, "m_proj");
  f->glUniformMatrix4fv(
      vmatrix1, 1, GL_FALSE,
      glm::value_ptr(currentScene->getCamera()->getViewMatrix()));
  f->glUniformMatrix4fv(
      pmatrix1, 1, GL_FALSE,
      glm::value_ptr(currentScene->getCamera()->getProjMatrix()));

  uint p2ID = program[2].getProgramId();
  f->glUseProgram(p2ID);
  GLuint vmatrix2 = f->glGetUniformLocation(p2ID, "m_view");
  GLuint pmatrix2 = f->glGetUniformLocation(p2ID, "m_proj");
  f->glUniformMatrix4fv(
      vmatrix2, 1, GL_FALSE,
      glm::value_ptr(currentScene->getCamera()->getViewMatrix()));
  f->glUniformMatrix4fv(
      pmatrix2, 1, GL_FALSE,
      glm::value_ptr(currentScene->getCamera()->getProjMatrix()));
  std::cout << "0\n";

  // POINTS

  f->glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

  f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);

  f->glBindVertexArray(VAO[0]);

  int pointsCount =
      frames[currentFrame].PointEnd - frames[currentFrame].PointBegin;
  f->glUseProgram(program[0].getProgramId());
  f->glDrawElements(GL_POINTS, indexes.size(), GL_UNSIGNED_INT, indexes.data());

  f->glBindVertexArray(0);

  // LINES

  // Ligando VAO anterior, pois ele esta sendo atualizado.
  f->glBindVertexArray(VAO[1]);
  f->glUseProgram(program[0].getProgramId());

  int linesCount =
      frames[currentFrame].LineEnd - frames[currentFrame].LineBegin;
  f->glDrawElements(GL_LINES, linesCount * 2, GL_UNSIGNED_INT,
                    lines.data() + frames[currentFrame].LineBegin *
                                       sizeof(std::pair<uint, uint>));

  f->glBindVertexArray(0);

  //// TRIANGLES
  f->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  f->glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);

  f->glBindVertexArray(VAO[2]);

  f->glUseProgram(program[0].getProgramId());

  int trianglesCount =
      frames[currentFrame].TriangleEnd - frames[currentFrame].TriangleBegin;
  f->glDrawArrays(GL_TRIANGLES, frames[currentFrame].TriangleBegin * 3,
                  trianglesCount * 3);

  f->glBindVertexArray(0);

  // wireframe

  f->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // f->glClear(GL_DEPTH_BUFFER_BIT);

  f->glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);

  f->glBindVertexArray(VAO[3]);

  f->glUseProgram(program[2].getProgramId());

  f->glDrawArrays(GL_TRIANGLES, frames[currentFrame].TriangleBegin * 3,
                  trianglesCount * 3);

  f->glBindVertexArray(0);
  f->glBindBuffer(GL_ARRAY_BUFFER, 0);
  f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  if (tree != nullptr && tree->isVisible()) {
    tree->draw();
  }


  ////Circle
  int num_segments = 100;

  float theta = 2 * M_PI / float(num_segments);
  float c = cosf(theta); // precalculate the sine and cosine
  float s = sinf(theta);
  float t;

  float x = 0.1; // frames[currentFrame].radius; // we start at angle = 0
  float y = 0;
  if (Misc::Util::almostZero(x, 10)) {
    return;
  }
  QVector<float> vector;
  vector.clear();

  glm::vec3 center = glm::vec3(0., 0., 0); // frames[currentFrame].center;

  for (int i = 0; i < num_segments; i++) {
    // apply translation
    vector.append(x + center.x);
    vector.append(y + center.y);

    // apply the rotation matrix
    t = x;
    x = c * x - s * y;
    y = s * t + c * y;
  }

  float centerPoint[] = {center.x, center.y};

  f->glClear(GL_DEPTH_BUFFER_BIT);
  f->glPointSize(4);
  f->glLineWidth(2);

  f->glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
  f->glBufferData(GL_ARRAY_BUFFER, vector.size() * sizeof(float), vector.data(),
                  GL_STATIC_DRAW);

  GLint position_attribute =
      f->glGetAttribLocation(program[1].getProgramId(), "aPos");

  f->glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
  f->glEnableVertexAttribArray(position_attribute);
  f->glBindBuffer(GL_ARRAY_BUFFER, 0);

  f->glUseProgram(program[1].getProgramId());
  f->glDrawArrays(GL_LINE_LOOP, 0, num_segments); // Simple Circle

  f->glBindVertexArray(VAO[4]);

  f->glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
  f->glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float), centerPoint,
                  GL_STATIC_DRAW);

  f->glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
  f->glEnableVertexAttribArray(position_attribute);
  f->glBindBuffer(GL_ARRAY_BUFFER, 0);

  f->glUseProgram(program[1].getProgramId());
  f->glDrawArrays(GL_POINTS, 0, 1); // Simple Circle

  f->glBindVertexArray(0);
  f->glBindBuffer(GL_ARRAY_BUFFER, 0);

  std::cout << "hand draw\n";
}

void HandObject::restoreOriginal() { GLTriMesh::restoreOriginal(); }

bool HandObject::isQuadVisible() { return tree->isVisible(); }

void HandObject::setQuadVisible() {
  if (tree->isVisible()) {
    tree->setVisible(false);
  } else {
    tree->setVisible(true);
  }
}

QuadTree *HandObject::getQuadTree() { return tree; }
