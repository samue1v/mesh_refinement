#include "PieChart.hpp"
#include <algorithm>
#include <iostream>

PieChart::PieChart(ImPlotContext *ctx, const ImVec2 &_pos, const ImVec2 &_size)
    : pos(_pos), size(_size), pieData(4, 0) {
  ImPlot::SetCurrentContext(ctx);
}

PieChart::~PieChart() {}

void PieChart::appendColor(float score) {
  if (score >= 1) {
    score = 0.99;
  }
  dataHistory.push_back(score);
  int part = score / 0.25;
  pieData[part] += 1;
}

void PieChart::popColor() {
  float lastScore = dataHistory.back();
  int part = lastScore / 0.25;
  pieData[part] = std::max(pieData[part] - 1, 0);
  dataHistory.pop_back();
}

void PieChart::clearChart() {
  std::fill(pieData.begin(), pieData.end(), 0);
  dataHistory.clear();
}

void PieChart::draw() {
  static ImVec4 my_colors[4] = {
      ImVec4(1.0f, 0.0f, 0.0f, 1.0f),  // red
      ImVec4(1.0f, 0.65f, 0.0f, 1.0f), // orange
      ImVec4(1.0f, 1.0f, 0.0f, 1.0f),  // yellow
      ImVec4(0.0f, 1.0f, 0.0f, 1.0f),  // green
  };

  static int MyCMap = ImPlot::AddColormap("MyColormap", my_colors, IM_ARRAYSIZE(my_colors));

  ImGui::SetNextWindowPos(pos);
  ImGui::SetNextWindowSize(size);

  static const char *labels1[] = {"Poor", "Fair", "Good", "Very good"};
  static ImU32 colors[] = {IM_COL32(255, 0, 0, 255), IM_COL32(255, 165, 0, 255),
                           IM_COL32(255, 255, 0, 255),
                           IM_COL32(0, 255, 0, 255)};
  static ImPlotPieChartFlags flags = ImPlotPieChartFlags_Exploding;

  ImGui::Begin("##RadiusP", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
  ImPlot::PushColormap(MyCMap);
  if (ImPlot::BeginPlot("##Pie1", size,
                        ImPlotFlags_Equal | ImPlotFlags_NoMouseText)) {

    // No need for axes for a pie chart, but you can keep limits if you want
    ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations,
                      ImPlotAxisFlags_NoDecorations);
    ImPlot::SetupAxesLimits(0, 1, 0, 1);

    // Draw pie chart
    ImPlot::PlotPieChart(labels1, pieData.data(), 4, 0.5, 0.5, 0.1, "%.2f", 90,
                         flags);

    ImPlot::EndPlot();
  }
  ImPlot::PopColormap();

  ImGui::End();
}
