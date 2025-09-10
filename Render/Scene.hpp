#ifndef SCENE_HPP
#define SCENE_HPP
#include "../Common/DrawCommands.hpp"
#include "../Logic/CoRHandlers.hpp"
#include "../Logic/FileHandler.hpp"
#include "Camera.hpp"
#include "GLObject.hpp"
#include "HandObject.hpp"
//#include "TextRender.hpp"
#include <QOpenGLContext>
#include <format>
#include <functional>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class MyGLWidget;

struct KeyEqualsVec3 {
  bool operator()(Vertex lhs, Vertex rhs) const {
    const float epsilon = 10e-9;
    return (std::abs(lhs.position.x - rhs.position.x) < epsilon &&
            std::abs(lhs.position.y - rhs.position.y) < epsilon);
  }
};

struct KeyHasherVec3 {
  std::size_t operator()(const Vertex &k) const {
    // lembrar de olhar no stackoverflow!!!!!!!!!!!!!!!
    return ((std::hash<float>()(k.position.x) ^
             (std::hash<float>()(k.position.y) << 1)));
  }
};

class Scene {
public:
  Scene(MyGLWidget *glw , QOpenGLContext *currentContext,
        const MenuCommands::CreateSceneCMD &cmd_scene);
  ~Scene();
  void addObject(std::string src);
  void removeObject(std::string name);
  void setVisible(std::string s);
  void render();
  int addVertex(Vertex p);
  void w_press();
  void s_press();
  void d_press();
  void a_press();
  void up_press();
  void down_press();
  void space_press();
  void v_press();
  void left_press(int step);
  void right_press(int step);
  std::pair<int, Triangle> mousePickLeft(glm::vec3 virtualPos, int width,
                                         int height);
  void mousePickRight(glm::vec3 virtualPos, int width, int height);
  void scrollUp();
  void scrollDown();
  void increaseSpeed();
  void decreaseSpeed();
  int getNumObjects();
  //void updateText();
  Camera *getCamera();
  std::vector<Vertex> *getScenePoints();
  int getIndexFromVertex(Vertex v);
  Vertex getVertexFromIndex(uint idx);
  std::string getObjectName(int idx);
  AbsHandler *buildHandler(int typeHandler);

  // private:
  std::string sceneSource;
  std::vector<HandObject *> objects;
  std::unordered_map<Vertex, uint, KeyHasherVec3, KeyEqualsVec3> table;
  std::vector<Vertex> scenePoints;
  HandObject *currentObj;

private:
  GLFileHandler *f;
  QOpenGLContext *currentContext;
  //TextRender *textRenderer;
  Camera *camera;
  MyGLWidget *glWidget;
};

#endif
