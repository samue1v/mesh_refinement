#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#include <QOpenGLWidget>
#include <QWidget>
//#include <QOpenGLFunctions_3_3_Core>

#include <QOpenGLVersionFunctionsFactory>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QDateTime>
#include <QTimer>
#include <QEvent>
#include <QKeyEvent>
#include <QDir>
#include <vector>
#include <QString>
#include <QWheelEvent>
#include <QMouseEvent>
#include <cmath>
#include <stdlib.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "InfoBox.hpp"
#include "Scene.hpp"


class MainWidget;

class MyGLWidget : public QOpenGLWidget{

  Q_OBJECT
public:
  //MyGLWidget(QWidget * parent = 0);
  MyGLWidget(QWidget * parent = 0);
  ~MyGLWidget();
  void keyPressEvent(QKeyEvent * keyEvent) override;
  void wheelEvent(QWheelEvent *event) override;
  void mousePressEvent(QMouseEvent* evt) override;
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override; 
  Scene * getScene();
  void createScene(QString fileName,int type);
  void setEventManager(MainWidget *);
  void setVisibleObjects(std::string name,int transformType);
  void updateWidget();
  void notifyPrevious();
  void notifyNext(float score);

//private:
  void widgetToPhoto();
  void screenshot(QString dstPath,int example_number);


private:

  Scene * scene;
  QOpenGLFunctions_3_3_Core * f;
  MainWidget * eventManager;
  InfoBox * info;



};


#endif
