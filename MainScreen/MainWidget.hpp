#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include "../Common/DrawCommands.hpp"
#include "../Render/GLObject.hpp"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include <QOpenGLContext>
#include <QWidget>
#include <string>
#include <vector>

class MyGLWidget;

class MainWidget : public GLDrawable {
public:
  MainWidget(MyGLWidget *_glWidget, QOpenGLContext *context);
  ~MainWidget();
  MainWidget(const MainWidget &) = delete;
  MainWidget &operator=(const MainWidget &) = delete;
  MainWidget(MainWidget &&) = delete;
  MainWidget &operator=(MainWidget &&) = delete;

  void fileChosen(const std::string &file);
  void objListChosen(std::string name);
  void logChosenFile(const std::string &name);
  void render();

  void ShowMainMenuBar();
  void ShowSideMenuBar();
  void ShowFileMenu();
  void openFile(const std::string &path);

  void draw() override;
  void init() override;

  // Interface for getting menu data
  int getStep();
  int getSmoothStep();

public:
  MyGLWidget *glWidget;
  std::vector<std::string> objList;
  float MainMenuHeight;
  ImVec2 size;
  ImVec2 pos;

private:
  bool showMenu;
  bool showCharts;
  bool openDialog;

private:
  float ScaleFactor;

  float RotationAngle;
  MenuCommands::MeshingApproach selectedAnalytical;
  MenuCommands::PostProcessingApproach selectedPostProcessing;
  int step;
};

#endif
