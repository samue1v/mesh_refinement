#include "Scene.hpp"
#include "MyGLWidget.hpp"
Scene::Scene(MyGLWidget *glw, QOpenGLContext *context,
             const MenuCommands::CreateSceneCMD &scene_cmd)
    : glWidget(glw), currentContext(context), sceneSource(scene_cmd.objPath) {

  f = new ObjFileHandler();
  f->readFile(sceneSource);
  int numf = f->getNumStrings();
  camera = new Camera(glm::vec3(0, 0., 1.), glm::vec3(0., 0., -1.));
  for (int i = 0; i < numf; i++) {
    HandObject *h = new HandObject(this, context);
    f->parse(h, i);
    h->setbaseHandler(buildHandler(scene_cmd.MeshApproach));
    h->init();
    objects.push_back(h);
  }
  currentObj = objects.at(0);
  // textRenderer = new TextRender(this, context);
  // textRenderer->init();
  // updateText();
}

Scene::~Scene() {
  if (f != nullptr) {
    delete f;
  }
  for (GLDrawable *o : objects) {
    delete o;
  }
  //delete textRenderer;
}

Camera *Scene::getCamera() { return camera; }

void Scene::addObject(std::string objSrc) {
  delete f;
  f = new GLFileHandler();
  f->readFile(objSrc);
  int numf = f->getNumStrings();
  for (int i = 0; i < numf; i++) {
    HandObject *h = new HandObject(this, currentContext);
    f->parse(h, i);
    objects.push_back(h);
  }
}

void Scene::removeObject(std::string name) {
  for (int i = 0; i < objects.size(); i++) {
    GLTriMesh *o = objects.at(i);
    if (o->name == name) {
      objects.erase(objects.begin() + i);
    }
  }
}

void Scene::setVisible(std::string s) {
  for (HandObject *obj : objects) {
    if (obj->name == s || s == "All") {
      obj->restoreOriginal();
      // obj->setbaseHandler(buildHandler(transformType));
      // obj->init();

      // obj->execHandlers();
      obj->setVisible(true);
      currentObj = obj;
    } else {
      obj->setVisible(false);
    }
  }
  // updateText();
}

AbsHandler *Scene::buildHandler(MenuCommands::MeshingApproach typeHandler) {
  AbsHandler *base = new AbsHandler;
  TriangulationHandler *trHandler = new TriangulationHandler;
  NormalizationHandler *normHandler = new NormalizationHandler;
  QuadTreeHandler *treeHandler = new QuadTreeHandler;
  if (typeHandler == MenuCommands::MeshingApproach::PCAInv) {
    PCAHandler *pcaHand = new PCAHandler;
    PCAInverseHandler *pcaHandInv = new PCAInverseHandler(pcaHand);
    base->setNext(normHandler)
        ->setNext(pcaHand)
        ->setNext(treeHandler)
        ->setNext(trHandler)
        ->setNext(pcaHandInv);
  } else if (typeHandler == MenuCommands::MeshingApproach::PCA) {
    PCAHandler *pcaHand = new PCAHandler;
    base->setNext(normHandler)
        ->setNext(pcaHand)
        ->setNext(treeHandler)
        ->setNext(trHandler);
  } else {
    base->setNext(normHandler)->setNext(treeHandler)->setNext(trHandler);
  }
  return base;
}

int Scene::getNumObjects() { return objects.size(); }

std::string Scene::getObjectName(int idx) {
  if (idx < 0 || idx > objects.size()) {
    return "invalid";
  }
  return objects.at(idx)->name;
}

int Scene::addVertex(Vertex p) {
  auto it = table.find(p);
  if (it == table.end()) {
    int pointIdx = scenePoints.size();
    table.insert({p, pointIdx});
    scenePoints.push_back(p);
    return pointIdx;
  }
  return it->second;
}

void Scene::render() {
  for (GLTriMesh *obj : objects) {
    if (obj->isVisible()) {
      obj->draw();
    }
  }
  //textRenderer->draw();
}

void Scene::w_press() { camera->walkFront(); }

void Scene::s_press() { camera->walkBack(); }

void Scene::a_press() { camera->walkLeft(); }

void Scene::d_press() { camera->walkRight(); }

void Scene::up_press() { camera->walkUp(); }

void Scene::down_press() { camera->walkDown(); }

void Scene::space_press() { camera->resetView(); }

void Scene::scrollUp() { camera->zoomIn(); }

void Scene::scrollDown() { camera->zoomOut(); }

