#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#include <QOpenGLWidget>
#include <QWidget>
// #include <QOpenGLFunctions_3_3_Core>

#include "../Common/DrawCommands.hpp"
#include "../MainScreen/MainWidget.hpp"
#include "InfoBox.hpp"
#include "Scene.hpp"
#include <QDateTime>
#include <QDir>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions>
#include <QOpenGLVersionFunctionsFactory>
#include <QString>
#include <QWheelEvent>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <stdlib.h>
#include <unordered_map>

class IMGUI_helper {
public:
  static std::unordered_map<int, ImGuiKey> qtToImGuiKey;
  static ImGuiKey QtKeyToImGuiKey(int qt_key);
  static void StyleColorsSpectrum();
};

class MyGLWidget : public QOpenGLWidget {
  Q_OBJECT

public:
  MyGLWidget(QWidget *parent = 0);
  ~MyGLWidget();
  void keyPressEvent(QKeyEvent *keyEvent) override;
  void wheelEvent(QWheelEvent *event) override;
  void mousePressEvent(QMouseEvent *evt) override;
  void mouseReleaseEvent(QMouseEvent *evt) override;
  void keyReleaseEvent(QKeyEvent *evt) override;
  void mouseMoveEvent(QMouseEvent *evt) override;
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;
  Scene *getScene();
  void createScene(MenuCommands::CreateSceneCMD &scene_cmd);
  void setVisibleObjects(std::string name);

  // private:
  // void widgetToPhoto();
  // void screenshot(QString dstPath, int example_number);

private:
  Scene *scene;
  InfoBox *info;
  MainWidget *mainMenu;

public:
  QOpenGLFunctions_3_3_Core *f;
};

#endif
