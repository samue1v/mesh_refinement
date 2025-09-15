#include "MyGLWidget.hpp"
// #include <QtImGui.h>
#include <imgui.h>
// #include <implot.h>
#include "imgui_spectrum.h"
#include <iostream>
#include <unistd.h>

std::unordered_map<int, ImGuiKey> IMGUI_helper::qtToImGuiKey = {
    {Qt::Key_Tab, ImGuiKey_Tab},
    {Qt::Key_Left, ImGuiKey_LeftArrow},
    {Qt::Key_Right, ImGuiKey_RightArrow},
    {Qt::Key_Up, ImGuiKey_UpArrow},
    {Qt::Key_Down, ImGuiKey_DownArrow},
    {Qt::Key_PageUp, ImGuiKey_PageUp},
    {Qt::Key_PageDown, ImGuiKey_PageDown},
    {Qt::Key_Home, ImGuiKey_Home},
    {Qt::Key_End, ImGuiKey_End},
    {Qt::Key_Insert, ImGuiKey_Insert},
    {Qt::Key_Delete, ImGuiKey_Delete},
    {Qt::Key_Backspace, ImGuiKey_Backspace},
    {Qt::Key_Space, ImGuiKey_Space},
    {Qt::Key_Enter, ImGuiKey_Enter},
    {Qt::Key_Return, ImGuiKey_Enter},
    {Qt::Key_Escape, ImGuiKey_Escape},
    {Qt::Key_A, ImGuiKey_A},
    {Qt::Key_W, ImGuiKey_W},
    {Qt::Key_S, ImGuiKey_S},
    {Qt::Key_D, ImGuiKey_D},
    {Qt::Key_C, ImGuiKey_C},
    {Qt::Key_V, ImGuiKey_V},
    {Qt::Key_X, ImGuiKey_X},
    {Qt::Key_Y, ImGuiKey_Y},
    {Qt::Key_Z, ImGuiKey_Z},
};

ImGuiKey IMGUI_helper::QtKeyToImGuiKey(int qt_key) {
  auto it = qtToImGuiKey.find(qt_key);
  return (it != qtToImGuiKey.end()) ? it->second : ImGuiKey_None;
}

void IMGUI_helper::StyleColorsSpectrum() {
  ImGuiStyle *style = &ImGui::GetStyle();
  style->GrabRounding = 4.0f;

  ImVec4 *colors = style->Colors;
  colors[ImGuiCol_Text] = ImGui::ColorConvertU32ToFloat4(
      ImGui::Spectrum::GRAY800); // text on hovered controls is gray900
  colors[ImGuiCol_TextDisabled] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY500);
  colors[ImGuiCol_WindowBg] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY100);
  colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_PopupBg] = ImGui::ColorConvertU32ToFloat4(
      ImGui::Spectrum::GRAY50); // not sure about this. Note: applies to
                                // tooltips too.
  colors[ImGuiCol_Border] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY300);
  colors[ImGuiCol_BorderShadow] = ImGui::ColorConvertU32ToFloat4(
      ImGui::Spectrum::Static::NONE); // We don't want shadows. Ever.
  colors[ImGuiCol_FrameBg] = ImGui::ColorConvertU32ToFloat4(
      ImGui::Spectrum::GRAY75); // this isnt right, spectrum does not do this,
                                // but it's a good fallback
  colors[ImGuiCol_FrameBgHovered] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY50);
  colors[ImGuiCol_FrameBgActive] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY200);
  colors[ImGuiCol_TitleBg] = ImGui::ColorConvertU32ToFloat4(
      ImGui::Spectrum::GRAY300); // those titlebar values are totally made up,
                                 // spectrum does not have this.
  colors[ImGuiCol_TitleBgActive] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY200);
  colors[ImGuiCol_TitleBgCollapsed] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY400);
  colors[ImGuiCol_MenuBarBg] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY100);
  colors[ImGuiCol_ScrollbarBg] = ImGui::ColorConvertU32ToFloat4(
      ImGui::Spectrum::GRAY100); // same as regular background
  colors[ImGuiCol_ScrollbarGrab] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY400);
  colors[ImGuiCol_ScrollbarGrabHovered] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY600);
  colors[ImGuiCol_ScrollbarGrabActive] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY700);
  colors[ImGuiCol_CheckMark] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::BLUE500);
  colors[ImGuiCol_SliderGrab] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY700);
  colors[ImGuiCol_SliderGrabActive] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY800);
  colors[ImGuiCol_Button] = ImGui::ColorConvertU32ToFloat4(
      ImGui::Spectrum::GRAY75); // match default button to ImGui::Spectrum's
                                // 'Action Button'.
  colors[ImGuiCol_ButtonHovered] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY50);
  colors[ImGuiCol_ButtonActive] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY200);
  colors[ImGuiCol_Header] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::BLUE400);
  colors[ImGuiCol_HeaderHovered] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::BLUE500);
  colors[ImGuiCol_HeaderActive] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::BLUE600);
  colors[ImGuiCol_Separator] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY400);
  colors[ImGuiCol_SeparatorHovered] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY600);
  colors[ImGuiCol_SeparatorActive] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY700);
  colors[ImGuiCol_ResizeGrip] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY400);
  colors[ImGuiCol_ResizeGripHovered] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY600);
  colors[ImGuiCol_ResizeGripActive] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY700);
  colors[ImGuiCol_PlotLines] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::BLUE400);
  colors[ImGuiCol_PlotLinesHovered] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::BLUE600);
  colors[ImGuiCol_PlotHistogram] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::BLUE400);
  colors[ImGuiCol_PlotHistogramHovered] =
      ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::BLUE600);
  colors[ImGuiCol_TextSelectedBg] = ImGui::ColorConvertU32ToFloat4(
      (ImGui::Spectrum::BLUE400 & 0x00FFFFFF) | 0x33000000);
  colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
  colors[ImGuiCol_NavHighlight] = ImGui::ColorConvertU32ToFloat4(
      (ImGui::Spectrum::GRAY900 & 0x00FFFFFF) | 0x0A000000);
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
  colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

