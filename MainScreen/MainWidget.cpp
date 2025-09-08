#include "MainWidget.hpp"
#include "../Render/MyGLWidget.hpp"

MainWidget::MainWidget(MyGLWidget *_glWidget) : glWidget(_glWidget) {
  f = glWidget->f;
}

MainWidget::~MainWidget() {
  // clear charts
  for (ChartSubscriber *subs : subscribers) {
    delete subs;
  }
}

void MainWidget::init() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  ImGui::StyleColorsDark();

  ImGui_ImplOpenGL3_Init("#version 330");
}

void MainWidget::render() { this->draw(); }

void MainWidget::fileChosen(std::string filePath) {

  MenuCommands::CreateSceneCMD cmd;
  cmd.objPath = filePath;
  // complete with other stuff...
  glWidget->createScene(cmd);
  for (int i = 0; i < glWidget->getScene()->getNumObjects(); i++) {
    objList.push_back(glWidget->getScene()->getObjectName(i));
  }
  notifyClear();
}

void MainWidget::subscribe(ChartSubscriber *chart) {
  subscribers.push_back(chart);
}

void MainWidget::notifyNext(float score) {
  for (ChartSubscriber *s : subscribers) {
    s->appendColor(score);
  }
}

void MainWidget::notifyPrevious() {
  for (ChartSubscriber *s : subscribers) {
    s->popColor();
  }
}

void MainWidget::notifyClear() {
  for (ChartSubscriber *s : subscribers) {
    s->clearChart();
  }
}

int MainWidget::getStep() {
  // return sideMenu->getStepVal();
}

int MainWidget::getSmoothStep() {
  // return sideMenu->getStepValSmooth();
}

void MainWidget::objListChosen(std::string name) {
  notifyClear();
  glWidget->setVisibleObjects(name, sideMenu->getPCAVal());
}

AbsHandler *MainWidget::buildHandlers() { return nullptr; }
