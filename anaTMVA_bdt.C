#define anaTMVA_bdt_cxx
#define NMOD 14
#define NBIN 200
#define NMIN 0
#define NMAX 10
#include "anaTMVA_bdt.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "../basicPlotUtil.h"
void anaTMVA_bdt::Looplohi(Bool_t isTrainTree, TString ipFile)
{
    
    TString subname = "anaTMVA_bdt_ver3_";
    subname += isTrainTree? "train_":"test_";
    Float_t lolimit = -0.04;
    Float_t hilimit = 0.04;
    subname +="bdtlohi_";
    
    Bool_t isEnergyReweightInclude = true;
    subname +=isEnergyReweightInclude?"wEnW_":"woEnW_";
    //GET the weight
    string fratio="/home/kikawa/macros/nd34_tuned_11bv3.1_250ka.root";
    TH1D*  tratio[NMOD];
    TH1D*  tratio2[NMOD];
    double rwratio[NMOD][NBIN];
    fTFRatio   = new TFile(fratio.c_str());
    for(int i=0;i<NMOD;i++){
        char histname[300];
        char histname2[300];
        sprintf(histname,"ing%d_tune_numu",i);
        sprintf(histname2,"ing%d_nom_numu",i);
        tratio[i]= (TH1D*)fTFRatio  ->Get(histname);
        tratio2[i]= (TH1D*)fTFRatio  ->Get(histname2);
        for(int j=0;j<NBIN;j++){
            if(tratio2[i]->GetBinContent(j+1)!=0)
                rwratio[i][j]=(tratio[i]->GetBinContent(j+1))/(tratio2[i]->GetBinContent(j+1));
            else
                rwratio[i][j]=1;
        }
    }
    
    
    //Re initialization ipFile
    TFile *pipfile = new TFile(ipFile);
    TTree *piptree = (TTree*)pipfile->Get(isTrainTree? "TrainTree":"TestTree");
    Init(piptree);
    
    //Histogram to fill
    TH1F* hnuE_all = new TH1F("hnuE_all","",80,0,20);
    TH1F* hnuE_all_sig = new TH1F("hnuE_all_sig","",80,0,20);
    TH1F* hnuE_all_bkg = new TH1F("hnuE_all_bkg","",80,0,20);
    
    TH1F* hnuE_cutmva = new TH1F("hnuE_cutmva","",80,0,20);
    TH1F* hnuE_cutmva_sig = new TH1F("hnuE_cutmva_sig","",80,0,20);
    TH1F* hnuE_cutmva_sig_loonly = new TH1F("hnuE_cutmva_sig_loonly","",80,0,20);
    TH1F* hnuE_cutmva_bkg = new TH1F("hnuE_cutmva_bkg","",80,0,20);
    
    TH1F* hnuE_cutnom = new TH1F("hnuE_cutnom","",80,0,20);
    TH1F* hnuE_cutnom_sig = new TH1F("hnuE_cutnom_sig","",80,0,20);
    TH1F* hnuE_cutnom_bkg = new TH1F("hnuE_cutnom_bkg","",80,0,20);
    
    TH2F* hnuE_vs_bdt = new TH2F("hnuE_vs_bdt","",100,0,20,120,-1.0,0.2);
    TH2F* hnuE_vs_bdt_sig = new TH2F("hnuE_vs_bdt_sig","",100,0,20,120,-1.0,0.2);
    TH2F* hnuE_vs_bdt_bkg = new TH2F("hnuE_vs_bdt_bkg","",100,0,20,120,-1.0,0.2);
    
    Long64_t nentries = fChain->GetEntriesFast();
    Long64_t iprintProcess = Long64_t(nentries/100.);
    
    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
        if (jentry%iprintProcess == 0) cout<<"Processing "<<int(jentry*100./nentries)<<"% of events"<<endl;
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        Float_t totweight = norm*totcrsne*weight*2.8647e-13;
        int intnue=nuE/NMAX*NBIN;
        if(intnue>=NBIN)intnue=NBIN-1;
        if (isEnergyReweightInclude) {
            totweight *=rwratio[3][intnue];
        }
        hnuE_vs_bdt->Fill(nuE,BDT,totweight);
        hnuE_all->Fill(nuE,totweight);
        
        if (classID==0) {
            hnuE_all_sig->Fill(nuE,totweight);
            hnuE_vs_bdt_sig->Fill(nuE,BDT,totweight);
        }
        if (classID==1) {
            hnuE_all_bkg->Fill(nuE,totweight);
            hnuE_vs_bdt_bkg->Fill(nuE,BDT,totweight);
        }
        
        if (CutMVAlohi(ientry,lolimit,hilimit)>0) {
            hnuE_cutmva->Fill(nuE,totweight);
            if (classID==0) hnuE_cutmva_sig->Fill(nuE,totweight);
            if (classID==1) hnuE_cutmva_bkg->Fill(nuE,totweight);
        }
        if (CutMVA(ientry,lolimit)>0) {
            if (classID==0) hnuE_cutmva_sig_loonly->Fill(nuE,totweight);
        }
        if (CutNom(ientry)>0) {
            hnuE_cutnom->Fill(nuE,totweight);
            if (classID==0) hnuE_cutnom_sig->Fill(nuE,totweight);
            if (classID==1) hnuE_cutnom_bkg->Fill(nuE,totweight);
        }
    }//end for
    basicPlotUtil *pPlotUtil = new basicPlotUtil();
    //compare three energy distribution
    hnuE_all_sig->GetXaxis()->SetTitle("True Energy (GeV)");
    hnuE_all_sig->GetYaxis()->SetTitle("Number of events");
    hnuE_all_sig->GetXaxis()->SetRangeUser(0,8);
    pPlotUtil->plot3hist(hnuE_all_sig,"All signal",hnuE_cutmva_sig,"#pi -0.04<PID<0.04", hnuE_cutmva_sig_loonly,"#pi PID > -0.04",subname+"comp_nuE_cutpid",0.5,0.5);
    
    //print efficiency and purity
    Float_t allsignal = hnuE_all_sig->Integral();
    Float_t signal_tmva_sel = hnuE_cutmva_sig->Integral();
    Float_t signal_nom_sel = hnuE_cutnom_sig->Integral();
    
    Float_t allsignal_5gev = hnuE_all_sig->Integral(0,20);
    Float_t signal_tmva_sel_5gev = hnuE_cutmva_sig->Integral(0,20);
    Float_t signal_nom_sel_5gev = hnuE_cutnom_sig->Integral(0,20);
    
    cout<<"Signal all "<<allsignal<<"selected bymva "<<signal_tmva_sel<<" (eff) "<<100*signal_tmva_sel/allsignal<<" bynom "<<signal_nom_sel<<"(eff)"<<100*signal_nom_sel/allsignal<<endl;
    
    cout<<"5GeV Signal all "<<allsignal_5gev<<"selected bymva "<<signal_tmva_sel_5gev<<" (eff) "<<100*signal_tmva_sel_5gev/allsignal_5gev<<" bynom "<<signal_nom_sel_5gev<<"(eff)"<<100*signal_nom_sel_5gev/allsignal_5gev<<endl;
    
    Float_t allbkg = hnuE_all_bkg->Integral();
    Float_t bkg_tmva_sel = hnuE_cutmva_bkg->Integral();
    Float_t bkg_nom_sel = hnuE_cutnom_bkg->Integral();
    
    Float_t allbkg_5gev = hnuE_all_bkg->Integral(0,20);
    Float_t bkg_tmva_sel_5gev = hnuE_cutmva_bkg->Integral(0,20);
    Float_t bkg_nom_sel_5gev = hnuE_cutnom_bkg->Integral(0,20);
    
    cout<<"Background all "<<allbkg<<"selected bymva "<<bkg_tmva_sel<<" (eff) "<<100*bkg_tmva_sel/allbkg<<" bynom "<<bkg_nom_sel<<"(eff)"<<100*bkg_nom_sel/allbkg<<endl;
    
    cout<<"Purity w/ tmva "<<signal_tmva_sel/(signal_tmva_sel+bkg_tmva_sel)<<endl;
    cout<<"Purity w/ nom "<<signal_nom_sel/(signal_nom_sel+bkg_nom_sel)<<endl;
    
    cout<<"5GeV Background all "<<allbkg_5gev<<"selected bymva "<<bkg_tmva_sel_5gev<<" (eff) "<<100*bkg_tmva_sel_5gev/allbkg_5gev<<" bynom "<<bkg_nom_sel_5gev<<"(eff)"<<100*bkg_nom_sel_5gev/allbkg_5gev<<endl;
    
    cout<<"5GeV Purity w/ tmva "<<signal_tmva_sel_5gev/(signal_tmva_sel_5gev+bkg_tmva_sel_5gev)<<endl;
    cout<<"5GeV Purity w/ nom "<<signal_nom_sel_5gev/(signal_nom_sel_5gev+bkg_nom_sel_5gev)<<endl;
    
    Float_t significant_tmva = signal_tmva_sel/sqrt(signal_tmva_sel+bkg_tmva_sel);
    Float_t significant_nom = signal_nom_sel/sqrt(signal_nom_sel+bkg_nom_sel);
    cout<<"Significance tmva "<<significant_tmva<<" normal "<<significant_nom<<endl;
    //plot efficiency and purity
    TH1F* heff_tmva = (TH1F*)hnuE_cutmva_sig->Clone("heff_tmva");
    heff_tmva->Sumw2();
    heff_tmva->Divide(hnuE_all_sig);
    heff_tmva->Scale(100.);
    TH1F* heff_nom = (TH1F*)hnuE_cutnom_sig->Clone("heff_tmva");
    heff_nom->Sumw2();
    heff_nom->Divide(hnuE_all_sig);
    heff_nom->Scale(100.);
    
    heff_tmva->GetXaxis()->SetTitle("True Neutrino Energy (GeV)");
    heff_tmva->GetYaxis()->SetTitle("Signal Efficiency (%)");
    heff_tmva->GetYaxis()->SetRangeUser(0,100);
    heff_tmva->GetXaxis()->SetRangeUser(0,10);
    pPlotUtil->plot2hist(heff_tmva,"TMVA",heff_nom,"Nominal",subname+"comp_eff",0.22,0.6);
    
    TH1F* hpur_tmva = (TH1F*)hnuE_cutmva_sig->Clone("hpur_tmva");
    hpur_tmva->Sumw2();
    hpur_tmva->Divide(hnuE_cutmva);
    hpur_tmva->Scale(100.);
    TH1F* hpur_nom = (TH1F*)hnuE_cutnom_sig->Clone("hpur_tmva");
    hpur_nom->Sumw2();
    hpur_nom->Divide(hnuE_cutnom);
    hpur_nom->Scale(100.);
    hpur_tmva->GetXaxis()->SetTitle("True Neutrino Energy (GeV)");
    hpur_tmva->GetYaxis()->SetTitle("Signal Purity (%)");
    hpur_tmva->GetYaxis()->SetRangeUser(0,100);
    hpur_tmva->GetXaxis()->SetRangeUser(0,10);
    pPlotUtil->plot2hist(hpur_tmva,"TMVA",hpur_nom,"Nominal",subname+"comp_pur",0.52,0.6);
    
    
    
}


