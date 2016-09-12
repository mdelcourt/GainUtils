 
#include "GainComparator.cpp"

bool all (APV * a){return(true);}

int compareTest(){
//   Gain * gOld = new Gain("Gains_Tree.root");
  gainComparator comp;
  comp.DisableRefit();
  comp.DisableCC();

  comp.AddPayload("New gain","Gains_Tree.root");
  comp.AddPayload("Old gain","Gains_Tree_01.root");
  
  comp.BookHistos();
  comp.FillHistos();
  comp.DrawHistos();
//   payloadAnalyser analyser("oldGain",new Gain("Gains_Tree.root"),0);
  comp.DrawHistos(true);

  return(1);
}