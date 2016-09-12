#ifndef __GAIN__
#define __GAIN__
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <TH1.h>
#include <TH1I.h>
#include <TH1D.h>
#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include <sys/time.h>
using namespace std;


struct APV{
  int APVId;
  int absoluteId;
  bool isMasked;
  float oldGain;
  float newGain;
  float MPV;
  float MPVError;
  float Width;
  float FitNorm;
  int NEntries;
};

class module{
public:
    module(int _detId, int _subDet){nAPV=0; _buffer = new APV * [17];subDet=_subDet; detId=_detId;}
    ~module(){delete _buffer;}
    int addAPV(APV * a);
    APV * getAPVbyId(int i){if (i<nAPV){return(_buffer[i]);}return(0);}
    APV * getAPV(int i){for (int j=0; j<nAPV; j++){if (_buffer[j]->APVId==i)return(_buffer[j]);}return(0);}
    float MPV;
    float Gain;
    float oldGain;
    int nAPV;
    int detId;
    int subDet;
    void average();
    bool isMasked;
private:
    APV ** _buffer;
};


class Gain{
public:
  module * Get(int i){if (i<nModules){return(_buffer[i]);} else{return(0);}};
  int printToFile(string fName);
  void average();
  int nModules;
  int nAPV;
  Gain(){_buffer=0;};
  ~Gain(){delete _buffer;}
  int getData(string fName);
  int addAPV(APV * a, int detId, int subDet);
  Gain(string fName){_buffer=0, getData(fName);};
  
private:
  module ** _buffer;
};

#endif
