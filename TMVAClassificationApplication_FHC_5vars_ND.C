/**********************************************************************************
 * Project   : TMVA - a Root-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Exectuable: TMVAClassificationApplication                                      *
 *                                                                                *
 * This macro provides a simple example on how to use the trained classifiers     *
 * within an analysis module                                                      *
 **********************************************************************************/

#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TPluginManager.h"
#include "TStopwatch.h"

#include "TMVAGui.C"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h" 
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#endif

using namespace TMVA;

void TMVAClassificationApplication_FHC_5vars_ND( TString myMethodList = "" ) 
{   
   //---------------------------------------------------------------
#ifdef __CINT__
	gROOT->ProcessLine( ".O0" ); // turn off optimization in CINT
#endif

   // this loads the library
   TMVA::Tools::Instance();

   std::map<std::string,int> Use;
  


   // Use["KNN80"]         = 1;
    Use["KNN100_wEn"]         = 1;
    //Use["KNN120"]         = 1;
 

 
   std::cout << std::endl;
   std::cout << "==> Start TMVAClassificationApplication" << std::endl;

   if (myMethodList != "") {
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

      std::vector<TString> mlist = gTools().SplitString( myMethodList, ',' );
      for (UInt_t i=0; i<mlist.size(); i++) {
         std::string regMethod(mlist[i]);

         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
            for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
            std::cout << std::endl;
            return;
         }
         Use[regMethod] = 1;
      }
   }

   //========================================================
   // create the Reader object
   // !Color is for colour output/ silent for suppress all output
   TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );    

   // create a set of variables and declare them to the reader
   // - the variable names must corresponds in name and type to 
   // those given in the weight file(s) that you use
	
	Float_t roID, qp, absrelAngle, trknplane, energy;
	reader->AddVariable("roID", &roID);
	reader->AddVariable("qp", &qp);
	reader->AddVariable("absrelAngle", &absrelAngle);
	reader->AddVariable("trknplane", &trknplane);
	reader->AddVariable("energy", &energy);



   //=================================================
   // book the MVA methods
  
   TString dir    = "weights/";
   TString prefix = "TMVAClassification_FHC";//changehere

   // book method(s)
   for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
      if (it->second) {
         TString methodName = it->first + " method";
         TString weightfile = dir + prefix + "_" + TString(it->first) + ".weights.xml";
         reader->BookMVA( methodName, weightfile ); 
      }
   }
   

   

   //===========================


   TFile *input(0);

   const  TString fname = "/minos/app/cvson/Numubar_workdir/kNN/tmva/train/FHC/FHC_near_Dataset_r3.root";
   if (!gSystem->AccessPathName( fname )) {
      input = TFile::Open( fname ); // check if file in local directory exists
   } 
    
   if (!input) {
      std::cout << "ERROR: could not open data file" << std::endl;
      exit(1);
   }
   std::cout << "--- TMVAClassificationApp    : Using input file: " << input->GetName() << std::endl;
   
   //

     TTree* theTree = (TTree*)input->Get("data");

   
	
   std::cout << "--- Select data sample" << std::endl;
	std::cout << "--- Processing: " << theTree->GetEntries() << " events" << std::endl;
	Float_t dpID;
        theTree->SetBranchAddress("dpID", &dpID);//newadd
	
	theTree->SetBranchAddress("trknplane", &trknplane);
    Int_t iaction, inu;
	theTree->SetBranchAddress("iaction", &iaction);
	theTree->SetBranchAddress("inu", &inu);

	
	Float_t  knn01, knn10, knn20, knn40;//changehere
   theTree->SetBranchAddress( "knn01", &knn01 );
   theTree->SetBranchAddress( "knn10", &knn10 );	
   theTree->SetBranchAddress( "knn20", &knn20 );
   theTree->SetBranchAddress( "knn40", &knn40 ); 
   theTree->SetBranchAddress( "roID",  &roID );
	
	Float_t jmTrkPlane, jmMeanPh,jmEndPh, jmScatU, jmScatV, jmID;
    theTree->SetBranchAddress( "jmID",   &jmID );
	theTree->SetBranchAddress("jmTrkPlane",&jmTrkPlane);
	theTree->SetBranchAddress("jmEndPh",&jmEndPh);
	theTree->SetBranchAddress("jmMeanPh",&jmMeanPh);
	theTree->SetBranchAddress("jmScatU",&jmScatU);
	theTree->SetBranchAddress("jmScatV",&jmScatV);
	
	Float_t qp_sigqp, relativeAngle;
	theTree->SetBranchAddress("qp",&qp);
	theTree->SetBranchAddress("qp_sigqp",&qp_sigqp);
	theTree->SetBranchAddress("relativeAngle",&relativeAngle);
	theTree->SetBranchAddress("absrelAngle", &absrelAngle);
	
	Int_t trkIdMC, charge, ntrk, trkfitpass;
	Bool_t isInFidVolCC, isGoodTrackReclamation, isGoodDataQuality,goodBeam, coilIsOk, isLI;
	Float_t rw;	
	Float_t hornCur, dirCosNu;
        Double_t litime;
	
	theTree->SetBranchAddress("trkIdMC", &trkIdMC);
	theTree->SetBranchAddress("charge", &charge);
	theTree->SetBranchAddress("ntrk", &ntrk);
	theTree->SetBranchAddress("trkfitpass", &trkfitpass);
	theTree->SetBranchAddress("isInFidVolCC", &isInFidVolCC);
	theTree->SetBranchAddress("isGoodTrackReclamation", &isGoodTrackReclamation);
	theTree->SetBranchAddress("isGoodDataQuality",&isGoodDataQuality);
	theTree->SetBranchAddress("goodBeam", &goodBeam);
	theTree->SetBranchAddress("coilIsOk", &coilIsOk);
	theTree->SetBranchAddress("isLI", &isLI);
	theTree->SetBranchAddress("rw", &rw);
	theTree->SetBranchAddress("hornCur", &hornCur);
	theTree->SetBranchAddress("dirCosNu", &dirCosNu);
	theTree->SetBranchAddress("litime", &litime);
	
	Float_t trkEn, shwEnkNN, shwEnMC, shwEnLinCCCor, energyMC;
  	theTree->SetBranchAddress("trkEn", &trkEn);
	theTree->SetBranchAddress("shwEnkNN", &shwEnkNN);
	theTree->SetBranchAddress("shwEnMC", &shwEnMC);
	theTree->SetBranchAddress("shwEnLinCCCor", &shwEnLinCCCor);
	theTree->SetBranchAddress("energy",&energy);
	theTree->SetBranchAddress("energyMC",&energyMC);   
  

    std::vector<Float_t> vecVar(5);// vector for EvaluateMVA tests
  
     
 
   //=============================================================

   // write histograms
   

   TFile *target  = new TFile( "TMVA_Application_FHC_wEn_ND_r3.root","RECREATE" );
   TTree *s = new TTree("s","tree for data");

   Float_t dpIDc;
   s->Branch("dpID", &dpIDc);//newadd
   Int_t trknplanec;
   s->Branch("trknplane", &trknplanec);
    Int_t iactionc, inuc;
	s->Branch("iaction",&iactionc);
	s->Branch("inu", &inuc);
	
	Float_t knn01c, knn10c, knn20c, knn40c, roIDc;
	s->Branch("knn01",&knn01c);
	s->Branch("knn10",&knn10c);
	s->Branch("knn20",&knn20c);
	s->Branch("knn40",&knn40c);
	s->Branch("roID",&roIDc);
	
	Float_t jmIDc, jmTrkPlanec, jmEndPhc, jmMeanPhc, jmScatUc, jmScatVc;
	s->Branch("jmID",&jmIDc);
	s->Branch("jmTrkPlane",&jmTrkPlanec);
	s->Branch("jmEndPh",&jmEndPhc);
	s->Branch("jmMeanPh",&jmMeanPhc);
	s->Branch("jmScatU",&jmScatUc);
	s->Branch("jmScatV",&jmScatVc);

	Float_t qpc, qp_sigqpc, relativeAnglec, absrelAnglec;
	s->Branch("qp", &qpc);
	s->Branch("qp_sigqp", &qp_sigqpc);
	s->Branch("relativeAngle",&relativeAnglec);
	s->Branch("absrelAngle", &absrelAnglec);
	
	Int_t trkIdMCc, chargec, ntrkc, trkfitpassc;
	Bool_t isInFidVolCCc, isGoodTrackReclamationc, isGoodDataQualityc,goodBeamc, coilIsOkc, isLIc;
	Float_t rwc;	
	Float_t hornCurc, dirCosNuc;
        Double_t litimec;
	s->Branch("trkIdMC", &trkIdMCc);
	s->Branch("charge", &chargec);
	s->Branch("ntrk", &ntrkc);
	s->Branch("trkfitpass", &trkfitpassc);
	s->Branch("isInFidVolCC", &isInFidVolCCc);
	s->Branch("isGoodTrackReclamation", &isGoodTrackReclamationc);
	s->Branch("isGoodDataQuality",&isGoodDataQualityc);
	s->Branch("goodBeam", &goodBeamc);
	s->Branch("coilIsOk", &coilIsOkc);
	s->Branch("isLI", &isLIc);
	s->Branch("rw", &rwc);
	s->Branch("hornCur", &hornCurc);
	s->Branch("dirCosNu", &dirCosNuc);
	s->Branch("litime", &litimec);
	
	Float_t trkEnc, shwEnkNNc, shwEnMCc, shwEnLinCCCorc, energyc, energyMCc;
  	s->Branch("trkEn", &trkEnc);
	s->Branch("shwEnkNN", &shwEnkNNc);
	s->Branch("shwEnMC", &shwEnMCc);
	s->Branch("shwEnLinCCCor", &shwEnLinCCCorc);
	s->Branch("energy",&energyc);
	s->Branch("energyMC",&energyMCc);
	

   
	Float_t  kid100;//, kid120;
   // s->Branch("kid80",&kid80);
    s->Branch("kid100",&kid100);
    //s->Branch("kid100",&kid120);

     

    //TStopwatch sw;
    //sw.Start();

   for (Long64_t ievt=0; ievt<theTree->GetEntries();ievt++) {

      if (ievt%1000 == 0){
	std::cout << "--- ... Processing event: " << ievt<<" out of "<<theTree->GetEntries() << std::endl;
      }

      theTree->GetEntry(ievt);
      dpIDc = dpID;//newaddd
      trknplanec = trknplane;
	   iactionc = iaction;
	   inuc = inu;
	   
	   knn01c = knn01;
	   knn10c = knn10;
	   knn20c = knn20;
	   knn40c = knn40;
	   roIDc = roID ;
	   
	   jmIDc = jmID;
	   jmTrkPlanec = jmTrkPlane;
	   jmEndPhc = jmEndPh; 
	   jmMeanPhc = jmMeanPh;
	   jmScatUc = jmScatU;
	   jmScatVc = jmScatV;
	   
	   qpc = qp;
	   qp_sigqpc = qp_sigqp; 
	   relativeAnglec = relativeAngle;
	   absrelAnglec = absrelAngle;
	   
	   trkIdMCc = trkIdMC;
	   chargec = charge;
	   ntrkc = ntrk;
	   trkfitpassc = trkfitpass;
	   isInFidVolCCc = isInFidVolCC;
	   isGoodTrackReclamationc = isGoodTrackReclamation; 
	   isGoodDataQualityc = isGoodDataQuality;
	   goodBeamc = goodBeam;
	   coilIsOkc = coilIsOk;
	   isLIc = isLI;
	   rwc = rw;	
	   hornCurc = hornCur;
	   dirCosNuc = dirCosNu;
	   litimec = litime;
	   
	   trkEnc = trkEn;
	   shwEnkNNc = shwEnkNN;
	   shwEnMCc = shwEnMC;
	   shwEnLinCCCorc = shwEnLinCCCor;
	   energyc = energy;
	   energyMCc = energyMC;
	   
	   
	   // if (Use["KNN80"])       kid80 = reader->EvaluateMVA("KNN80 method");
   if (Use["KNN100_wEn"])      kid100 = reader->EvaluateMVA("KNN100_wEn method");
   //if (Use["KNN120"])      kid120 = reader->EvaluateMVA("KNN120 method");
   // sw.Stop();
   //std::cout << "----End of event loop: "; sw.Print();
       s->Fill();
   }
   s->Write();

	

   std::cout << "--- Created root file: \"TMVA_Application_abs.root\" containing the MVA output histograms" << std::endl;
  
   delete reader;
    
   std::cout << "==> TMVAClassificationApplication is done!" << endl << std::endl;
} 
