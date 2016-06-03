//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Dec  5 15:36:56 2014 by ROOT version 5.34/09
// from TTree TrainTree/TrainTree
// found on file: TMVA_cc1pcoh_dev.root
//////////////////////////////////////////////////////////

#ifndef anaTMVA_h
#define anaTMVA_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class anaTMVA {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           classID;
   Char_t          className[11];
   Float_t         mumucl;
   Float_t         pmucl;
   Float_t         pang;
   Float_t         muang_t;
   Float_t         veract;
   Float_t         fileIndex;
   Float_t         nuE;
   Float_t         inttype;
   Float_t         norm;
   Float_t         totcrsne;
   Float_t         weight;
   Float_t         KNN;

   // List of branches
   TBranch        *b_classID;   //!
   TBranch        *b_className;   //!
   TBranch        *b_mumucl;   //!
   TBranch        *b_pmucl;   //!
   TBranch        *b_pang;   //!
   TBranch        *b_muang_t;   //!
   TBranch        *b_veract;   //!
   TBranch        *b_fileIndex;   //!
   TBranch        *b_nuE;   //!
   TBranch        *b_inttype;   //!
   TBranch        *b_norm;   //!
   TBranch        *b_totcrsne;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_KNN;   //!

   anaTMVA(TTree *tree=0);
   virtual ~anaTMVA();
   virtual Int_t    CutMVA(Long64_t entry, Float_t knnCut);
    virtual Int_t    CutNom(Long64_t entry);

    
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(Bool_t isTrainTree, TString ipFile);
    //virtual double    ReweightEnergy(Int_t mod, Float_t Enu);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef anaTMVA_cxx
anaTMVA::anaTMVA(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("TMVA_cc1pcoh_dev.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("TMVA_cc1pcoh_dev.root");
      }
      f->GetObject("TrainTree",tree);

   }
   Init(tree);
}

anaTMVA::~anaTMVA()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t anaTMVA::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t anaTMVA::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void anaTMVA::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("classID", &classID, &b_classID);
   fChain->SetBranchAddress("className", className, &b_className);
   fChain->SetBranchAddress("mumucl", &mumucl, &b_mumucl);
   fChain->SetBranchAddress("pmucl", &pmucl, &b_pmucl);
   fChain->SetBranchAddress("pang", &pang, &b_pang);
   fChain->SetBranchAddress("muang_t", &muang_t, &b_muang_t);
   fChain->SetBranchAddress("veract", &veract, &b_veract);
   fChain->SetBranchAddress("fileIndex", &fileIndex, &b_fileIndex);
   fChain->SetBranchAddress("nuE", &nuE, &b_nuE);
   fChain->SetBranchAddress("inttype", &inttype, &b_inttype);
   fChain->SetBranchAddress("norm", &norm, &b_norm);
   fChain->SetBranchAddress("totcrsne", &totcrsne, &b_totcrsne);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("KNN", &KNN, &b_KNN);
   Notify();
}

Bool_t anaTMVA::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void anaTMVA::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t anaTMVA::CutMVA(Long64_t entry, Float_t knnCut)
{
    if (KNN>knnCut) {
        return 1;
    }
   else return -1;
}


Int_t anaTMVA::CutNom(Long64_t entry)
{
    if (mumucl>0.6 && pmucl>0.25 && pang<90 && muang_t<15 && veract*7.6634e-2<34) {
        return 1;
    }
    else return -1;
}

/*double anaTMVA::ReweightEnergy(Int_t mod, Float_t Enu)
{
    double wratio;
    int intnue;
    intnue=Enu/NMAX*NBIN;
    if(intnue>=NBIN)intnue=NBIN-1;
    wratio=rwratio[mod][intnue];
    return wratio;
}*/

#endif // #ifdef anaTMVA_cxx
