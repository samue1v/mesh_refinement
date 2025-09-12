#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include "../Charts/ChartSubscriber.hpp"
#include "../Common/DrawCommands.hpp"
#include "../Render/GLObject.hpp"
#include "../Common/DrawCommands.hpp"
#include "Publisher.hpp"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include <QOpenGLContext>
#include <QWidget>
#include <string>
#include <vector>

class MyGLWidget;

class MainWidget : public GLDrawable, public Publisher {
public:
  MainWidget(MyGLWidget *_glWidget, QOpenGLContext *context);
  ~MainWidget();
  MainWidget(const MainWidget &) = delete;
  MainWidget &operator=(const MainWidget &) = delete;
  MainWidget(MainWidget &&) = delete;
  MainWidget &operator=(MainWidget &&) = delete;

  void subscribe(ChartSubscriber *) override;
  void notifyNext(float score) override;
  void notifyPrevious() override;
  void notifyClear() override;
  void fileChosen(const std::string & file);
  void objListChosen(std::string name);
  void render();

  void ShowMainMenuBar();
  void ShowSideMenuBar();
  void ShowFileMenu();
  void openFile(const std::string & path);

  void draw() override;
  void init() override;

  // Interface for getting menu data
  int getStep();
  int getSmoothStep();

public:
  MyGLWidget *glWidget;
  std::vector<ChartSubscriber *> subscribers;
  std::vector<std::string> objList;

private:
  bool showMenu;
  bool showCharts;
  bool openDialog;



private:
  float MainMenuHeight;
  float ScaleFactor;
  float RotationAngle;
  MenuCommands::MeshingApproach selectedAnalytical;
  MenuCommands::PostProcessingApproach selectedPostProcessing;
  int step;

};

#endif
