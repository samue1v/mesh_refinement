#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include "../Charts/ChartSubscriber.hpp"
#include "../Common/DrawCommands.hpp"
#include "../Render/GLObject.hpp"
#include "Publisher.hpp"
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include <QWidget>
#include <vector>
#include <string>

class MyGLWidget;

class MainWidget : public GLDrawable, public Publisher {
public:
  MainWidget(MyGLWidget * _glWidget);
  ~MainWidget();
  MainWidget(const MainWidget &) = delete;
  MainWidget &operator=(const MainWidget &) = delete;
  MainWidget(MainWidget &&) = delete;
  MainWidget &operator=(MainWidget &&) = delete;

  void fileChosen(std::string filePath);
  void subscribe(ChartSubscriber *) override;
  void notifyNext(float score) override;
  void notifyPrevious() override;
  void notifyClear() override;
  void objListChosen(std::string name);
  void render();

  void draw() override;
  void init() override;

  // Interface for getting menu data
  int getStep();
  int getSmoothStep();

public:
  MyGLWidget *glWidget;
  std::vector<ChartSubscriber *> subscribers;
  std::vector<std::string> objList;
};

#endif
