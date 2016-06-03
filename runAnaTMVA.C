{
    gROOT->ProcessLine(".x /home/cvson/rootlogon.C");
    gROOT->ProcessLine(".L ../basicPlotUtil.C");
    
    gROOT->ProcessLine(".L anaTMVA.C");
    
    anaTMVA *pAnalysis = new anaTMVA();
    pAnalysis->Loop(true,"TMVA_cc1pcoh_dev.root");
    cout<<"Processing sucessfully"<<endl;
}