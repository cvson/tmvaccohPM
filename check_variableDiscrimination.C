/***********************************************************************/
//This is to check input discrepancy
/***********************************************************************/
{
    gROOT->ProcessLine(".x /home/cvson/rootlogon.C");
    gROOT->ProcessLine(".L /home/cvson/basicPlotUtil.C");
    
    TString subname = "cc1pcoh_input_2trk_";
    //TFile *pfile = new TFile("TMVA_cc1pcoh_null.root");
    TFile *pfile = new TFile("TMVA_cc1pcoh_null_2trk.root");
    //TFile *pfile = new TFile("TMVA_cc1pcoh_dev.root");
    
    TDirectory* dir = (TDirectory*)pfile->Get( "InputVariables_Id" );
    if (dir==0) {
        cout << "No information about file "<< endl;
        return;
    }
    dir->cd();
    //Get number of variable
    TIter next(dir->GetListOfKeys());
    TKey* key    = 0;
    Int_t noVars = 0;
    
    while ((key = (TKey*)next())) {
        if (key->GetCycle() != 1) continue;
        
        // count number of variables (signal is sufficient)
        if (TString(key->GetName()).Contains("__Signal") || TString(key->GetName()).Contains("__Regression")) noVars++;
    }
    
    cout<<"Number of variable "<<noVars<<endl;
    
    //Loop over all objects in directory
    TKey*    key  = 0;
    Bool_t   createNewFig = kFALSE;
    TIter next(dir->GetListOfKeys());
    while ((key = (TKey*)next())) {
        if (key->GetCycle() != 1) continue;
        
        if (!TString(key->GetName()).Contains("__Signal") ) continue;
        
        // make sure, that we only look at histograms
        TClass *cl = gROOT->GetClass(key->GetClassName());
        if (!cl->InheritsFrom("TH1")) continue;
        TH1 *sig = (TH1*)key->ReadObj();
        TString hname(sig->GetName());
        
        //get background
        TString bgname = hname;
        bgname.ReplaceAll("__Signal","__Background");
        TH1 *bgd = (TH1*)dir->Get(bgname);
        if (bgd == NULL) {
            cout << "ERROR!!! couldn't find background histo for" << hname << endl;
            exit;
        }
        
        TString varname(bgname(0,bgname.Length()-15));
        //check weight and entry
        cout<<"Signal "<<varname<<" entries "<<sig->GetEntries()<<" weight "<<sig->Integral()<<endl;
        cout<<"Background "<<varname<<" entries "<<bgd->GetEntries()<<" weight "<<bgd->Integral()<<endl;
        //plot here
        sig->GetYaxis()->SetTitle("Area Normalization");
        plot2hist(sig,"CC 1#pi coherent",bgd,"Background",subname+varname,0.4,0.6);
        
    }//end while
    
}