void Scene::left_press(int step) {
  for (GLTriMesh *o : objects) {
    if (o->isVisible()) {
      for (int i = 0; i < step; i++) {
        //if (o->previousFrame()) {
        //  glWidget->notifyPrevious();
        //}
        o->previousFrame();
      }
    }
  }
}

void Scene::right_press(int step) {
  for (GLTriMesh *o : objects) {
    if (o->isVisible()) {
      for (int i = 0; i < step; i++) {
        //if (o->nextFrame()) {
        //  float score = o->getCurrentTriangle().classify();
        //  glWidget->notifyNext(score);
        //}
        o->nextFrame();
      }
    }
  }
}

void Scene::v_press() {
  for (GLTriMesh *obj : objects) {
    HandObject *o = dynamic_cast<HandObject *>(obj);
    if (o != nullptr) {
      o->setQuadVisible();
    }
  }
}

void Scene::increaseSpeed() { camera->increaseSpeed(); }

void Scene::decreaseSpeed() { camera->decreaseSpeed(); }

std::pair<int, Triangle> Scene::mousePickLeft(glm::vec3 virtualPos, int width,
                                              int height) {
  glm::mat4 view = camera->getViewMatrix();
  glm::mat4 proj = camera->getProjMatrix();
  glm::mat4 projView = proj * view;
  glm::mat4 projViewInv = glm::inverse(projView);
  float w = width;
  float h = height;
  glm::vec3 camerapos = camera->getPos();

  glm::vec4 pos(virtualPos.x, virtualPos.y, -1.f, 1.0f);

  pos.x = (pos.x / w) * 2.f - 1.f;
  pos.y = ((h - pos.y) / h) * 2.f - 1.f;

  pos = (projViewInv * pos);

  pos.w = 1.f / pos.w;

  pos.x *= pos.w;
  pos.y *= pos.w;
  pos.z *= pos.w;

  for (GLTriMesh *o : objects) {
    if (o->isVisible()) {
      int idx = o->pickTriangle(glm::vec3(pos));
      if (idx > -1) {
        return std::pair<int, Triangle>({idx, o->getTriangles()->at(idx)});
      }
    }
  }
  return {-1, Triangle()};
}

void Scene::mousePickRight(glm::vec3 virtualPos, int width, int height) {
  glm::mat4 view = camera->getViewMatrix();
  glm::mat4 proj = camera->getProjMatrix();
  glm::mat4 projView = proj * view;
  glm::mat4 projViewInv = glm::inverse(projView);
  float w = width;
  float h = height;
  glm::vec3 camerapos = camera->getPos();

  glm::vec4 pos(virtualPos.x, virtualPos.y, -1.f, 1.0f);

  pos.x = (pos.x / w) * 2.f - 1.f;
  pos.y = ((h - pos.y) / h) * 2.f - 1.f;

  pos = (projViewInv * pos);

  pos.w = 1.f / pos.w;

  pos.x *= pos.w;
  pos.y *= pos.w;
  pos.z *= pos.w;

  for (GLTriMesh *o : objects) {
    if (o->isVisible()) {
      for (int i = 0; i < o->getPoints()->size(); i++) {
        float dist =
            glm::length(glm::vec2(pos.x, pos.y) -
                        glm::vec2(o->getPoints()->at(i).position.x,
                                  o->getPoints()->at(i).position.y));
        if (dist < 0.00031f) {
          o->getPoints()->at(i).isActive =
              (o->getPoints()->at(i).isActive + 1) % 2;
        }
      }
    }
  }
}

int Scene::getIndexFromVertex(Vertex v) {
  auto it = table.find(v);
  if (it == table.end()) {
    return -1;
  }
  return it->second;
}

Vertex Scene::getVertexFromIndex(uint idx) {
  if (idx >= scenePoints.size()) {
    return Vertex();
  }
  return scenePoints.at(idx);
}

std::vector<Vertex> *Scene::getScenePoints() { return &scenePoints; }

//void Scene::updateText() {
//  textRenderer->clearTextList();
//  for (HandObject *ho : objects) {
//    if (ho->isVisible()) {
//      for (int i = 0; i < ho->getLocalPoints()->size(); i++) {
//        Vertex v = ho->getLocalPoints()->at(i);
//        if (v.isActive) {
//          float x = v.position.x;
//          float y = v.position.y;
//          textRenderer->addText("([" + std::to_string(i) + "], " +
//                                    std::format("{:.3f}", x) + ", " +
//                                    std::format("{:.3f}", y) + ")",
//                                x, y, 0.0006f, glm::vec3(0.f, 0.f, 1.f));
//        }
//      }
//    }
//  }
//}
