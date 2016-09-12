#ifndef __GAIN_CPP__
#define __GAIN_CPP__
#include "Gain.h"


int module::addAPV(APV * a){
  for (int i=0; i<nAPV; i++){
    if (_buffer[i]->APVId==a->APVId){
      return(0);
    }
  }
  if (nAPV>17){
    cerr<<"APV memory full !"<<endl;
    return(0);
  }
  _buffer[nAPV]=a;
  nAPV++;
  return(1);
}

void module::average(){
  Gain=0;
  oldGain=0;
  MPV=0;
  isMasked=false;
  for(int i=0; i<nAPV; i++){
    isMasked+=getAPVbyId(i)->isMasked;
    MPV+=getAPVbyId(i)->MPV*1./nAPV;
    Gain+=getAPVbyId(i)->newGain*1./nAPV;
    oldGain+=getAPVbyId(i)->oldGain*1./nAPV;
  }
}

void Gain::average(){
  for(int i=0; i<nModules; i++){
    Get(i)->average();
  }
}
int Gain::addAPV(APV * a, int detId, int subDet){
  if (!_buffer){_buffer = new module * [17000];}
  for(int i=0; i<nModules; i++){
    if (_buffer[i]->detId==detId){_buffer[i]->addAPV(a);return(1);}
  }
  _buffer[nModules]= new module(detId,subDet);
  _buffer[nModules]->addAPV(a);
  nModules++;
  //WARNING Never checked if created
  return(1);
}

int Gain::printToFile(string fName){
  ofstream myFile(fName.c_str());
  if (! myFile.is_open()){cerr<<"Can't open file !"<<endl; return(0);}
  average();
  for (int i=0; i<nModules; i++){
    myFile<<Get(i)->detId<<"#"<<Get(i)->Gain<<endl;
  }
  myFile.close();
  return(1);
}

int Gain::getData(string fName){
   if (_buffer){
     delete _buffer;
   }
   TFile * f=new TFile(fName.c_str());
   TTree * t = (TTree*) f->Get("SiStripCalib/APVGain");
   
   if (!t){
     delete f;
     return(-1);
   }
  
  nAPV = t->GetEntries();
  cout<<"Reading file "<<fName<<endl;
  cout<<"Num entries : "<<nAPV<<endl;
  nModules=0;
  
  TLeaf *  DetIdLeaf= t->GetLeaf("DetId");
  TLeaf *  APVIdLeaf= t->GetLeaf("APVId");
  TLeaf *  isMaskedLeaf = t->GetLeaf("isMasked");
  TLeaf *  PrevGainLeaf = t->GetLeaf("PrevGain");
  TLeaf *  SubDetLeaf = t->GetLeaf("SubDet");
  TLeaf *  GainLeaf = t->GetLeaf("Gain");
  TLeaf *  MPVLeaf= t->GetLeaf("FitMPV");
  TLeaf *  MPVErrorLeaf= t->GetLeaf("FitMPVErr");
  TLeaf *  WidthLeaf= t->GetLeaf("FitWidth");
  TLeaf *  FitNormLeaf= t->GetLeaf("FitNorm");
  TLeaf *  NEntriesLeaf= t->GetLeaf("NEntries");
  

  
  for (int j=0; j<nAPV; j++){
    t->GetEntry(j);
    APV * a = new APV;
    a->absoluteId = j;
    a->oldGain = PrevGainLeaf->GetValue();
    a->newGain = GainLeaf->GetValue();
    a->APVId = APVIdLeaf->GetValue();
    a->isMasked = isMaskedLeaf->GetValue();
    a->MPV = MPVLeaf->GetValue();
    a->MPVError = MPVErrorLeaf->GetValue();
    a->Width = WidthLeaf->GetValue();
    a->FitNorm = FitNormLeaf->GetValue();
    a->NEntries = NEntriesLeaf->GetValue();
    addAPV(a,DetIdLeaf->GetValue(),SubDetLeaf->GetValue());
  }
   
 return(1);
}
 
#endif