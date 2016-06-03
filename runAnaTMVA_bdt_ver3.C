{
    gROOT->ProcessLine(".x /home/cvson/rootlogon.C");
    gROOT->ProcessLine(".L ../basicPlotUtil.C");
    
    gROOT->ProcessLine(".L anaTMVA_bdt.C");
    
    anaTMVA_bdt *pAnalysis = new anaTMVA_bdt();
    pAnalysis->Loop(false,"TMVA_cc1pcoh_bdt_ver3.root");
    //pAnalysis->Looplohi(false,"TMVA_cc1pcoh_bdt_ver3.root");
    cout<<"Processing sucessfully"<<endl;
}