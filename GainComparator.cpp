#include "GainComparator.h"
#include "TF1.h"
void getPeakOfLandau(TH1* InputHisto, double* FitResults, double LowRange=50, double HighRange=5400)
{
//   cout<<"Start peak"<<endl;
   FitResults[0]         = -0.5;  //MPV
   FitResults[1]         =  0;    //MPV error
   FitResults[2]         = -0.5;  //Width
   FitResults[3]         =  0;    //Width error
   FitResults[4]         = 0;     //Normalization   
   FitResults[5]         = 0;     //Normalization Error
// cout<<" peak 2"<<endl;
  if( InputHisto->GetEntries() < 20)return;
  
   // perform fit with standard landau
   TF1* MyLandau = new TF1("MyLandau","landau",LowRange, HighRange);
   MyLandau->SetParameter(1,300);
   InputHisto->Fit(MyLandau,"0QR WW");

   // MPV is parameter 1 (0=constant, 1=MPV, 2=Sigma)
   FitResults[0]         = MyLandau->GetParameter(1);  //MPV
   FitResults[1]         = MyLandau->GetParError(1);   //MPV error
   FitResults[2]         = MyLandau->GetParameter(2);  //Width
   FitResults[3]         = MyLandau->GetParError(2);   //Width error
   FitResults[4]         = MyLandau->GetParameter(0); //Norm
   FitResults[5]         = MyLandau->GetParError(0);  //Norm Error

   delete MyLandau;
//    cout<<" end peak"<<endl;

}



void histHolder::link(bool doRefit_, bool doCC_){
  hists.clear();
  hists.push_back(MPV);
  names.push_back("MPV");
  hists.push_back(G2);
  names.push_back("Gain");
  hists.push_back(Error);
  names.push_back("MPV Error");
  hists.push_back(GainDiff);
  names.push_back("Gain Difference");
  if (doRefit_){
    hists.push_back(RefitMPV);
    names.push_back("RefitMPV");
    hists.push_back(RefitWidth);
    names.push_back("RefitWidth");
    hists.push_back(RefitPar0);
    names.push_back("RefitPar0");
    hists.push_back(RefitMPVError);
    names.push_back("RefitMPVError");
    hists.push_back(RefitWidthError);
    names.push_back("RefitWidthError");
    hists.push_back(RefitPar0Error);
    names.push_back("RefitPar0Error");
    hists.push_back(RefitDiff);
    names.push_back("RefitDiff");
  }
  if (doCC_){
    hists.push_back(CC);
    names.push_back("ClusterCharge");
  }
}


cut::cut(string name_,std::function<bool(APV*)>cutFn_):name(name_),cutFn(cutFn_){}

bool cut::pass(APV * a){
  return(cutFn(a));
}

payloadAnalyser::payloadAnalyser(string name_,Gain * g_,TH2 * ClusterCharge_,bool doRefit_,bool doCC_)
  :name(name_),g(g_),ClusterCharge(ClusterCharge_),doRefit(doRefit_),doCC(doCC_)
{
  AddCut("noCut",CutFn::isClean); 
  AddCut("moved",CutFn::moved);
//    AddCut("movedPlus",CutFn::movedPlus);
//    AddCut("movedMinus",CutFn::movedMinus);
  AddCut("was near 300",CutFn::wasNear300); 
//   AddCut("far300",CutFn::far300); 
}

void payloadAnalyser::AddCut(string name_,std::function <bool(APV*)> cutFn_){
  cuts.push_back(cut(name_,cutFn_));
}

