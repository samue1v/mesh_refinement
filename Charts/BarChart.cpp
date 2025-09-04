#include "BarChart.hpp"
#include <iostream>

#define BAR_WIDTH 0.5f

BarChart::BarChart(QCPAxis *keyAxis, QCPAxis *valueAxis) : QCPBars(keyAxis,valueAxis){

  colors.append(QColor(255, 38, 0));
  colors.append(QColor(249, 85, 0));
  colors.append(QColor(239, 116, 0));
  colors.append(QColor(228, 141, 0));
  colors.append(QColor(214, 164, 0));
  colors.append(QColor(198, 184, 0));
  colors.append(QColor(180, 202, 0));
  colors.append(QColor(161, 219, 45));
  colors.append(QColor(139, 235, 87));
  colors.append(QColor(112, 250, 126));

  for(int i =0;i<10;i++){
    brushes.append(QBrush(colors[i]));
  }
  barX = QVector<double>(10);
  barY = QVector<double>(10);
  for (int i=0; i<10; ++i){
    barX[i] = i+BAR_WIDTH;
  }



  setWidth(BAR_WIDTH);
  setData(barX,barY);

  setPen(Qt::NoPen);
  setBrush(QColor(10, 140, 70, 160));
  
  

}

BarChart::~BarChart(){
}

void BarChart::appendColor(float score){
  if(score>=1){
    score = 0.99;
  }
  scoreHistory.push_back(score);
  barY[int((score)*10)] += 1;
  setData(barX,barY);
}

void BarChart::popColor(){
  float lastScore = scoreHistory.back();
  barY[int((lastScore)*10)] = std::max(barY[int((lastScore)*10)]-1,0.);
  scoreHistory.pop_back();
  setData(barX,barY);
}

void BarChart::clearChart(){
  for(int i = 0; i<barY.size();i++){
    barY[i] = 0;
  }
  scoreHistory.clear();
  setData(barX,barY);
}


void BarChart::draw(QCPPainter *painter){
  if (!mKeyAxis || !mValueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
    if (mDataContainer->isEmpty()) return;
    
    QCPBarsDataContainer::const_iterator visibleBegin, visibleEnd;
    getVisibleDataBounds(visibleBegin, visibleEnd);
    
    // loop over and draw segments of unselected/selected data:
    QList<QCPDataRange> selectedSegments, unselectedSegments, allSegments;
    getDataSegments(selectedSegments, unselectedSegments);
    allSegments << unselectedSegments << selectedSegments;
    //qDebug() << "All segments:"<< mDataContainer.get() << "\n";
    for (int i=0; i<allSegments.size(); ++i)
    {
      
      bool isSelectedSegment = i >= unselectedSegments.size();
      QCPBarsDataContainer::const_iterator begin = visibleBegin;
      QCPBarsDataContainer::const_iterator end = visibleEnd;
      mDataContainer->limitIteratorsToDataRange(begin, end, allSegments.at(i));
      if (begin == end)
        continue;
      int count = 0;
      for (QCPBarsDataContainer::const_iterator it=begin; it!=end; ++it)
      {
        // check data validity if flag set:
  #ifdef QCUSTOMPLOT_CHECK_DATA
        if (QCP::isInvalidData(it->key, it->value))
          qDebug() << Q_FUNC_INFO << "Data point at" << it->key << "of drawn range invalid." << "Plottable name:" << name();
  #endif
        // draw bar:
        if (isSelectedSegment && mSelectionDecorator)
        {
          mSelectionDecorator->applyBrush(painter);
          mSelectionDecorator->applyPen(painter);
        } else
        {
          painter->setBrush(brushes[count]);
          painter->setPen(mPen);
        }
        applyDefaultAntialiasingHint(painter);
        painter->drawPolygon(getBarRect(it->key, it->value));
        ++count;
      }
    }
    
    // draw other selection decoration that isn't just line/scatter pens and brushes:
    if (mSelectionDecorator)
      mSelectionDecorator->drawDecoration(painter, selection());

}