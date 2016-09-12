#ifndef __GAIN_COMP_H__
#define __GAIN_COMP_H__

#include "TFile.h"
#include "TH2.h"
#include "TH2D.h"
#include "TCanvas.h"
#include <functional>
#include <iostream>
#include "Gain.cpp"
#include "Cuts.cpp"
#include "TLegend.h"
#include "progressBar.cpp"

class histHolder{
public:
  histHolder(){}
  void link(bool doRefit_=true, bool doCC_=true);
  vector <TH1*> hists;
  vector <string> names;
  TH1F * MPV;
  TH1F * G2;
  TH1F * Error;
  TH1F * GainDiff;
  TH1 * RefitMPV;
  TH1 * RefitDiff;
  TH1 * CC;
  TH1 * RefitPar0        ;
  TH1 * RefitWidth        ;
  TH1 * RefitMPVError      ;
  TH1 * RefitWidthError     ;
  TH1 * RefitPar0Error       ;
};



class cut{
public:
  cut(string name_,std::function <bool(APV*)> cutFn_);
  bool pass(APV * a);
  string name;
  std::function <bool(APV *)> cutFn;
};

class payloadAnalyser{
public:
  payloadAnalyser(string name_,Gain * g_,TH2 * ClusterCharge_,bool doRefit_,bool doCC_);
  void AddCut(string name_,std::function <bool(APV*)> cutFn_);
  void BookHistos();
  void FillHistos();
  vector < histHolder * > histos;
  vector < cut > cuts;
  string name;

private:
  bool doRefit,doCC;
  Gain * g;
  TH2 * ClusterCharge;
  //TODO add destructor
};





class crossHistHolder{
public:
  histHolder(){}
  void link(bool doRefit_=true, bool doCC_=true);
  vector <TH1*> hists;
  vector <string> names;
  TH1F * MPV;
  TH1F * G2;
  TH1F * Error;
  TH1F * GainDiff;
  TH1 * RefitMPV;
  TH1 * RefitDiff;
  TH1 * CC;
  TH1 * RefitPar0        ;
  TH1 * RefitWidth        ;
  TH1 * RefitMPVError      ;
  TH1 * RefitWidthError     ;
  TH1 * RefitPar0Error       ;
};


class gainComparator{
public:
  gainComparator();
  void AddPayload(string userName,string fName);
  
  void DisableRefit();
  void DisableCC();
  void BookHistos();
  void FillHistos();
  void DrawHistos(bool norm = false);
  vector <payloadAnalyser *> payloads;
  ~gainComparator();
private:
  bool doRefit;
  bool doCC;
  void DefineStyle();
  vector <int> colours;
  vector <int> lineStyle;
  vector <int> lineThickness;
};



#endif