#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QChartView>
#include <QSizePolicy>
#include <QPen>
#include <QVector>
#include <QBrush>
#include <QColor>
#include <vector>
#include <QSizePolicy>
#include <algorithm>
#include <QDir>
#include <QStringList>
#include <QString>
#include <QIcon>
#include <QTreeView>
#include <QFileSystemModel>
#include <QComboBox>
#include "../Charts/BarChart.hpp"
#include "../Charts/PieChart.hpp"
#include "../Render/MyGLWidget.hpp"
#include "../Charts/BarPlot.hpp"
#include "SideMenu.hpp"
#include "Publisher.hpp"
#include "FileChooser.hpp"




class MainWidget : public QWidget, public Publisher{
Q_OBJECT
  public:
  MainWidget(QWidget * parent  = 0);
  ~MainWidget();


  public slots:
  void fileChosen(QString filePath);
  void subscribe(ChartSubscriber *) override;
  void notifyNext(float score) override;
  void notifyPrevious() override;
  void notifyClear() override;
  QIcon getIconByName(std::string name);
  void objListChosen(std::string name);

  // Interface for getting menu data
  int getStep();
  int getSmoothStep();

  private:
  void loadIcons();
  AbsHandler * buildHandlers();




 // private:
  public:
  QGridLayout * layout;
  MyGLWidget * glWidget;
  SideMenu * sideMenu;


  BarPlot * barPlot;
  PieChart * pieChart;
  QChartView * pieChartView;
  FileChooser * fileChooser;

  std::vector<ChartSubscriber * > subscribers;

  std::vector<std::pair<std::string,QIcon>> icons;


}; 


#endif
