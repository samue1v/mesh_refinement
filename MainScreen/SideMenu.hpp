#ifndef SIDE_MENU_H
#define SIDE_MENU_H

#include <QWidget>
#include <vector>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QString>
#include <QSlider>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>
#include <QComboBox>
#include "SideSubMenu.hpp"

class MainWidget;

class SideMenu : public QWidget{
Q_OBJECT
  public:
  SideMenu(MainWidget * parent = nullptr);
  ~SideMenu();
  void addSubMenu(SideSubMenu * submenu);
  float getRotationVal();
  int getPCAVal();
  int getStepVal();

  int getStepValSmooth();

  public slots:
  void updateSlide(int val);

  void updateSmoothSlide(int val);

  void addObjectMenu(std::string name);

  void clearObjectsMenu();

  void objBoxChosen(QString name);
  
  
  
  private:
  std::vector<SideSubMenu *> subMenusVector;
  QVBoxLayout * layout;
  QFormLayout * transformationLayout;
  QVBoxLayout * miscLayout;
  QHBoxLayout * pcaLayout;
  QHBoxLayout * objLayout;
  QLineEdit * rotationLE;
  QLineEdit * scaleLE;
  QSlider * numFrameSlider;
  QSlider * numFrameSliderSmooth;
  QWidget * sliderWidget;
  QWidget * sliderWidgetSmooth;
  QWidget * pcaWidget;
  QLabel * stepCounterLabel;
  QLabel * smoothCounterLabel;
  QGroupBox *groupBoxPCA; 
  QRadioButton * buttonPCA0;
  QRadioButton * buttonPCA1;
  QRadioButton * buttonPCA2;
  QComboBox * objBox;


  SideSubMenu *transformSubMenu;
  SideSubMenu *PCASubMenu;
  SideSubMenu *miscSubMenu;
  SideSubMenu *objSubMenu;

  MainWidget * mainMenu;

};

#endif