void anaTMVA_bdt::Loop(Bool_t isTrainTree, TString ipFile)
{
    
    TString subname = "anaTMVA_bdt_ver3_";
    subname += isTrainTree? "train_":"test_";
    Float_t bdtCut = -0.05;
    subname +="bdtm0dot05_";
    
    Bool_t isEnergyReweightInclude = true;
    subname +=isEnergyReweightInclude?"wEnW_":"woEnW_";
    //GET the weight
    string fratio="/home/kikawa/macros/nd34_tuned_11bv3.1_250ka.root";
    TH1D*  tratio[NMOD];
    TH1D*  tratio2[NMOD];
    double rwratio[NMOD][NBIN];
    fTFRatio   = new TFile(fratio.c_str());
    for(int i=0;i<NMOD;i++){
        char histname[300];
        char histname2[300];
        sprintf(histname,"ing%d_tune_numu",i);
        sprintf(histname2,"ing%d_nom_numu",i);
        tratio[i]= (TH1D*)fTFRatio  ->Get(histname);
        tratio2[i]= (TH1D*)fTFRatio  ->Get(histname2);
        for(int j=0;j<NBIN;j++){
            if(tratio2[i]->GetBinContent(j+1)!=0)
                rwratio[i][j]=(tratio[i]->GetBinContent(j+1))/(tratio2[i]->GetBinContent(j+1));
            else
                rwratio[i][j]=1;
        }
    }
    
    
    //Re initialization ipFile
    TFile *pipfile = new TFile(ipFile);
    TTree *piptree = (TTree*)pipfile->Get(isTrainTree? "TrainTree":"TestTree");
    Init(piptree);
    
    //Histogram to fill
    TH1F* hnuE_all = new TH1F("hnuE_all","",80,0,20);
    TH1F* hnuE_all_sig = new TH1F("hnuE_all_sig","",80,0,20);
    TH1F* hnuE_all_bkg = new TH1F("hnuE_all_bkg","",80,0,20);
    
    TH1F* hnuE_cutmva = new TH1F("hnuE_cutmva","",80,0,20);
    TH1F* hnuE_cutmva_sig = new TH1F("hnuE_cutmva_sig","",80,0,20);
    TH1F* hnuE_cutmva_bkg = new TH1F("hnuE_cutmva_bkg","",80,0,20);
    
    TH1F* hnuE_cutnom = new TH1F("hnuE_cutnom","",80,0,20);
    TH1F* hnuE_cutnom_sig = new TH1F("hnuE_cutnom_sig","",80,0,20);
    TH1F* hnuE_cutnom_bkg = new TH1F("hnuE_cutnom_bkg","",80,0,20);
    
    TH2F* hnuE_vs_bdt = new TH2F("hnuE_vs_bdt","",100,0,20,120,-1.0,0.2);
    TH2F* hnuE_vs_bdt_sig = new TH2F("hnuE_vs_bdt_sig","",100,0,20,120,-1.0,0.2);
    TH2F* hnuE_vs_bdt_bkg = new TH2F("hnuE_vs_bdt_bkg","",100,0,20,120,-1.0,0.2);
    
    Long64_t nentries = fChain->GetEntriesFast();
    Long64_t iprintProcess = Long64_t(nentries/100.);
    
    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
        if (jentry%iprintProcess == 0) cout<<"Processing "<<int(jentry*100./nentries)<<"% of events"<<endl;
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        Float_t totweight = norm*totcrsne*weight*2.8647e-13;
        int intnue=nuE/NMAX*NBIN;
        if(intnue>=NBIN)intnue=NBIN-1;
        if (isEnergyReweightInclude) {
            totweight *=rwratio[3][intnue];
        }
        hnuE_vs_bdt->Fill(nuE,BDT,totweight);
        hnuE_all->Fill(nuE,totweight);
        
        if (classID==0) {
            hnuE_all_sig->Fill(nuE,totweight);
            hnuE_vs_bdt_sig->Fill(nuE,BDT,totweight);
        }
        if (classID==1) {
            hnuE_all_bkg->Fill(nuE,totweight);
            hnuE_vs_bdt_bkg->Fill(nuE,BDT,totweight);
        }
        
        if (CutMVA(ientry,bdtCut)>0) {
            hnuE_cutmva->Fill(nuE,totweight);
            if (classID==0) hnuE_cutmva_sig->Fill(nuE,totweight);
            if (classID==1) hnuE_cutmva_bkg->Fill(nuE,totweight);
        }
        if (CutNom(ientry)>0) {
            hnuE_cutnom->Fill(nuE,totweight);
            if (classID==0) hnuE_cutnom_sig->Fill(nuE,totweight);
            if (classID==1) hnuE_cutnom_bkg->Fill(nuE,totweight);
        }
    }//end for
    //print efficiency and purity
    Float_t allsignal = hnuE_all_sig->Integral();
    Float_t signal_tmva_sel = hnuE_cutmva_sig->Integral();
    Float_t signal_nom_sel = hnuE_cutnom_sig->Integral();
    
    Float_t allsignal_5gev = hnuE_all_sig->Integral(0,20);
    Float_t signal_tmva_sel_5gev = hnuE_cutmva_sig->Integral(0,20);
    Float_t signal_nom_sel_5gev = hnuE_cutnom_sig->Integral(0,20);
    
    cout<<"Signal all "<<allsignal<<"selected bymva "<<signal_tmva_sel<<" (eff) "<<100*signal_tmva_sel/allsignal<<" bynom "<<signal_nom_sel<<"(eff)"<<100*signal_nom_sel/allsignal<<endl;
    
    cout<<"5GeV Signal all "<<allsignal_5gev<<"selected bymva "<<signal_tmva_sel_5gev<<" (eff) "<<100*signal_tmva_sel_5gev/allsignal_5gev<<" bynom "<<signal_nom_sel_5gev<<"(eff)"<<100*signal_nom_sel_5gev/allsignal_5gev<<endl;
    
    Float_t allbkg = hnuE_all_bkg->Integral();
    Float_t bkg_tmva_sel = hnuE_cutmva_bkg->Integral();
    Float_t bkg_nom_sel = hnuE_cutnom_bkg->Integral();
    
    Float_t allbkg_5gev = hnuE_all_bkg->Integral(0,20);
    Float_t bkg_tmva_sel_5gev = hnuE_cutmva_bkg->Integral(0,20);
    Float_t bkg_nom_sel_5gev = hnuE_cutnom_bkg->Integral(0,20);
    
    cout<<"Background all "<<allbkg<<"selected bymva "<<bkg_tmva_sel<<" (eff) "<<100*bkg_tmva_sel/allbkg<<" bynom "<<bkg_nom_sel<<"(eff)"<<100*bkg_nom_sel/allbkg<<endl;
    
    cout<<"Purity w/ tmva "<<signal_tmva_sel/(signal_tmva_sel+bkg_tmva_sel)<<endl;
    cout<<"Purity w/ nom "<<signal_nom_sel/(signal_nom_sel+bkg_nom_sel)<<endl;
    
    cout<<"5GeV Background all "<<allbkg_5gev<<"selected bymva "<<bkg_tmva_sel_5gev<<" (eff) "<<100*bkg_tmva_sel_5gev/allbkg_5gev<<" bynom "<<bkg_nom_sel_5gev<<"(eff)"<<100*bkg_nom_sel_5gev/allbkg_5gev<<endl;
    
    cout<<"5GeV Purity w/ tmva "<<signal_tmva_sel_5gev/(signal_tmva_sel_5gev+bkg_tmva_sel_5gev)<<endl;
    cout<<"5GeV Purity w/ nom "<<signal_nom_sel_5gev/(signal_nom_sel_5gev+bkg_nom_sel_5gev)<<endl;
    
    Float_t significant_tmva = signal_tmva_sel/sqrt(signal_tmva_sel+bkg_tmva_sel);
    Float_t significant_nom = signal_nom_sel/sqrt(signal_nom_sel+bkg_nom_sel);
    cout<<"Significance tmva "<<significant_tmva<<" normal "<<significant_nom<<endl;
    //plot efficiency and purity
    TH1F* heff_tmva = (TH1F*)hnuE_cutmva_sig->Clone("heff_tmva");
    heff_tmva->Sumw2();
    heff_tmva->Divide(hnuE_all_sig);
    heff_tmva->Scale(100.);
    TH1F* heff_nom = (TH1F*)hnuE_cutnom_sig->Clone("heff_tmva");
    heff_nom->Sumw2();
    heff_nom->Divide(hnuE_all_sig);
    heff_nom->Scale(100.);
    basicPlotUtil *pPlotUtil = new basicPlotUtil();
    heff_tmva->GetXaxis()->SetTitle("True Neutrino Energy (GeV)");
    heff_tmva->GetYaxis()->SetTitle("Signal Efficiency (%)");
    heff_tmva->GetYaxis()->SetRangeUser(0,100);
    heff_tmva->GetXaxis()->SetRangeUser(0,10);
    pPlotUtil->plot2hist(heff_tmva,"TMVA",heff_nom,"Nominal",subname+"comp_eff",0.22,0.6);
    
    TH1F* hpur_tmva = (TH1F*)hnuE_cutmva_sig->Clone("hpur_tmva");
    hpur_tmva->Sumw2();
    hpur_tmva->Divide(hnuE_cutmva);
    hpur_tmva->Scale(100.);
    TH1F* hpur_nom = (TH1F*)hnuE_cutnom_sig->Clone("hpur_tmva");
    hpur_nom->Sumw2();
    hpur_nom->Divide(hnuE_cutnom);
    hpur_nom->Scale(100.);
    hpur_tmva->GetXaxis()->SetTitle("True Neutrino Energy (GeV)");
    hpur_tmva->GetYaxis()->SetTitle("Signal Purity (%)");
    hpur_tmva->GetYaxis()->SetRangeUser(0,100);
    hpur_tmva->GetXaxis()->SetRangeUser(0,10);
    pPlotUtil->plot2hist(hpur_tmva,"TMVA",hpur_nom,"Nominal",subname+"comp_pur",0.52,0.6);
    
    //plot correlation between nuE vs BDT
    hnuE_vs_bdt->GetXaxis()->SetTitle("True Neutrino Energy (GeV)");
    hnuE_vs_bdt->GetYaxis()->SetTitle("#pi PID");
    pPlotUtil->plot2dcolz(hnuE_vs_bdt,subname+"hnuE_vs_bdt");
    
    hnuE_vs_bdt_sig->GetXaxis()->SetTitle("True Neutrino Energy (GeV)");
    hnuE_vs_bdt_sig->GetYaxis()->SetTitle("#pi PID");
    pPlotUtil->plot2dcolz(hnuE_vs_bdt_sig,subname+"hnuE_vs_bdt_sig");
    
    hnuE_vs_bdt_bkg->GetXaxis()->SetTitle("True Neutrino Energy (GeV)");
    hnuE_vs_bdt_bkg->GetYaxis()->SetTitle("#pi PID");
    pPlotUtil->plot2dcolz(hnuE_vs_bdt_bkg,subname+"hnuE_vs_bdt_bkg");
}
