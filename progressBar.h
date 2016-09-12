#ifndef __PROG_BAR__
#define __PROG_BAR__
#include <iostream>
#include <sys/time.h>
#include <sstream>
using namespace std;


class progressBar{
public:
  progressBar(int nMax_, int nSteps_=100);
  void step(int i);
  void printOut(int i);
  void start();
  void stop();
private:
  timeval tStart,tStep,tLastPrinted;
  int nMax;
  int nSteps;
  int prevStep;
  float printEvery;
  int lastPrinted;
  int lastStep;
};
#endif