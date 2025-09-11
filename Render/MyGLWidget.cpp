#include "MyGLWidget.hpp"
#include <QtImGui.h>
#include <imgui.h>
// #include <implot.h>
#include <iostream>
#include <unistd.h>
MyGLWidget::MyGLWidget(QWidget *parent) {
  setFixedSize(1280, 960);
  setFocusPolicy(Qt::ClickFocus);
  setContentsMargins(5, 5, 5, 5);
  info = new InfoBox(this);
  scene = nullptr;
  f = nullptr;



  show();
}

MyGLWidget::~MyGLWidget() {

  // f->glDeleteBuffers(1,&pointsVBO);
  delete scene;
  delete f;
  delete info;
}

void MyGLWidget::createScene(MenuCommands::CreateSceneCMD &scene_cmd) {
  makeCurrent();
  if (scene != nullptr) {
    delete scene;
  }
  scene = new Scene(this, context(), scene_cmd);
  doneCurrent();
}

Scene *MyGLWidget::getScene() { return scene; }

void MyGLWidget::initializeGL() {

  f = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(context());
  f->initializeOpenGLFunctions();
  f->glEnable(GL_DEPTH_TEST);
  f->glEnable(GL_DEPTH_CLAMP);
  f->glEnable(GL_BLEND);
  f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  f->glEnable(GL_LINE_SMOOTH);
  f->glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  f->glEnable(GL_PROGRAM_POINT_SIZE);
  f->glPointSize(2);
  f->glLineWidth(1);

  f->glClearColor(255.0, 255.0, 255.0, 1.0f);

  QtImGui::initialize(this);

  // IMGUI_CHECKVERSION();
  // ImGui::CreateContext();

  // ImGui::StyleColorsDark();

  // ImGui_ImplOpenGL3_Init("#version 330");

  mainMenu = new MainWidget(this, context());

  // widgetToPhoto();
}

