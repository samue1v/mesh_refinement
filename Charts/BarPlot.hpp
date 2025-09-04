#ifndef BAR_PLOT_H
#define BAR_PLOT_H
#include <QWidget>
#include <iostream>
#include <QMargins>
#include "../qcustomplot/qcustomplot.h"
#include "BarChart.hpp"
#include "ChartSubscriber.hpp"

class  BarPlot : public QCustomPlot, public ChartSubscriber{
Q_OBJECT
public:
  BarPlot(QWidget * parent = nullptr);
  ~BarPlot();


public slots:
  void appendColor(float score) override;
  void popColor() override;
  void clearChart() override;
  void displayData();
  

public:
  BarChart * barChart;
};


#endif
