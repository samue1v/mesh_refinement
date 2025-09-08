#include <QApplication>
#include <QSurfaceFormat>
#include "Render/MyGLWidget.hpp"


int main(int argc, char ** argv){
    QApplication app(argc,argv);
    MyGLWidget * widget = new MyGLWidget();
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    app.exec();
    return 0;

}
