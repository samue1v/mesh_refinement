#include "MainWidget.hpp"

MainWidget::MainWidget(QWidget * parent) : QWidget(parent){
  setFixedSize(1900,720);
  loadIcons();

  layout = new QGridLayout(this);
  glWidget = new MyGLWidget(this);

  glWidget->setEventManager(this);
  glWidget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

  layout->setContentsMargins(5,5,5,5);
  layout->setSpacing(5);


  fileChooser = new FileChooser(this);

  sideMenu = new SideMenu(this);


  barPlot = new BarPlot(this);
  barPlot->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
  barPlot->setMaximumWidth(this->width()/4.f);
  barPlot->setMaximumHeight(this->height()/2.f);

  pieChart = new PieChart();
  pieChartView = new QChartView();
  pieChart->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
  //pieChart->setMaximumWidth(this->width()/4.f);
  //pieChart->setMaximumHeight(this->height()/2.f);
  pieChart->setMaximumWidth(this->width()/1.f);
  pieChart->setMaximumHeight(this->height()/1.f);
  pieChartView->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
  pieChartView->setMaximumWidth(this->width()/1.f);
  pieChartView->setMaximumHeight(this->height()/1.f);
  pieChartView->setRenderHint(QPainter::Antialiasing);
  pieChartView->setChart(pieChart);
  pieChartView->setParent(this);

  subscribe(barPlot);
  subscribe(pieChart);

  layout->addWidget(fileChooser,0,0,3,12);
  layout->addWidget(sideMenu,3,0,1,12);
  layout->addWidget(glWidget,0,12,10,36);
  layout->addWidget(pieChartView,0,48,5,6);
  layout->addWidget(barPlot,5,48,5,6);


  setLayout(layout);


  show();
}


MainWidget::~MainWidget(){
  delete layout;


}

void MainWidget::fileChosen(QString filePath){

  //Update side menu
  glWidget->createScene(filePath,sideMenu->getPCAVal());
  sideMenu->clearObjectsMenu();
  sideMenu->addObjectMenu("All");
  for(int i = 0; i<glWidget->getScene()->getNumObjects();i++){
    sideMenu->addObjectMenu(glWidget->getScene()->getObjectName(i));
  }
  notifyClear();
  glWidget->updateWidget();

}

void MainWidget::subscribe(ChartSubscriber * chart){
  subscribers.push_back(chart);
}

void MainWidget::notifyNext(float score){
  for(ChartSubscriber * s : subscribers ){
    s->appendColor(score);
  }
}

void MainWidget::notifyPrevious(){
  for(ChartSubscriber * s : subscribers ){
    s->popColor();
  }
}

void MainWidget::notifyClear(){
  for(ChartSubscriber * s : subscribers ){
    s->clearChart();
  }
}

int MainWidget::getStep(){
  return sideMenu->getStepVal();
}

int MainWidget::getSmoothStep(){
  return sideMenu->getStepValSmooth();
}

void MainWidget::loadIcons(){
  QDir dir(QDir::currentPath());
  dir.cd("Misc/Icons");
  QStringList images = dir.entryList(QStringList() << "*.png" << "*.PNG",QDir::Files);
  for(QString i : images){
    std::string stdIconName = i.toStdString();
    size_t slashPos = stdIconName.find_last_of("/");
    std::string iconName = stdIconName.substr(slashPos+1);
    QString namesds = dir.path()+ "/" + i;
    icons.push_back(std::make_pair(iconName,QIcon(dir.path()+ "/" + i)));
  }
}

QIcon MainWidget::getIconByName(std::string name){
  for(std::pair<std::string,QIcon> p : icons){
    if(p.first == name){
      return p.second;
    }
  }
  return QIcon();
}

void MainWidget::objListChosen(std::string name){
  notifyClear();
  glWidget->setVisibleObjects(name,sideMenu->getPCAVal());
}

AbsHandler * MainWidget::buildHandlers(){
  return nullptr;
}