void MyGLWidget::paintGL() {
  // if (width() <= 0 || height() <= 0)
  //   return;

  // ImGuiIO &io = ImGui::GetIO();
  // io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
  // io.DisplaySize = ImVec2((float)width(), (float)height());

  // ImGui_ImplOpenGL3_NewFrame();
  // ImGui::NewFrame();
  //

  f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  QtImGui::newFrame();

  // Render scene here
  //
  if (scene != nullptr) {
    scene->render();
  }

  mainMenu->render();

  ImGui::Render();
  QtImGui::render();
  // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MyGLWidget::resizeGL(int w, int h) {}

void MyGLWidget::keyPressEvent(QKeyEvent *keyEvent) {
  makeCurrent();
  // if (keyEvent->key() == Qt::Key_1) {
  //   widgetToPhoto();
  // }

  if (scene != nullptr) {

    if (keyEvent->key() == Qt::Key_Left) {
      scene->left_press(mainMenu->getStep());
    }

    else if (keyEvent->key() == Qt::Key_Right) {
      scene->right_press(mainMenu->getStep());
    }

    else if (keyEvent->key() == Qt::Key_W) {
      scene->w_press();
    }

    else if (keyEvent->key() == Qt::Key_S) {
      scene->s_press();
    }

    else if (keyEvent->key() == Qt::Key_D) {
      scene->d_press();
    }

    else if (keyEvent->key() == Qt::Key_A) {
      scene->a_press();
    }

    else if (keyEvent->key() == Qt::Key_Up) {
      scene->up_press();
    }

    else if (keyEvent->key() == Qt::Key_Down) {
      scene->down_press();
    }

    else if (keyEvent->key() == Qt::Key_Space) {
      scene->space_press();
    }

    else if (keyEvent->key() == Qt::Key_V) {
      scene->v_press();
    }

    else if (keyEvent->key() == Qt::Key_8) {
      scene->scrollDown();
    }

    else if (keyEvent->key() == Qt::Key_9) {
      scene->scrollUp();

    } else if (keyEvent->key() == Qt::Key_C) {
      scene->getCamera()->centerCamera(
          scene->currentObj->getQuadTree()->getRootCenter(),
          scene->currentObj->getQuadTree()->getRootHeight());

    }

    else if (keyEvent->key() == Qt::Key_Plus) {
      scene->increaseSpeed();
    } else if (keyEvent->key() == Qt::Key_Minus) {
      scene->decreaseSpeed();
    }
  }
  updateWidget();
  doneCurrent();
}

void MyGLWidget::wheelEvent(QWheelEvent *event) {
  makeCurrent();
  if (scene != nullptr) {
    if (event->angleDelta().y() > 0) {
      scene->scrollUp();
    }

    else if (event->angleDelta().y() < 0) {
      scene->scrollDown();
    }
  }
  updateWidget();
  doneCurrent();
}

void MyGLWidget::mousePressEvent(QMouseEvent *evt) {
  makeCurrent();
  ImGuiIO &io = ImGui::GetIO();
  if (evt->button() == Qt::LeftButton)
    io.MouseDown[0] = true;
  if (evt->button() == Qt::RightButton)
    io.MouseDown[1] = true;

  if (scene) {
    if (evt->button() == Qt::LeftButton) {
      QPointF virtualPos = evt->pos();
      float w = width();
      float h = height();
      std::pair<int, Triangle> ret = scene->mousePickLeft(
          glm::vec3(virtualPos.x(), virtualPos.y(), 0.f), w, h);
      info->update(ret.first, ret.second);
    } else if (evt->button() == Qt::RightButton) {
      QPointF virtualPos = evt->pos();
      float w = width();
      float h = height();
      scene->mousePickRight(glm::vec3(virtualPos.x(), virtualPos.y(), 0.f), w,
                            h);
      // scene->updateText();
    }
  }
  // evt->accept();
  update();
  doneCurrent();
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent *evt) {
  makeCurrent();
  ImGuiIO &io = ImGui::GetIO();
  if (evt->button() == Qt::LeftButton)
    io.MouseDown[0] = false;
  if (evt->button() == Qt::RightButton)
    io.MouseDown[1] = false;

  // evt->accept();
  update();
  doneCurrent();
}

void MyGLWidget::setVisibleObjects(std::string name) {
  scene->setVisible(name);
  updateWidget();
}

void MyGLWidget::updateWidget() {
  makeCurrent();
  // info->update(-1);
  update();
  doneCurrent();
}

// void MyGLWidget::widgetToPhoto() {
//
//   // scene->getCamera()->setPos({-1.f,-1.f,1.f});
//   std::vector<std::string> dirNames = {"good/", "bad/", "idk/"};
//   //std::string srcRootDir =
//   "/home/galeg0/Documents/back/pibic/objFiles/testes/";
//   //std::string dstRootDir = "screenshots/full_result/";
//   std::string srcRootDir =
//   "/home/galeg0/Documents/back/smooth_screenshot/objFiles/testes/";
//   std::string dstRootDir =
//   "/home/galeg0/Documents/back/smooth_screenshot/screenshots/full_result/";
//   std::cout << this->width() << "x" << this->height() << std::endl;
//   QStringList filters;
//   filters << "*.obj";
//   for (auto d : dirNames) {
//     QString finalSrcPath = QString::fromStdString(srcRootDir + d);
//     QDir dir(finalSrcPath);
//     dir.setNameFilters(filters);
//     dir.setFilter(QDir::Files | QDir::NoSymLinks);
//     QStringList fileList = dir.entryList();
//
//     std::string dstPath = dstRootDir + d;
//     int count = 0;
//     for (auto s : fileList) {
//
//       std::string fileSrc = finalSrcPath.toStdString() + s.toStdString();
//       //createScene(QString::fromStdString(fileSrc), 1);
//       createScene(QString::fromStdString(fileSrc), 0);
//       // std::cout << fileSrc << std::endl;
//       QDir().mkpath(
//           QString::fromStdString(dstPath)); // Ensure the directory exists
//       std::cout << dstPath << std::endl;
//       screenshot(QString::fromStdString(dstPath), count);
//       count++;
//     }
//   }
// }
//
// void MyGLWidget::screenshot(QString dstPath, int example_number) {
//
//   for (int i = 0; i < scene->objects.size(); i++) {
//     eventManager->notifyClear();
//     setVisibleObjects(scene->objects[i]->name, 0);
//     int frameSize = scene->objects[i]->getFrames().size();
//     scene->right_press(frameSize);
//     float height = scene->objects[i]->getQuadTree()->getRootHeight();
//     glm::vec3 quadCenter = scene->objects[i]->getQuadTree()->getRootCenter();
//
//     float fullHeight = height;
//     float aspect = float(this->width()) / float(this->height());
//     float fullWidth = fullHeight * aspect;
//
//     // Half sizes of the full square
//     float halfW = fullWidth / 2.0f;
//     float halfH = fullHeight / 2.0f;
//
//     // Offset from center to reach each quadrant's center
//     float quarterW = halfW / 2.0f; // or just: quarterW = fullWidth / 2.0f;
//     float quarterH = halfH / 2.0f;
//
//     // Offsets relative to the square center
//     glm::vec3 offsets[4] = {
//         {-quarterW, quarterH, 0.0f},  // Top-left
//         {quarterW, quarterH, 0.0f},   // Top-right
//         {-quarterW, -quarterH, 0.0f}, // Bottom-left
//         {quarterW, -quarterH, 0.0f}   // Bottom-right
//     };
//
//     float quadrantHeight = fullHeight / 2.0f;
//     // for (int j = 0; j < 4; j++) {
//     scene->getCamera()->centerCamera(quadCenter, height);
//     QPixmap pixmap0 =
//         eventManager->barPlot->grab(); // myWidget is a pointer to any
//         QWidget
//     QPixmap pixmap1 = eventManager->pieChartView
//                           ->grab(); // myWidget is a pointer to any QWidget
//                                     //
//
//        int width_pix = pixmap0.width() + pixmap1.width();
//     int height_pix = qMax(pixmap0.height(), pixmap1.height());
//
//     // Create new pixmap and painter
//     QPixmap combined(width_pix, height_pix);
//     combined.fill(Qt::transparent); // Optional: fill background if needed
//
//     QPainter painter(&combined);
//     painter.drawPixmap(0, 0, pixmap0);
//     painter.drawPixmap(pixmap0.width(), 0, pixmap1);
//     painter.end();
//
//
//
//
//     // Save result
//     combined.save(dstPath +
//                  QString("e_%1_%2_graph_smooth.png").arg(example_number).arg(i));
//     // QImage screenshot = this->grabFramebuffer();
//     // QString filename =
//     //     dstPath +
//     //     QString("e_%1_%2_smooth.png").arg(example_number).arg(i);
//     // screenshot.save(filename);
//     //}
//   }
// }
