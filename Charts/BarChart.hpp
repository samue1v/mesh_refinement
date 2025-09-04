#ifndef BAR_CHART_H
#define BAR_CHART_H
#include <QWidget>
#include <QBrush>
#include <QColor>
#include <QVector>
#include <algorithm>
#include "../qcustomplot/qcustomplot.h"

class BarChart : public QCPBars{
  Q_OBJECT
    public:
    BarChart(QCPAxis *keyAxis, QCPAxis *valueAxis);
    ~BarChart();

    public slots:
    //works like a facede, dont inherit from Chart but takes part on the chain
    void appendColor(float score);
    void popColor();
    void clearChart();

    public:
    
    void draw(QCPPainter *painter) override;


  
    QVector<double> barX,barY;
    QVector<QBrush> brushes;
    QVector<QColor> colors;
    std::vector<float> scoreHistory;



    


};

#endif