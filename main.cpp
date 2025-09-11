#include "Render/MyGLWidget.hpp"
#include <QApplication>
#include <QSurfaceFormat>
#include <QTimer>

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  MyGLWidget *widget = new MyGLWidget();
  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setVersion(3, 3);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  QTimer *timer = new QTimer(widget);
  QObject::connect(timer, &QTimer::timeout, [widget]() { widget->update(); });
  timer->start(16);

  app.exec();
  return 0;
}
