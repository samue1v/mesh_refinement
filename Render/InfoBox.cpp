#include "InfoBox.hpp"

InfoBox::InfoBox(QWidget * parent) : QWidget(parent){
  //setAttribute(Qt::WA_TranslucentBackground);
  nameLabel = new QLabel("dadad");
  p0Label = new QLabel("dadad");
  p1Label = new QLabel("dadad");
  p2Label = new QLabel("dadad");
  scoreLabel = new QLabel("dadad");

  setFixedSize(parent->width()/5.f,parent->height()/5.f);
  setStyleSheet("background-color: rgba(255, 255, 255, 0.5);border: 1px solid rgba(50, 20, 20, .6);");


  layout = new QVBoxLayout(this);
  layout->setContentsMargins(0,0,0,0);
  layout->setSpacing(0);
  layout->addWidget(nameLabel);
  layout->addWidget(p0Label);
  layout->addWidget(p1Label);
  layout->addWidget(p2Label);
  layout->addWidget(scoreLabel);

  setLayout(layout);
  hide();

}

InfoBox::~InfoBox(){}

void InfoBox::update(int idx, Triangle t){
  if(idx>-1){  
    //Vertex * vtx = t.getVertices();
    float score = t.classify();
    //glm::vec3 v0 = vtx[0].position;
    //glm::vec3 v1 = vtx[1].position;
    //glm::vec3 v2 = vtx[2].position;
    glm::vec3 v0 = t.vertices[0]->position;
    glm::vec3 v1 = t.vertices[1]->position;
    glm::vec3 v2 = t.vertices[2]->position;
    nameLabel->setText(QString("Triangle index: %1").arg(idx));
    p0Label->setText(QString("P0: ( %1, %2 )").arg(v0.x).arg(v0.y));
    p1Label->setText(QString("P1: ( %1, %2 )").arg(v1.x).arg(v1.y));
    p2Label->setText(QString("P2: ( %1, %2 )").arg(v2.x).arg(v2.y));
    scoreLabel->setText(QString("Score: %1").arg(score));
    show();
  }
  else{
    hide();
  }
}

