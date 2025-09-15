#include "BarChart.hpp"
#include <algorithm>
#include <iostream>

#define BAR_WIDTH 0.5f

BarChart::BarChart(const ImVec2 &_pos, const ImVec2 &_size)
    : pos(_pos), size(_size), barData(10, 0) {
  ctx = ImPlot::CreateContext();
  ImPlot::SetCurrentContext(ctx);
}

BarChart::~BarChart() { ImPlot::DestroyContext(ctx); }

void BarChart::appendColor(float score) {
  if (score >= 1) {
    score = 0.99;
  }
  barData.push_back(score);
  barData[int((score) * 10)] += 1;
}

void BarChart::popColor() {
  float lastScore = dataHistory.back();
  barData[int((lastScore) * 10)] =
      std::max(barData[int((lastScore) * 10)] - 1, 0);
  dataHistory.pop_back();
}

void BarChart::clearChart() {
  std::fill(barData.begin(), barData.end(), 0);
  dataHistory.clear();
}

void BarChart::draw() {
  ImGui::SetNextWindowPos(pos);
  ImGui::SetNextWindowSize(size);
  ImGui::Begin("##RadiusR", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_None |
                   ImGuiWindowFlags_NoTitleBar);
  if (ImPlot::BeginPlot("Radius Ratio", size, ImPlotFlags_NoLegend)) {
    ImPlot::SetupAxisLimits(ImAxis_X1, 0.0,
                            *std::max_element(barData.begin(), barData.end()),
                            ImPlotCond_Always);
    ImPlot::PlotBars("Score", barData.data(), barData.size(), BAR_WIDTH, 1,
                     ImPlotBarsFlags_Horizontal);
    ImPlot::EndPlot();
  }
  ImGui::End();
}
