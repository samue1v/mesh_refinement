#include "MainWidget.hpp"
#include "../Render/MyGLWidget.hpp"
#include "ImGuiFileDialog.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

MainWidget::MainWidget(MyGLWidget *_glWidget, QOpenGLContext *context)
    : GLDrawable(context), glWidget(_glWidget), showMenu(true),
      openDialog(false), showCharts(true), RotationAngle(-1), ScaleFactor(-1),
      selectedPostProcessing(MenuCommands::PostProcessingApproach::None),
      selectedAnalytical(MenuCommands::MeshingApproach::None) {
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
    std::ifstream fi("user_data.json");
    json data;
    if (fi.peek() != std::ifstream::traits_type::eof()) {
      data = json::parse(fi);
      auto &recent = data["recentFiles"];
      for (auto it = recent.rbegin(); it != recent.rend(); ++it) {
        std::string s = it->get<std::string>();
        if (ImGui::MenuItem(s.c_str())) {
          fileChosen(s);
        }
      }
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
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
      logChosenFile(filePathName);
      fileChosen(filePathName);
    }

    // close
    ImGuiFileDialog::Instance()->Close();
  }
}

void MainWidget::logChosenFile(const std::string &name) {
  std::ifstream fi("user_data.json");
  constexpr int recent_files_max_count = 5;

  json data;

  if (fi.peek() != std::ifstream::traits_type::eof()) { // not empty
    data = json::parse(fi);
  } else {
    data = json::object(); // start with empty JSON object
  }

  std::string key = "recentFiles";
  if (!data.contains(key)) {
    data[key] = json::array();
  }
  if (data[key].size() < recent_files_max_count) {
    data[key].push_back(name);
  } else {
    data[key].erase(data[key].end() - 1);
    data[key].push_back(name);
  }
  std::ofstream fo("user_data.json");
  fo << data;
}

void MainWidget::fileChosen(const std::string &filePath) {

  std::cout << filePath << "\n";
  MenuCommands::CreateSceneCMD cmd;
  cmd.objPath = filePath;
  cmd.MeshApproach = selectedAnalytical;
  cmd.PostApproach = selectedPostProcessing;
  cmd.transformation.scale = ScaleFactor;
  cmd.transformation.rotation = RotationAngle;
  glWidget->createScene(cmd);
  for (int i = 0; i < glWidget->getScene()->getNumObjects(); i++) {
    objList.push_back(glWidget->getScene()->getObjectName(i));
  }
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
