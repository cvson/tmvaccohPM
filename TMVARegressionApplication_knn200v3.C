/**********************************************************************************
 * Project   : TMVA - a Root-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Exectuable: TMVARegressionApplication                                          *
 *                                                                                *
 * This macro provides a simple example on how to use the trained regression MVAs *
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
#endif

using namespace TMVA;

void TMVARegressionApplication_knn200v2( TString myMethodList = "" )
{
    
    //---------------------------------------------------------------
    // This loads the library
    TMVA::Tools::Instance();
    
    // Default MVA methods to be trained + tested
    std::map<std::string,int> Use;
    
    // --- Mutidimensional likelihood and Nearest-Neighbour methods
    Use["PDERS"]           = 0;
    Use["PDEFoam"]         = 0;
    Use["KNN"]             = 1;
    //
    // --- Linear Discriminant Analysis
    Use["LD"]		        = 0;
    //
    // --- Function Discriminant analysis
    Use["FDA_GA"]          = 0;
    Use["FDA_MC"]          = 0;
    Use["FDA_MT"]          = 0;
    Use["FDA_GAMT"]        = 0;
    //
    // --- Neural Network
    Use["MLP"]             = 0;
    //
    // --- Support Vector Machine
    Use["SVM"]             = 0;
    //
    // --- Boosted Decision Trees
    Use["BDT"]             = 0;
    Use["BDTG"]            = 0;
    // ---------------------------------------------------------------
    
    std::cout << std::endl;
    std::cout << "==> Start TMVARegressionApplication" << std::endl;
    
    // Select methods (don't look at this code - not of interest)
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
    
    // --------------------------------------------------------------------------------------------------
    
    // --- Create the Reader object
    
    TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );
    
    // Create a set of variables and declare them to the reader
    // - the variable names MUST corresponds in name and type to those given in the weight file(s) used
    Float_t range;
    Float_t muang_t, mupe;
    reader->AddVariable( "range", &range );
    reader->AddVariable( "muang_t", &muang_t );
    reader->AddVariable( "mupe", &mupe );
    
    // Spectator variables declared in the training have to be added to the reader, too
    Int_t Ntrack, Ningtrack, inttype, mufc, pene, mupdg;
    Float_t Qsq_true, mumucl;
    
    reader->AddSpectator( "Ntrack",&Ntrack );
    reader->AddSpectator( "Ningtrack", &Ningtrack );
    reader->AddSpectator( "inttype", &inttype);
    reader->AddSpectator( "Qsq_true",  &Qsq_true );
    reader->AddSpectator( "mumucl",  &mumucl);
    reader->AddSpectator( "mufc", &mufc);
    reader->AddSpectator( "pene",  &pene);
    reader->AddSpectator( "mupdg",  &mupdg);
    
    // --- Book the MVA methods
    
    TString dir    = "weights/";
    TString subname = "_knn200v2T2";
    TString prefix = "TMVARegression"+subname;
    
    // Book method(s)
    for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
        if (it->second) {
            TString methodName = it->first + " method";
            TString weightfile = dir + prefix + "_" + TString(it->first) + ".weights.xml";
            reader->BookMVA( methodName, weightfile );
        }
    }
    
    
    
    // Prepare input tree (this must be replaced by your data source)
    // in this example, there is a toy tree with signal and one with background events
    // we'll later on use only the "signal" events for the test in this example.
    //
    
    
    TString fname = "/home/cvson/cc1picoh/frkikawa/meAna/tmva/regression/pmmc_merged_ccqe_bugfix_addtrue_addmon.root";

    std::cout << "--- Selecting data sample" << std::endl;
    TFile *pfile = new TFile(fname,"update");
    TTree* theTree = (TTree*)pfile->Get("tree");
    theTree->SetBranchAddress( "range", &range );
    theTree->SetBranchAddress( "muang_t", &muang_t );
    theTree->SetBranchAddress( "mupe", &mupe );
    theTree->SetBranchAddress( "Ntrack", &Ntrack );
    theTree->SetBranchAddress( "Ningtrack", &Ningtrack );
    theTree->SetBranchAddress( "inttype", &inttype);
    theTree->SetBranchAddress( "Qsq_true", &Qsq_true);
    theTree->SetBranchAddress( "mumucl", &mumucl);
    theTree->SetBranchAddress( "mufc", &mufc);
    theTree->SetBranchAddress( "pene", &pene);
    theTree->SetBranchAddress( "mupdg", &mupdg);
    // --- Event loop
    
    // Prepare the tree
    // - here the variable names have to corresponds to your tree
    // - you can use the same variables as above which is slightly faster,
    //   but of course you can use different ones and copy the values inside the event loop
    //
    Float_t reconMomentum;
    TBranch *breconMomentum = theTree->Branch("reconMomentum",&reconMomentum,"reconMomentum/F");
    
    //------Find the distance for kNN
    if(Use["KNN"]){
        TMVA::kNN::ModulekNN *fModule = new kNN::ModulekNN();
        TMVA::kNN::VarVec vvec(3,0.0);
    }
    //-----------------------------
    Long64_t nentries = theTree->GetEntriesFast();
    Long64_t iprintProcess = Long64_t(nentries/100.);
    std::cout << "--- Processing: " << nentries << " events" << std::endl;
    
    TStopwatch sw;
    sw.Start();
    for (Long64_t ievt=0; ievt<nentries;ievt++) {
    //for (Long64_t ievt=0; ievt<1000;ievt++) {
        if (ievt%iprintProcess == 0) cout<<"Processing "<<int(ievt*100./nentries)<<"% of events"<<endl;
        theTree->GetEntry(ievt);
        
        Float_t val=0;
        if (Use["KNN"]) {
            val = (reader->EvaluateRegression( "KNN method" ))[0];
            cout<<"val "<<val<<endl;
        }
        
        reconMomentum = val;
        breconMomentum->Fill();
        
    }
    theTree->Write();
    sw.Stop();
    std::cout << "--- End of event loop: "; sw.Print();
    
    // --- Write histograms
    
    std::cout << "--- Update root file: \"" << pfile->GetName()
    << "\" containing the MVA output histograms" << std::endl;
    
    delete pfile;
    delete reader;
    
    std::cout << "==> TMVARegressionApplication is done!" << std::endl << std::endl;
}