MyGLWidget::MyGLWidget(QWidget *parent) {
  setFixedSize(1280, 720);
  setFocusPolicy(Qt::ClickFocus);
  setContentsMargins(5, 5, 5, 5);
  setFocusPolicy(Qt::StrongFocus);
  setAttribute(Qt::WA_InputMethodEnabled, true);
  setMouseTracking(true);
  ctx = ImPlot::CreateContext();
  scene = nullptr;
  f = nullptr;

  show();
}

MyGLWidget::~MyGLWidget() {

  // f->glDeleteBuffers(1,&pointsVBO);
  if (scene != nullptr) {
    delete scene;
  }
  for (ChartSubscriber *subs : subscribers) {
    delete subs;
  }

  ImPlot::DestroyContext(ctx); 
  delete mainMenu;
  delete f;
  delete info;
}

void MyGLWidget::createScene(const MenuCommands::CreateSceneCMD &scene_cmd) {
  // makeCurrent();
  if (scene != nullptr) {
    delete scene;
    scene = nullptr;
    notifyClear();
  }
  scene = new Scene(this, context(), scene_cmd);

  // doneCurrent();
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

  // QtImGui::initialize(this);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // ImGui::StyleColorsLight();
  IMGUI_helper::StyleColorsSpectrum();

  ImGui_ImplOpenGL3_Init("#version 330");

  mainMenu = new MainWidget(this, context());

  subscribe(
      new BarChart(ctx, {mainMenu->size.x,
                    static_cast<float>(this->height() - this->height() / 4.)},
                   {static_cast<float>(this->width() * 2.5 / 6.),
                    static_cast<float>(this->height() / 4.)}));

  subscribe(
      new PieChart(ctx, { mainMenu->size.x + static_cast<float>(this->width() * 2.5 / 6.)+1 , 
                    static_cast<float>(this->height() - this->height() / 4.)},
                   {static_cast<float>(this->width() * 2.5 / 6.),
                    static_cast<float>(this->height() / 4.)}));
  info = new InfoBox(this);
  info->move({this->width() - info->width(), static_cast<int>(20)});

  // widgetToPhoto();
}

void MyGLWidget::paintGL() {
  if (width() <= 0 || height() <= 0)
    return;

  ImGuiIO &io = ImGui::GetIO();
  io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
  io.DisplaySize = ImVec2((float)width(), (float)height());

  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();

  f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Render scene here
  if (scene != nullptr) {
    scene->render();
  }

  mainMenu->render();
  if (mainMenu->showCharts) {
    for (ChartSubscriber *c : subscribers) {
      c->draw();
    }
  }
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  ImGui::EndFrame();
}

