#ifndef BAR_CHART_H
#define BAR_CHART_H
#include "ChartSubscriber.hpp"
#include "imgui.h"
#include "implot.h"
#include <QWidget>
#include <algorithm>
#include <vector>

class BarChart : public ChartSubscriber {
public:
  BarChart(const ImVec2 &_pos, const ImVec2 &_size);
  ~BarChart();

public slots:
  void appendColor(float score) override;
  void popColor() override;
  void clearChart() override;
  void draw() override;

  std::vector<int> barData;
  std::vector<float> dataHistory;
  ImPlotContext *ctx;
  ImVec2 pos;
  ImVec2 size;
};

#endif
