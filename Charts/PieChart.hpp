#ifndef PIE_CHART_H
#define PIE_CHART_H

#include <QChart>
#include <QPieSeries>
#include <QPieSlice>
#include <QChart>
#include <QChartView>
#include <QGraphicsItem>
#include <Qt>
#include <vector>
#include <iostream>
#include "ChartSubscriber.hpp"




class PieChart : public QChart, public ChartSubscriber {
Q_OBJECT
public:
  PieChart(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = Qt::WindowFlags());
  ~PieChart();

public slots:
  void appendColor(float v) override;
  void popColor() override;
  void clearChart() override;
  void goodHovered(bool state);
  void avgHovered(bool state);
  void badHovered(bool state);

public:
  QPieSeries * pieSeries;
  QPieSlice * goodSlice;
  QPieSlice * avgSlice;
  QPieSlice * badSlice;
  int goodCount;
  int avgCount;
  int badCount;
  std::vector<float> scoreHistory;

private:
  double calcPercentage(int val);
};

#endif //PIE_CHART_H
