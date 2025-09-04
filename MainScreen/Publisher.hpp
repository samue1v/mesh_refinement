#ifndef PUBLISHER_HPP
#define PUBLISHER_HPP

#include "../Charts/ChartSubscriber.hpp"

//PUBLISHER INTERFACE
class Publisher{
public:
  virtual void subscribe(ChartSubscriber *) = 0;
  virtual void notifyNext(float score) = 0;
  virtual void notifyPrevious() = 0;
  virtual void notifyClear() = 0;

};

#endif