#include "BarPlot.hpp"

BarPlot::BarPlot(QWidget * parent) : QCustomPlot(parent){
  barChart = new BarChart(xAxis,yAxis);
  setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
  xAxis->setLabel("Radi Score");
  yAxis->setLabel("Number of Triangles");
  xAxis->setRange(0,10);
  yAxis->setRange(0,1);
  xAxis->setPadding(1);
  xAxis->ticker()->setTickCount(10);

}

BarPlot::~BarPlot(){}

void BarPlot::appendColor(float score){
  barChart->appendColor(score);
  yAxis->setRange(0,*std::max_element(barChart->barY.constBegin(),barChart->barY.constEnd())*1.2);
  replot();
}

void BarPlot::popColor(){
  barChart->popColor();
  yAxis->setRange(0,*std::max_element(barChart->barY.constBegin(),barChart->barY.constEnd())/1.2f);
  replot();
}

void BarPlot::clearChart(){
  barChart->clearChart();
  yAxis->setRange(0,*std::max_element(barChart->barY.constBegin(),barChart->barY.constEnd())/1.2f);
  replot();
}

void BarPlot::displayData(){
  double spacing = 0.5;
  for(int x=0; x < barChart->barX.size(); x++){
    //Creating and configuring an item
    QCPItemText *textLabel = new QCPItemText(this);
    textLabel->setPadding(QMargins(0,0,0,0));
    textLabel->setClipToAxisRect(false);
    textLabel->position->setAxes(xAxis,yAxis);
    textLabel->position->setType(QCPItemPosition::ptPlotCoords);
    //placing the item over the bar with a spacing of 0.25
    textLabel->position->setCoords(x+spacing,barChart->barY[x]);
    //Customizing the item
    textLabel->setText(QString::number(barChart->barY[x]));
    textLabel->setFont(QFont(font().family(), 12));
    textLabel->setPen(QPen(Qt::black));
}
  //rescaleAxes();
  replot();
}
