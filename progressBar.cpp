#ifndef __PROG_BAR_CPP_
#define __PROG_BAR_CPP_


#include "progressBar.h"

int get_ms(timeval * t1, timeval * t2){
    long seconds  = -t1->tv_sec  + t2->tv_sec;
    long useconds = -t1->tv_usec + t2->tv_usec;
    return(seconds*1000+useconds/1000+0.5);
}


progressBar::progressBar(int nMax_, int nSteps_):nMax(nMax_),nSteps(nSteps_),prevStep(0),lastPrinted(0){
  gettimeofday(&tStart,NULL);
  tLastPrinted=tStart;
  if (nSteps<2)nSteps=2;
  printEvery = 0.5;
}

void progressBar::step(int i){
   gettimeofday(&tStep,NULL);
   lastStep=i;
   if (get_ms(&tLastPrinted,&tStep)>1000*printEvery){
    printOut(i);
    lastPrinted=i;
    gettimeofday(&tLastPrinted,NULL);
  }
}

void progressBar::printOut(int i){
  // check if printOut.
  int s = 0; 
  ostringstream outBuff;
  while (i> s*nMax*1./nSteps){
    outBuff<<".";
    s++;
  }
  if (s<nSteps){
    for (int i=0; i<nSteps-s; i++)outBuff<<" ";
  }
  outBuff<<i*100./nMax<<"% done; eta ";
  gettimeofday(&tStep,NULL); 
  float speed = (i)*1000./get_ms(&tStart,&tStep);
//   tStart=tStop;
  prevStep=i;
//   outBuff<<"("<<nMax<<";"<<i<<")";
  outBuff<<(float)(nMax-i)*1./speed<<" s";
  cout<<"\r"<<outBuff.str()<<std::flush;
}


void progressBar::start(){
  
  string Header="|";
  for (int i=0; i<nSteps-2; i++)Header+=".";
  Header+="|";
  
  cout<<Header<<endl;
  
}
void progressBar::stop(){
  printOut(lastStep+1);
  cout<<endl;
}
#endif