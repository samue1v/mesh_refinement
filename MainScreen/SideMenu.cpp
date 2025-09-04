#include "SideMenu.hpp"
#include "MainWidget.hpp"

SideMenu::SideMenu(MainWidget * parent) : QWidget(parent){
  mainMenu = parent;
  layout = new QVBoxLayout(this);
  transformationLayout = new QFormLayout;
  miscLayout = new QVBoxLayout;
  pcaLayout = new QHBoxLayout;
  objLayout = new QHBoxLayout;

  layout->setSpacing(0);
  layout->setContentsMargins(0,0,0,0);

  transformationLayout->setSpacing(0);
  transformationLayout->setContentsMargins(0,0,0,0);

  miscLayout->setSpacing(0);
  miscLayout->setContentsMargins(0,0,0,0);

  pcaLayout->setSpacing(0);
  pcaLayout->setContentsMargins(0,0,0,0);

  objLayout->setSpacing(0);
  objLayout->setContentsMargins(0,0,0,0);

  rotationLE = new QLineEdit();
  scaleLE = new QLineEdit();
  groupBoxPCA = new QGroupBox(tr("PCA"));
  buttonPCA0 = new QRadioButton(tr("&None"));
  buttonPCA1 = new QRadioButton(tr("&PCAT"));
  buttonPCA2 = new QRadioButton(tr("&PCART"));
  buttonPCA0->setChecked(true);
  numFrameSlider = new QSlider(Qt::Horizontal);
  numFrameSliderSmooth = new QSlider(Qt::Horizontal);
  stepCounterLabel = new QLabel("1");
  smoothCounterLabel = new QLabel("0");


  connect(numFrameSliderSmooth,&QSlider::valueChanged,this,&SideMenu::updateSmoothSlide);
  sliderWidgetSmooth = new QWidget();
  QHBoxLayout * sliderLayoutSmooth = new QHBoxLayout();
  sliderLayoutSmooth->addWidget(new QLabel("Smooth:"));
  sliderLayoutSmooth->addWidget(numFrameSliderSmooth);
  sliderLayoutSmooth->addWidget(smoothCounterLabel);
  sliderWidgetSmooth->setLayout(sliderLayoutSmooth);

  numFrameSliderSmooth->setMinimum(0);
  numFrameSliderSmooth->setMaximum(100);

  connect(numFrameSlider,&QSlider::valueChanged,this,&SideMenu::updateSlide);
  sliderWidget = new QWidget();
  QHBoxLayout * sliderLayout = new QHBoxLayout();
  sliderLayout->addWidget(new QLabel("Step:"));
  sliderLayout->addWidget(numFrameSlider);
  sliderLayout->addWidget(stepCounterLabel);
  sliderWidget->setLayout(sliderLayout);

  numFrameSlider->setMinimum(1);
  numFrameSlider->setMaximum(100);


  transformationLayout->addRow(QString("Rotation:"),rotationLE);
  transformationLayout->addRow(QString("Scale:"),scaleLE);

  miscLayout->addWidget(sliderWidget);
  miscLayout->addWidget(sliderWidgetSmooth);


  pcaLayout->addWidget(buttonPCA0);
  pcaLayout->addWidget(buttonPCA1);
  pcaLayout->addWidget(buttonPCA2);

  groupBoxPCA->setLayout(pcaLayout);

  objBox = new QComboBox;
  connect(objBox,&QComboBox::textActivated,this,&SideMenu::objBoxChosen);

  objLayout->addWidget(objBox);

  QIcon subMenuIcon = mainMenu->getIconByName("subMenu.png");
  
  transformSubMenu = new SideSubMenu("Transform",subMenuIcon,300,this);
  PCASubMenu = new SideSubMenu("PCA",QIcon(),300,this);
  miscSubMenu = new SideSubMenu("Misc",QIcon(),300,this);
  objSubMenu = new SideSubMenu("Objects",QIcon(),300,this);

  transformSubMenu->setContentLayout(*transformationLayout);
  miscSubMenu->setContentLayout(*miscLayout);
  PCASubMenu->setContentLayout(*pcaLayout);
  objSubMenu->setContentLayout(*objLayout);

  addSubMenu(transformSubMenu);
  addSubMenu(PCASubMenu);
  addSubMenu(miscSubMenu);
  addSubMenu(objSubMenu);

  setLayout(layout);
  
}

SideMenu::~SideMenu(){
  for(auto menu : subMenusVector){
    delete menu;
  }
}

void SideMenu::addSubMenu(SideSubMenu * submenu){
  subMenusVector.push_back(submenu);
  layout->addWidget(submenu);
}

void SideMenu::updateSlide(int val){
  stepCounterLabel->setNum(val);
}


void SideMenu::updateSmoothSlide(int val){
  smoothCounterLabel->setNum(val);
}

int SideMenu::getPCAVal(){
  if(buttonPCA0->isChecked()){
    return 0;
  }
  else if(buttonPCA1->isChecked()){
    return 1;
  }
  return 2;
}

void SideMenu::addObjectMenu(std::string name){
  objBox->addItem(mainMenu->getIconByName("geometria.png"),QString::fromStdString(name));
}

void SideMenu::clearObjectsMenu(){
  objBox->clear();
}

int SideMenu::getStepVal(){
  return numFrameSlider->value();
}


int SideMenu::getStepValSmooth(){
  return numFrameSliderSmooth->value();
}

float SideMenu::getRotationVal(){
  //returns 0.f in case text is nan
  return rotationLE->text().toFloat();
}

void SideMenu::objBoxChosen(QString name){
  mainMenu->objListChosen(name.toStdString());
}