void payloadAnalyser::BookHistos(){
  stringstream hName;
  for (unsigned int c=0; c<cuts.size(); c++){
      histHolder * h = new histHolder;
      histos.push_back(h);
      string cutName =cuts.at(c).name;
      
      hName.str("");
      hName<<"MPV"<<"_"<<cutName<<"_"<<name;
      h->MPV=new TH1F(hName.str().c_str(), hName.str().c_str(), 400,0,400);

      hName.str("");
      hName<<"G2"<<"_"<<cutName<<"_"<<name;
      h->G2=new TH1F(hName.str().c_str(), hName.str().c_str(), 1000,0.5,1.5);
      
      hName.str("");
      hName<<"Error"<<"_"<<cutName<<"_"<<name;
      h->Error=new TH1F(hName.str().c_str(), hName.str().c_str(), 200,0,10);
      
      hName.str("");
      hName<<"GainDiff"<<"_"<<cutName<<"_"<<name;
      h->GainDiff=new TH1F(hName.str().c_str(), hName.str().c_str(), 1000,-2,2);
      
      hName.str("");
      hName<<"ReFitMPV"<<"_"<<cutName<<"_"<<name;
      h->RefitMPV = new TH1F(hName.str().c_str(), hName.str().c_str(), 800,0,800);
      
      hName.str("");
      hName<<"ReFitDiff"<<"_"<<cutName<<"_"<<name;
      h->RefitDiff = new TH1F(hName.str().c_str(), hName.str().c_str(),1000,-0.01,0.01);
      
      hName.str("");
      hName<<"ReFitMPVError"<<"_"<<cutName<<"_"<<name;
      h->RefitMPVError = new TH1F(hName.str().c_str(), hName.str().c_str(),1000,0,10);
      
      hName.str("");
      hName<<"ReFitWidth"<<"_"<<cutName<<"_"<<name;
      h->RefitWidth = new TH1F(hName.str().c_str(), hName.str().c_str(),1000,10,50);
      
      hName.str("");
      hName<<"ReFitWidthError"<<"_"<<cutName<<"_"<<name;
      h->RefitWidthError = new TH1F(hName.str().c_str(), hName.str().c_str(),1000,0,5);
      
      hName.str("");
      hName<<"ReFitPar0"<<"_"<<cutName<<"_"<<name;
      h->RefitPar0 = new TH1F(hName.str().c_str(), hName.str().c_str(),1000,0,5000);
      
      hName.str("");
      hName<<"ReFitPar0Error"<<"_"<<cutName<<"_"<<name;
      h->RefitPar0Error = new TH1F(hName.str().c_str(), hName.str().c_str(),10000,0,10);

      h->CC = 0;
      
      h->link(doRefit,doCC);
  }
}

void payloadAnalyser::FillHistos(){
  progressBar p(g->nModules);
  p.start();
  for (int moduleId=0; moduleId<g->nModules; moduleId++){
    p.step(moduleId);
    module * m = g->Get(moduleId);
//     cout<<moduleId<<endl;
    for (int aId = 0; aId<m->nAPV; aId++){
      APV * a =m->getAPVbyId(aId);
//      if (aId/(10*(m->nAPV/10)%10 ==0)){cout<<".";}
     for ( unsigned int c = 0; c<cuts.size(); c++){
       if (!cuts.at(c).pass(a)) continue;
       histHolder * h = histos.at(c);
       h->MPV->Fill(a->MPV);
       h->G2->Fill(a->newGain);
       h->Error->Fill(a->MPVError);
       h->GainDiff->Fill(a->newGain-a->oldGain);
       
       if (!doCC) continue;
       double fitResults[6];
       stringstream hName;
       hName<<"CC_"<<cuts.at(c).name<<"_"<<name<<"_"<<a->absoluteId;
       if (h->CC == 0){
	 TH1F * htemp =(TH1F *) ClusterCharge->ProjectionY(hName.str().c_str(),a->absoluteId+1,a->absoluteId+1)->Clone();
	 h->CC = htemp;
	 if(doRefit){
	  getPeakOfLandau(htemp,fitResults);
  // 	 cout<<"here"<<endl;
	  h->RefitMPV->Fill(fitResults[0]);
	  h->RefitMPVError->Fill(fitResults[1]);
	  h->RefitWidth->Fill(fitResults[2]);
	  h->RefitWidthError->Fill(fitResults[3]);
	  h->RefitPar0->Fill(fitResults[4]);
	  h->RefitPar0Error->Fill(fitResults[5]);
	  h->RefitDiff->Fill(fitResults[0]-a->MPV);
	 }
// 	 cout<<"here3"<<endl;
       }
       else{
// 	 cout<<h<<endl;
	 TH1F * htemp = (TH1F*)ClusterCharge->ProjectionY(hName.str().c_str(),a->absoluteId+1,a->absoluteId+1);
	 h->CC->Add(htemp);
	 if(doRefit){
	  getPeakOfLandau(htemp,fitResults);
	  h->RefitMPV->Fill(fitResults[0]);
	  h->RefitMPVError->Fill(fitResults[1]);
	  h->RefitWidth->Fill(fitResults[2]);
	  h->RefitWidthError->Fill(fitResults[3]);
	  h->RefitPar0->Fill(fitResults[4]);
	  h->RefitPar0Error->Fill(fitResults[5]);
	  h->RefitDiff->Fill(fitResults[0]-a->MPV);
	 }
	 delete htemp;
       }
     }
    }
  }
  p.stop();
}


