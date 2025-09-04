#ifndef SIDE_SUB_MENU_H
#define SIDE_SUB_MENU_H

#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QScrollArea>
#include <QToolButton>
#include <QIcon>
#include <QWidget>


class SideSubMenu : public QWidget {
    Q_OBJECT
private:
    QGridLayout mainLayout;
    QToolButton toggleButton;
    QFrame headerLine;
    QParallelAnimationGroup toggleAnimation;
    QScrollArea contentArea;
    int animationDuration;
public:
    SideSubMenu(const QString & title = "", const QIcon &icon = QIcon(),const int animationDuration = 300, QWidget *parent = 0);
    ~SideSubMenu() = default;
    void setContentLayout(QLayout & contentLayout);
};


#endif