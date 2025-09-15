#include "MainWidget.hpp"
#include "../Render/MyGLWidget.hpp"
#include "ImGuiFileDialog.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

MainWidget::MainWidget(MyGLWidget *_glWidget, QOpenGLContext *context)
    : GLDrawable(context), glWidget(_glWidget), showMainMenu(true),
      showSideMenu(true), openDialog(false), showCharts(true),
      RotationAngle(-1), ScaleFactor(-1), step(1),
      selectedPostProcessing(MenuCommands::PostProcessingApproach::None),
      selectedAnalytical(MenuCommands::MeshingApproach::None) {
  init();
}

MainWidget::~MainWidget() {
  // clear charts
}

void MainWidget::init() {
  size = ImVec2(glWidget->width() / 6.f, glWidget->height() - 19);
  pos = ImVec2(0, 19);
}

void MainWidget::render() { this->draw(); }

void MainWidget::ShowSideMenuBar() {
  ImGui::SetNextWindowSize(size, ImGuiCond_Always);
  ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
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
    if (ImGui::BeginMenu("View")) {
      if (ImGui::Checkbox("Side Menu", &showSideMenu)) {
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
    ImGui::SetNextWindowSize({static_cast<float>(glWidget->width()/2.), static_cast<float>(glWidget->height()/2.)});
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
}

int MainWidget::getStep() { return step; }

int MainWidget::getSmoothStep() { return step; }

void MainWidget::objListChosen(std::string name) {
  glWidget->notifyClear();
  glWidget->setVisibleObjects(name);
}

void MainWidget::draw() {
  MainWidget::ShowMainMenuBar();
  if (showSideMenu) {
    MainWidget::ShowSideMenuBar();
  }
}
