#ifndef PIE_CHART_H
#define PIE_CHART_H
#include "ChartSubscriber.hpp"
#include "imgui.h"
#include "implot.h"
#include <QWidget>
#include <algorithm>
#include <vector>

class PieChart : public ChartSubscriber {
public:
  PieChart(ImPlotContext * ctx, const ImVec2 &_pos, const ImVec2 &_size);
  ~PieChart();

public slots:
  void appendColor(float score) override;
  void popColor() override;
  void clearChart() override;
  void draw() override;

  std::vector<int> pieData;
  std::vector<float> dataHistory;
  ImVec2 pos;
  ImVec2 size;
};

#endif
