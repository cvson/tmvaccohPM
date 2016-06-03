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
#include "TStopwatch.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#endif

using namespace TMVA;

void TMVAClassificationApplication_cc1pcoh_bdt( TString myMethodList = "" )
{   
#ifdef __CINT__
   gROOT->ProcessLine( ".O0" ); // turn off optimization in CINT
#endif

   //---------------------------------------------------------------

   // This loads the library
   TMVA::Tools::Instance();

   // Default MVA methods to be trained + tested
   std::map<std::string,int> Use;
   //
   // --- Boosted Decision Trees
   Use["BDT"]             = 1; // uses Adaptive Boost
   

   std::cout << std::endl;
   std::cout << "==> Start TMVAClassificationApplication" << std::endl;

   // Select methods (don't look at this code - not of interest)
   if (myMethodList != "") {
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

      std::vector<TString> mlist = gTools().SplitString( myMethodList, ',' );
      for (UInt_t i=0; i<mlist.size(); i++) {
         std::string regMethod(mlist[i]);

         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod 
                      << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
            for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
               std::cout << it->first << " ";
            }
            std::cout << std::endl;
            return;
         }
         Use[regMethod] = 1;
      }
   }

   // --------------------------------------------------------------------------------------------------

   // --- Create the Reader object

   TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );    

   // Create a set of variables and declare them to the reader
   // - the variable names MUST corresponds in name and type to those given in the weight file(s) used
   Float_t mumucl, pmucl;
   Float_t pang_t, muang_t;
    Float_t veract;
    Float_t ppe, mupe;
    Float_t range, prange;

    reader->AddVariable( "mumucl", &mumucl );
    reader->AddVariable( "pmucl", &pmucl );
    reader->AddVariable( "pang_t", &pang_t );
    reader->AddVariable( "muang_t", &muang_t );
    reader->AddVariable( "veract", &veract );
    reader->AddVariable( "ppe", &ppe);
    reader->AddVariable( "mupe", &mupe);
    reader->AddVariable( "range", &range);
    reader->AddVariable( "prange", &prange);

   // Spectator variables declared in the training have to be added to the reader, too
   Int_t fileIndex, inttype;
    Float_t nuE, norm, totcrsne;
    reader->AddSpectator( "fileIndex", &fileIndex );
    reader->AddSpectator( "nuE", &nuE );
    reader->AddSpectator( "inttype", &inttype );
    reader->AddSpectator( "norm", &norm );
    reader->AddSpectator( "totcrsne", &totcrsne );

   
   // --- Book the MVA methods

   TString dir    = "weights/";
   TString prefix = "TMVAClassification";

   // Book method(s)
   for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
      if (it->second) {
         TString methodName = TString(it->first) + TString(" method");
         TString weightfile = dir + prefix + TString("_") + TString(it->first) + TString(".weights.xml");
         reader->BookMVA( methodName, weightfile ); 
      }
   }
   
     // Prepare input tree (this must be replaced by your data source)
   // in this example, there is a toy tree with signal and one with background events
   // we'll later on use only the "signal" events for the test in this example.
   //   

    TString fname = "/home/cvson/cc1picoh/frkikawa/meAna/ip4tmva/data_merged_ccqe_addpid.root";
    //TString fname = "/home/cvson/cc1picoh/frkikawa/meAna/ip4tmva/pm_merged_ccqe_tot_addpid.root";
    //TString fname = "/home/cvson/cc1picoh/frkikawa/meAna/ip4tmva/pmbar_merged_ccqe_addpid.root";
    //TString fname = "/home/cvson/cc1picoh/frkikawa/meAna/ip4tmva/ingrid_merged_nd3_ccqe_tot_addpid.root";
    //TString fname = "/home/cvson/cc1picoh/frkikawa/meAna/ip4tmva/wall_merged_ccqe_tot_addpid.root";
    //
    std::cout << "--- Selecting data sample" << std::endl;
    TFile *pfile = new TFile(fname,"update");
    TTree* theTree = (TTree*)pfile->Get("tree");
    theTree->SetBranchAddress( "mumucl", &mumucl );
    theTree->SetBranchAddress( "pmucl", &pmucl );
    theTree->SetBranchAddress( "pang_t", &pang_t );
    theTree->SetBranchAddress( "muang_t", &muang_t );
    theTree->SetBranchAddress( "veract", &veract );
    theTree->SetBranchAddress( "ppe", &ppe);
    theTree->SetBranchAddress( "mupe", &mupe);
    theTree->SetBranchAddress( "range", &range);
    theTree->SetBranchAddress( "prange", &prange);
    
    //theTree->SetBranchAddressr( "nuE", &nuE );
    //theTree->SetBranchAddressr( "inttype", &inttype );
    //theTree->SetBranchAddressr( "norm", &norm );
    //theTree->SetBranchAddressr( "totcrsne", &totcrsne );
    
    Int_t Ntrack;
    theTree->SetBranchAddress( "Ntrack", &Ntrack );
    
    
    Float_t pid;
    TBranch *bpid = theTree->Branch("pid",&pid,"pid/F");
   //Float_t pang;
   //theTree->SetBranchAddress( "pang", &pang );//this missing when training

    //make output
    /*TFile *poutput  = new TFile( "TMVA_Application_data_merged_ccqe.root","RECREATE" );
    TTree *ptree = new TTree("tree","tree for data");
    Float_t mumucl_cp, pmucl_cp;
    Float_t pang_t_cp, muang_t_cp;
    Float_t veract_cp;
    Float_t ppe_cp, mupe_cp;
    Float_t range_cp, prange_cp;
    
    ptree->Branch( "mumucl", &mumucl_cp );
    ptree->Branch( "pmucl", &pmucl_cp );
    ptree->Branch( "pang_t", &pang_t_cp );
    ptree->Branch( "muang_t", &muang_t_cp );
    ptree->Branch( "veract", &veract_cp );
    ptree->Branch( "ppe", &ppe_cp );
    ptree->Branch( "mupe", &mupe_cp );
    ptree->Branch( "range", &range_cp );
    ptree->Branch( "prange", &prange_cp );
    
    // Spectator variables declared in the training have to be added to the reader, too
    Int_t fileIndex_cp, inttype_cp;
    Float_t nuE_cp, norm_cp, totcrsne_cp;
    ptree->Branch( "fileIndex", &fileIndex_cp );
    ptree->Branch( "nuE", &nuE_cp );
    ptree->Branch( "inttype", &inttype_cp );
    ptree->Branch( "norm", &norm_cp );
    ptree->Branch( "totcrsne", &totcrsne_cp );
    
    Float_t pang_cp;
    ptree->Branch( "pang",&pang_cp);*/
    

   std::vector<Float_t> vecVar(9); // vector for EvaluateMVA tests

    Long64_t nentries = theTree->GetEntriesFast();
    Long64_t iprintProcess = Long64_t(nentries/100.);
    
   std::cout << "--- Processing: " << nentries << " events" << std::endl;
    
   TStopwatch sw;
   sw.Start();
   for (Long64_t ievt=0; ievt<nentries;ievt++) {

      //if (ievt%1000 == 0) std::cout << "--- ... Processing event: " << ievt << std::endl;
       if (ievt%iprintProcess == 0) cout<<"Processing "<<int(ievt*100./nentries)<<"% of events"<<endl;
       theTree->GetEntry(ievt);
       Float_t pid_tem;
       if (Use["BDT"])      {
           if (Ntrack!=2) pid_tem = -999;
           else pid_tem = reader->EvaluateMVA("BDT method");
       }
       pid = pid_tem;
       bpid->Fill();

   }
    theTree->Write();
    delete pfile;
   // Get elapsed time
   sw.Stop();
   std::cout << "--- End of event loop: "; sw.Print();

   delete reader;
    
   std::cout << "==> TMVAClassificationApplication is done!" << endl << std::endl;
} 
