#include <QApplication>
#include <iostream>
#include <QHBoxLayout>
#include <string.h>
#include <fstream>
#include "MainScreen/MainWidget.hpp"


int main(int argc, char ** argv){
    QApplication app(argc,argv);
    MainWidget * widget = new MainWidget();
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    app.exec();
    return 0;

}