gainComparator::gainComparator():doRefit(1),doCC(1){
  DefineStyle();
}

void  gainComparator::DisableRefit(){
  doRefit=false;
}
void  gainComparator::DisableCC(){
  doCC=false;
}

gainComparator::~gainComparator(){
  for (unsigned int i=0; i<payloads.size(); i++){
    delete payloads.at(i);
  }
}

void gainComparator::AddPayload(string userName,string fName){
  cout<<"Adding new analyser ("<<userName<<") for file "<<fName<<endl;
  TFile * file = new TFile(fName.c_str());
  TH2 * ClusterCharge =(TH2D*) file->Get("SiStripCalib/Charge_Vs_Index_StdBunch");    
  payloadAnalyser * analyser = new payloadAnalyser(userName,new Gain(fName),ClusterCharge,doRefit,doCC);
  payloads.push_back(analyser);
}

void gainComparator::FillHistos(){
    int s = (int) payloads.size();
    for (int i=0; i<s; i++){
      cout<<"Filling histo "<<i+1<<"/"<<s<<endl;
      payloads.at(i)->FillHistos();
    }
}

void gainComparator::BookHistos(){
    int s = (int) payloads.size();
    for (int i=0; i<s; i++){
      cout<<"Booking histo "<<i+1<<"/"<<s<<endl;
      payloads.at(i)->BookHistos();
    }
}

void gainComparator::DrawHistos(bool norm){
    int nPay   = (int) payloads.size();
    if (nPay == 0) return;
    int nCuts  = (int)payloads.at(0)->histos.size();
    if (nCuts== 0) return;
    int nHists = (int) payloads.at(0)->histos.at(0)->hists.size();
    cout<<"Will plot "<<nHists<<" hitograms from "<<nPay<<" payloads with "<<nCuts<<" selections."<<endl;
    stringstream histName;
    for (int histNumber = 0; histNumber<nHists; histNumber ++){
	TCanvas * c = new TCanvas;
	TLegend * leg = new TLegend(0.7,0.7,0.96,0.96);
	for (int payNumber = 0; payNumber<nPay; payNumber++){
	  for (int cutNumber = 0; cutNumber<nCuts; cutNumber++){
	      payloads.at(payNumber)->histos.at(cutNumber)->link(doRefit,doCC);
	      TH1 * h = payloads.at(payNumber)->histos.at(cutNumber)->hists.at(histNumber);
	      if (h==0) continue;
	      histName.str("");
	      histName<<payloads.at(payNumber)->name<<" "<<payloads.at(payNumber)->cuts.at(cutNumber).name;
	      leg->AddEntry(h,histName.str().c_str(),"lp");
	      h->SetLineColor(colours.at(payNumber));
	      h->SetLineStyle(lineStyle.at(cutNumber));
	      h->SetLineWidth(lineThickness.at(cutNumber));
	      if (norm && h->Integral()>0){
		h->Scale(1./h->Integral());
	      }
	      if (payNumber==0 && cutNumber==0){
		h->SetTitle(payloads.at(0)->histos.at(0)->names.at(histNumber).c_str());
		h->Draw();
	      }
	      else{
		h->Draw("same");
	      }
	  }
	}
	leg->Draw();
    }
}

void gainComparator::DefineStyle(){
    colours.push_back(kBlue);
    colours.push_back(kRed+1);
    colours.push_back(kGreen+2);
    
    lineStyle.push_back(1);
    lineStyle.push_back(2);
    lineStyle.push_back(10);
    lineStyle.push_back(3);
    
    lineThickness.push_back(1);
    lineThickness.push_back(2);
    lineThickness.push_back(2);
    lineThickness.push_back(1);
}