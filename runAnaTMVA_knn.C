{
    gROOT->ProcessLine(".x /home/cvson/rootlogon.C");
    gROOT->ProcessLine(".L ../basicPlotUtil.C");
    
    gROOT->ProcessLine(".L anaTMVA_knn.C");
    
    anaTMVA_knn *pAnalysis = new anaTMVA_knn();
    pAnalysis->Loop(true,"TMVA_cc1pcoh_knn.root");
    cout<<"Processing sucessfully"<<endl;
}