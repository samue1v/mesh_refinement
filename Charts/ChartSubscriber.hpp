#ifndef CHART_SUBSCRIBER_HPP
#define CHART_SUBSCRIBER_HPP

//chart interface
class ChartSubscriber{
  public:
  //append and color are the  Observer/Observable interface update method
  virtual void appendColor(float score) = 0;
  virtual void popColor() = 0;
  virtual void clearChart() = 0;
};

#endif