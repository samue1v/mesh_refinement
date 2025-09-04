#ifndef INFO_BOX_HPP
#define INFO_BOX_HPP
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "../Logic/Triangle.hpp"

class InfoBox : public QWidget{
Q_OBJECT
  public:
  InfoBox(QWidget * parent = 0);
  ~InfoBox();
  void update(int idx=-1, Triangle t=Triangle());

  private:
  QLabel * nameLabel;
  QLabel * p0Label;
  QLabel * p1Label;
  QLabel * p2Label;
  QLabel * scoreLabel;
  QVBoxLayout * layout;

};


#endif