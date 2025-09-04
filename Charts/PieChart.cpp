#include "PieChart.hpp"

PieChart::PieChart(QGraphicsItem *parent , Qt::WindowFlags wFlags) : QChart(parent,wFlags){
  goodCount = 0;
  avgCount = 0;
  badCount = 0;
  pieSeries = new QPieSeries();

  QColor greenColor = QColor(112, 250, 126);
  QColor yellowColor = QColor(198, 184, 0);
  QColor redColor = QColor(255, 38, 0);

  goodSlice = new QPieSlice(); 
  goodSlice->setPen(QPen(greenColor));
  goodSlice->setBrush(QBrush(greenColor));
  goodSlice->setLabel("Good");

  //connect(goodSlice,&QPieSlice::hovered,this,&PieChart::goodHovered); 

  
  avgSlice = new QPieSlice(); 
  avgSlice->setPen(QPen(yellowColor));
  avgSlice->setBrush(QBrush(yellowColor));
  avgSlice->setLabel("Average");
  
  //connect(avgSlice,&QPieSlice::hovered,this,&PieChart::avgHovered); 

  
  badSlice = new QPieSlice();
  badSlice->setPen(QPen(redColor));
  badSlice->setBrush(QBrush(redColor));
  badSlice->setLabel("Bad");

  //connect(badSlice,&QPieSlice::hovered,this,&PieChart::badHovered); 

  
  pieSeries->append(goodSlice);
  pieSeries->append(avgSlice);
  pieSeries->append(badSlice);


  
  
  
  addSeries(pieSeries);
  setTitle("Radius Ratio");
}

PieChart::~PieChart(){
  delete pieSeries;
  delete goodSlice;
  delete avgSlice;
  delete badSlice;
}

void PieChart::appendColor(float score){



  scoreHistory.push_back(score);
  if(score < 0.4){
    ++badCount;
    pieSeries->take(badSlice);
    badSlice->setValue(badCount); 
    pieSeries->append(badSlice); 
    
  }
  
  else if(score >= 0.4 && score < 0.7){
    ++avgCount;
    pieSeries->take(avgSlice);
    avgSlice->setValue(avgCount); 
    pieSeries->append(avgSlice);
    
  }

  else if(score >= 0.7 && score <= 1.1){
    ++goodCount;
    pieSeries->take(goodSlice);
    goodSlice->setValue(goodCount); 
    pieSeries->append(goodSlice);
    
    
  }
  badHovered(true);
  avgHovered(true);
  goodHovered(true);

}

void PieChart::popColor(){
  if(scoreHistory.size() == 0){//defaul value
    return;
  }
  float lastScore = scoreHistory.back();
  scoreHistory.pop_back();
  if(lastScore < 0.4){
    --badCount;
    pieSeries->take(badSlice);
    badSlice->setValue(badCount);
    pieSeries->append(badSlice); 
    
  }
  
  else if(lastScore >= 0.4 && lastScore < 0.7){
    --avgCount;
    pieSeries->take(avgSlice);
    avgSlice->setValue(avgCount); 
    pieSeries->append(avgSlice);
    
  }

  else if(lastScore >= 0.7 && lastScore <= 1.1){
    --goodCount;
    pieSeries->take(goodSlice);
    goodSlice->setValue(goodCount); 
    pieSeries->append(goodSlice);
    
  }
  badHovered(true);
  avgHovered(true);
  goodHovered(true);
  
  

}


void PieChart::goodHovered(bool state){
    goodSlice->setExploded(state);
    goodSlice->setLabel(QString("Good:").append(QString("%1%").arg(100*calcPercentage(goodCount), 0, 'f', 1)));
    goodSlice->setLabelVisible(state);

}

void PieChart::avgHovered(bool state){
    avgSlice->setExploded(state);
    avgSlice->setLabel(QString("Average:").append(QString("%1%").arg(100*calcPercentage(avgCount), 0, 'f', 1)));
    avgSlice->setLabelVisible(state);
}

void PieChart::badHovered(bool state){
    badSlice->setExploded(state);
    badSlice->setLabel(QString("Bad:").append(QString("%1%").arg(100*calcPercentage(badCount), 0, 'f', 1)));
    badSlice->setLabelVisible(state);
}

void PieChart::clearChart(){



  scoreHistory.clear();
  
  pieSeries->take(goodSlice);
  pieSeries->take(avgSlice);
  pieSeries->take(badSlice);

  goodSlice->setValue(0); 
  goodCount = 0;


  avgSlice->setValue(0); 
  avgCount = 0;


  badSlice->setValue(0); 
  badCount = 0;
  
  pieSeries->append(goodSlice);
  pieSeries->append(avgSlice);
  pieSeries->append(badSlice); 


  badHovered(true);
  avgHovered(true);
  goodHovered(true);

}

double PieChart::calcPercentage(int sliceVal){
  if(scoreHistory.size() == 0){
    return 0.;
  }
  return (double)sliceVal/(double)scoreHistory.size();
}