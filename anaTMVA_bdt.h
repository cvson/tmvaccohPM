//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Dec 10 22:06:00 2014 by ROOT version 5.34/09
// from TTree TrainTree/TrainTree
// found on file: TMVA_cc1pcoh_bdt.root
//////////////////////////////////////////////////////////

#ifndef anaTMVA_bdt_h
#define anaTMVA_bdt_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class anaTMVA_bdt {
    public :
    TTree          *fChain;   //!pointer to the analyzed TTree or TChain
    Int_t           fCurrent; //!current Tree number in a TChain
    
    // Declaration of leaf types
    Int_t           classID;
    Char_t          className[11];
    Float_t         mumucl;
    Float_t         pmucl;
    Float_t         pang_t;
    Float_t         muang_t;
    Float_t         veract;
    Float_t         ppe;
    Float_t         mupe;
    Float_t         range;
    Float_t         prange;
    Float_t         fileIndex;
    Float_t         nuE;
    Float_t         inttype;
    Float_t         norm;
    Float_t         totcrsne;
    Float_t         weight;
    Float_t         BDT;
    
    // List of branches
    TBranch        *b_classID;   //!
    TBranch        *b_className;   //!
    TBranch        *b_mumucl;   //!
    TBranch        *b_pmucl;   //!
    TBranch        *b_pang_t;   //!
    TBranch        *b_muang_t;   //!
    TBranch        *b_veract;   //!
    TBranch        *b_ppe;   //!
    TBranch        *b_mupe;   //!
    TBranch        *b_range;   //!
    TBranch        *b_prange;   //!
    TBranch        *b_fileIndex;   //!
    TBranch        *b_nuE;   //!
    TBranch        *b_inttype;   //!
    TBranch        *b_norm;   //!
    TBranch        *b_totcrsne;   //!
    TBranch        *b_weight;   //!
    TBranch        *b_BDT;   //!
    
    anaTMVA_bdt(TTree *tree=0);
    virtual ~anaTMVA_bdt();
    virtual Int_t    CutMVA(Long64_t entry, Float_t bdtCut);
    virtual Int_t    CutMVAlohi(Long64_t entry, Float_t lolimit, Float_t hilimit);
    virtual Int_t    CutNom(Long64_t entry);
    
    virtual Int_t    GetEntry(Long64_t entry);
    virtual Long64_t LoadTree(Long64_t entry);
    virtual void     Init(TTree *tree);
    virtual void     Loop(Bool_t isTrainTree, TString ipFile);
    virtual void     Looplohi(Bool_t isTrainTree, TString ipFile);
    virtual Bool_t   Notify();
    virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef anaTMVA_bdt_cxx
anaTMVA_bdt::anaTMVA_bdt(TTree *tree) : fChain(0)
{
    // if parameter tree is not specified (or zero), connect the file
    // used to generate this class and read the Tree.
    if (tree == 0) {
        TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("TMVA_cc1pcoh_bdt.root");
        if (!f || !f->IsOpen()) {
            f = new TFile("TMVA_cc1pcoh_bdt.root");
        }
        f->GetObject("TrainTree",tree);
        
    }
    Init(tree);
}

anaTMVA_bdt::~anaTMVA_bdt()
{
    if (!fChain) return;
    delete fChain->GetCurrentFile();
}

Int_t anaTMVA_bdt::GetEntry(Long64_t entry)
{
    // Read contents of entry.
    if (!fChain) return 0;
    return fChain->GetEntry(entry);
}
Long64_t anaTMVA_bdt::LoadTree(Long64_t entry)
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

void anaTMVA_bdt::Init(TTree *tree)
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
    fChain->SetBranchAddress("pang_t", &pang_t, &b_pang_t);
    fChain->SetBranchAddress("muang_t", &muang_t, &b_muang_t);
    fChain->SetBranchAddress("veract", &veract, &b_veract);
    fChain->SetBranchAddress("ppe", &ppe, &b_ppe);
    fChain->SetBranchAddress("mupe", &mupe, &b_mupe);
    fChain->SetBranchAddress("range", &range, &b_range);
    fChain->SetBranchAddress("prange", &prange, &b_prange);
    fChain->SetBranchAddress("fileIndex", &fileIndex, &b_fileIndex);
    fChain->SetBranchAddress("nuE", &nuE, &b_nuE);
    fChain->SetBranchAddress("inttype", &inttype, &b_inttype);
    fChain->SetBranchAddress("norm", &norm, &b_norm);
    fChain->SetBranchAddress("totcrsne", &totcrsne, &b_totcrsne);
    fChain->SetBranchAddress("weight", &weight, &b_weight);
    fChain->SetBranchAddress("BDT", &BDT, &b_BDT);
    Notify();
}

Bool_t anaTMVA_bdt::Notify()
{
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.
    
    return kTRUE;
}

void anaTMVA_bdt::Show(Long64_t entry)
{
    // Print contents of entry.
    // If entry is not specified, print current entry
    if (!fChain) return;
    fChain->Show(entry);
}
Int_t anaTMVA_bdt::CutMVA(Long64_t entry, Float_t bdtCut)
{
    if (BDT>bdtCut) {
        return 1;
    }
    else return -1;
}

Int_t anaTMVA_bdt::CutMVAlohi(Long64_t entry, Float_t lolimit, Float_t hilimit)
{
    if (BDT>lolimit && BDT<hilimit) {
        return 1;
    }
    else return -1;
}

Int_t anaTMVA_bdt::CutNom(Long64_t entry)
{
    //if (mumucl>0.6 && pmucl>0.25 && pang<90 && muang_t<15 && veract*7.6634e-2<34) {
    if (mumucl>0.6 && pmucl>0.25 && pang_t<90 && muang_t<15 && veract*7.6634e-2<34) {
        return 1;
    }
    else return -1;
}
#endif // #ifdef anaTMVA_bdt_cxx