void MyGLWidget::resizeGL(int w, int h) {}

void MyGLWidget::keyPressEvent(QKeyEvent *keyEvent) {
  // if (keyEvent->key() == Qt::Key_1) {
  //   widgetToPhoto();
  // }
  ImGuiIO &io = ImGui::GetIO();
  ImGuiKey key = IMGUI_helper::QtKeyToImGuiKey(keyEvent->key());
  if (key != ImGuiKey_None)
    io.AddKeyEvent(key, true);

  io.AddKeyEvent(ImGuiMod_Ctrl, keyEvent->modifiers() & Qt::ControlModifier);
  io.AddKeyEvent(ImGuiMod_Shift, keyEvent->modifiers() & Qt::ShiftModifier);
  io.AddKeyEvent(ImGuiMod_Alt, keyEvent->modifiers() & Qt::AltModifier);
  io.AddKeyEvent(ImGuiMod_Super, keyEvent->modifiers() & Qt::MetaModifier);

  QString text = keyEvent->text();
  for (QChar ch : text) {
    io.AddInputCharacter(ch.unicode());
  }

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
  QWidget::keyPressEvent(keyEvent);
  // updateWidget();
}

void MyGLWidget::keyReleaseEvent(QKeyEvent *evt) {
  ImGuiIO &io = ImGui::GetIO();
  ImGuiKey key = IMGUI_helper::QtKeyToImGuiKey(evt->key());
  if (key != ImGuiKey_None)
    io.AddKeyEvent(key, false);

  io.AddKeyEvent(ImGuiMod_Ctrl, evt->modifiers() & Qt::ControlModifier);
  io.AddKeyEvent(ImGuiMod_Shift, evt->modifiers() & Qt::ShiftModifier);
  io.AddKeyEvent(ImGuiMod_Alt, evt->modifiers() & Qt::AltModifier);
  io.AddKeyEvent(ImGuiMod_Super, evt->modifiers() & Qt::MetaModifier);

  QWidget::keyReleaseEvent(evt);
}

void MyGLWidget::wheelEvent(QWheelEvent *event) {
  makeCurrent();

  ImGuiIO &io = ImGui::GetIO();
  io.AddMouseWheelEvent(0.0f, event->angleDelta().y() / 120.0f);

  if (scene != nullptr) {
    if (event->angleDelta().y() > 0) {
      scene->scrollUp();
    }

    else if (event->angleDelta().y() < 0) {
      scene->scrollDown();
    }
  }

  QWidget::wheelEvent(event);
  // updateWidget();
  doneCurrent();
}

void MyGLWidget::mousePressEvent(QMouseEvent *evt) {
  makeCurrent();
  ImGuiIO &io = ImGui::GetIO();
  if (evt->button() == Qt::LeftButton)
    io.AddMouseButtonEvent(0, true);
  if (evt->button() == Qt::RightButton)
    io.AddMouseButtonEvent(1, true);
  if (evt->button() == Qt::MiddleButton)
    io.AddMouseButtonEvent(2, true);

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
      //  scene->updateText();
    }
  }
  // evt->accept();
  // update();
  QWidget::mousePressEvent(evt);
  doneCurrent();
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent *evt) {
  ImGuiIO &io = ImGui::GetIO();
  if (evt->button() == Qt::LeftButton)
    io.AddMouseButtonEvent(0, false);
  if (evt->button() == Qt::RightButton)
    io.AddMouseButtonEvent(1, false);
  if (evt->button() == Qt::MiddleButton)
    io.AddMouseButtonEvent(2, false);

  QWidget::mouseReleaseEvent(evt);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *evt) {
  ImGuiIO &io = ImGui::GetIO();
  io.AddMousePosEvent(evt->position().x(), evt->position().y());
  QWidget::mouseMoveEvent(evt);
}

void MyGLWidget::setVisibleObjects(std::string name) {
  scene->setVisible(name);
  // updateWidget();
}

void MyGLWidget::subscribe(ChartSubscriber *chart) {
  subscribers.push_back(chart);
}

void MyGLWidget::notifyNext(float score) {
  for (ChartSubscriber *s : subscribers) {
    s->appendColor(score);
  }
}

void MyGLWidget::notifyPrevious() {
  for (ChartSubscriber *s : subscribers) {
    s->popColor();
  }
}

void MyGLWidget::notifyClear() {
  for (ChartSubscriber *s : subscribers) {
    s->clearChart();
  }
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
