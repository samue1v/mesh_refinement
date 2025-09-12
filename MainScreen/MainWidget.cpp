#include "MainWidget.hpp"
#include "../Render/MyGLWidget.hpp"
#include "ImGuiFileDialog.h"
#include <string>

MainWidget::MainWidget(MyGLWidget *_glWidget, QOpenGLContext *context)
    : GLDrawable(context), glWidget(_glWidget), showMenu(true),
      openDialog(false), showCharts(true), RotationAngle(-1), ScaleFactor(-1) {
  init();
}

MainWidget::~MainWidget() {
  // clear charts
  for (ChartSubscriber *subs : subscribers) {
    delete subs;
  }
}

void MainWidget::init() {}

void MainWidget::render() { this->draw(); }

void MainWidget::ShowSideMenuBar() {
  ImGui::SetNextWindowSize(ImVec2(300, glWidget->height() - MainMenuHeight),
                           ImGuiCond_Always);
  ImGui::SetNextWindowPos(ImVec2(0, MainMenuHeight), ImGuiCond_Always);
  ImGui::Begin("Mesh Menu", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
  if (ImGui::TreeNodeEx("Geometric transforms",
                        ImGuiTreeNodeFlags_DefaultOpen)) {

    ImGui::Spacing();

    ImGui::Text("Scale");
    ImGui::Spacing();
    ImGui::InputFloat("##ScaleInput", &ScaleFactor);

    ImGui::Separator();
    ImGui::Text("Rotation");
    ImGui::Spacing();
    ImGui::InputFloat("##RotationInput", &RotationAngle);

    ImGui::TreePop();
  }

  if (ImGui::TreeNodeEx("Analytical transforms",
                        ImGuiTreeNodeFlags_DefaultOpen)) {

    ImGui::Spacing();

    if (ImGui::RadioButton("None", selectedAnalytical ==
                                       MenuCommands::MeshingApproach::None))
      selectedAnalytical = MenuCommands::MeshingApproach::None;
    if (ImGui::RadioButton("PCA - Transformed Axis",
                           selectedAnalytical ==
                               MenuCommands::MeshingApproach::PCA))
      selectedAnalytical = MenuCommands::MeshingApproach::PCA;
    if (ImGui::RadioButton("PCA - Original Axis",
                           selectedAnalytical ==
                               MenuCommands::MeshingApproach::PCAInv))
      selectedAnalytical = MenuCommands::MeshingApproach::PCAInv;

    ImGui::TreePop();
  }

  if (ImGui::TreeNodeEx("Post Processing", ImGuiTreeNodeFlags_DefaultOpen)) {

    ImGui::Spacing();

    if (ImGui::RadioButton("None",
                           selectedPostProcessing ==
                               MenuCommands::PostProcessingApproach::None))
      selectedPostProcessing = MenuCommands::PostProcessingApproach::None;
    if (ImGui::RadioButton("Smooth",
                           selectedPostProcessing ==
                               MenuCommands::PostProcessingApproach::Smooth))
      selectedPostProcessing = MenuCommands::PostProcessingApproach::Smooth;

    ImGui::TreePop();
  }

  if (ImGui::TreeNodeEx("Rendering options", ImGuiTreeNodeFlags_DefaultOpen)) {

    ImGui::Spacing();

    if (ImGui::SliderInt("Step", &step, 1, 100)) {
    }

    ImGui::TreePop();
  }

  ImGui::End();
}

void MainWidget::ShowMainMenuBar() {
  if (ImGui::BeginMainMenuBar()) {
    MainMenuHeight = ImGui::GetWindowHeight();
    if (ImGui::BeginMenu("File")) {
      ShowFileMenu();
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Layout")) {
      if (ImGui::Checkbox("Main Menu", &showMenu)) {
      };
      if (ImGui::Checkbox("Chart Menu", &showCharts)) {
      };
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  openFile(".");
}

void MainWidget::ShowFileMenu() {

  if (ImGui::MenuItem("New")) {
  }
  if (ImGui::MenuItem("Open", "Ctrl+O")) {
    openDialog = true;
  }
  if (ImGui::BeginMenu("Open Recent")) {
    ImGui::MenuItem("fish_hat.c");
    ImGui::MenuItem("fish_hat.inl");
    ImGui::MenuItem("fish_hat.h");
    if (ImGui::BeginMenu("More..")) {
      ImGui::MenuItem("Hello");
      ImGui::MenuItem("Sailor");
      ImGui::EndMenu();
    }
    ImGui::EndMenu();
  }
}

void MainWidget::openFile(const std::string &path) {
  if (openDialog) {
    openDialog = false; // reset flag
    IGFD::FileDialogConfig config;
    config.path = path;
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File",
                                            ".obj", config);
  }

  if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {

    if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
      std::cout << "Dentro\n";
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
      fileChosen(filePathName);
    }

    // close
    ImGuiFileDialog::Instance()->Close();
  }
}

void MainWidget::fileChosen(const std::string &filePath) {

  std::cout << filePath << "\n";
  // MenuCommands::CreateSceneCMD cmd;
  // cmd.objPath = filePath;
  // glWidget->createScene(cmd);
  // for (int i = 0; i < glWidget->getScene()->getNumObjects(); i++) {
  //   objList.push_back(glWidget->getScene()->getObjectName(i));
  // }
  // notifyClear();
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
  glWidget->setVisibleObjects(name);
}

void MainWidget::draw() {
  // static bool show_demo_window = true;
  // ImGui::ShowDemoWindow(&show_demo_window);
  MainWidget::ShowMainMenuBar();
  MainWidget::ShowSideMenuBar();
}
