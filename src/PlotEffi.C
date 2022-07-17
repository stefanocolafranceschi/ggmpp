
#include "TF1.h"
#include "TString.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TStyle.h"
#include "TLegend.h"
#include <TH2.h>
#include "TROOT.h"
#include "TApplication.h"
#include <TLorentzVector.h>
#include <string>
#include <iostream>
{
//int main(){
    //gStyle->SetOptStat(111111);
    //gStyle->SetOptFit(0);
        
    Float_t HVPlot[16][100];
    Float_t EffiPlot[16][100];
    Float_t x[100], y[100];
    TString listarun;
    string RootFile;
    int nRun=0;

//    TH1F *myhisto2e = new TH1F("myhisto2e", "GGM raw", 1, 0, 100);
    TH1F *myhisto3e = new TH1F("myhisto3e", "GGM raw", 1, 0, 100);
    TH1F *myhisto4e = new TH1F("myhisto4e", "GGM raw", 1, 0, 100);
    TH1F *myhisto5e = new TH1F("myhisto5e", "GGM raw", 1, 0, 100);
    TH1F *myhisto6e = new TH1F("myhisto6e", "GGM raw", 1, 0, 100);
    TH1F *myhisto7e = new TH1F("myhisto7e", "GGM raw", 1, 0, 100);
    TH1F *myhisto8e = new TH1F("myhisto8e", "GGM raw", 1, 0, 100);
    TH1F *myhisto9e = new TH1F("myhisto9e", "GGM raw", 1, 0, 100);
    TH1F *myhisto2hv = new TH1F("myhisto2hv", "GGM raw", 1, 0, 100);
    TH1F *myhisto3hv = new TH1F("myhisto3hv", "GGM raw", 1, 0, 100);
    TH1F *myhisto4hv = new TH1F("myhisto4hv", "GGM raw", 1, 0, 100);
    TH1F *myhisto5hv = new TH1F("myhisto5hv", "GGM raw", 1, 0, 15000);
    TH1F *myhisto6hv = new TH1F("myhisto6hv", "GGM raw", 1, 0, 15000);
    TH1F *myhisto7hv = new TH1F("myhisto7hv", "GGM raw", 1, 0, 15000);
    TH1F *myhisto8hv = new TH1F("myhisto8hv", "GGM raw", 1, 0, 15000);
    TH1F *myhisto9hv = new TH1F("myhisto9hv", "GGM raw", 1, 0, 15000);

    TF1 *sigmoid = new TF1("sigmoid"," [0]/(1 + exp( [1] * ([2] - x) ) )");
    sigmoid->SetParNames("Efficiency","Slope","HV @ 50 efficiency");
    
    ifstream runlist("/ggmdata/DATA/filelist.txt");
    
    if(!runlist) {      // Check if listarun.txt exists
        cout << "Error opening file filelist.txt" << endl;
        return -1;
    }
    
    while ( !runlist.eof() ) { // Reads the list of ROOTFILES you want to analyze
        runlist >> RootFile;
        
        cout << "\n Analyzing run: " << RootFile << endl;
        TFile *f=new TFile( RootFile.c_str() );
        TTree *GGMraw=(TTree*)f->Get("GGMraw");
TH1F *myhisto2e = new TH1F("myhisto2e", "GGM raw", 1, 0, 100);
        GGMraw->Draw("efficiency6>>myhisto2e");
        EffiPlot[2][nRun] = myhisto2e->GetMean();
        GGMraw->Draw("channel2hv>>myhisto2hv");
        HVPlot[2][nRun] = 10*myhisto2hv->GetMean();
        cout << "CH=2, Efficiency = " << EffiPlot[2][nRun] << "     HV = " << HVPlot[2][nRun] << endl;
return -1;
        GGMraw->Draw("efficiency3>>myhisto3e");
        EffiPlot[3][nRun] = myhisto3e->GetMean();
        GGMraw->Draw("channel3hv>>myhisto3hv");
        HVPlot[3][nRun] = 10*myhisto3hv->GetMean();
        cout << "CH=3, Efficiency = " << EffiPlot[3][nRun] << "     HV = " << HVPlot[3][nRun] << endl;

        GGMraw->Draw("efficiency4>>myhisto4e");
        EffiPlot[4][nRun] = myhisto4e->GetMean();
        GGMraw->Draw("channel4hv>>myhisto4hv");
        HVPlot[4][nRun] = 10*myhisto4hv->GetMean();
        cout << "CH=4, Efficiency = " << EffiPlot[4][nRun] << "     HV = " << HVPlot[4][nRun] << endl;

        GGMraw->Draw("efficiency5>>myhisto5e");
        EffiPlot[5][nRun] = myhisto5e->GetMean();
        GGMraw->Draw("channel5hv>>myhisto5hv");
        HVPlot[5][nRun] = 10*myhisto5hv->GetMean();
        cout << "CH=5, Efficiency = " << EffiPlot[5][nRun] << "     HV = " << HVPlot[5][nRun] << endl;

        GGMraw->Draw("efficiency6>>myhisto6e");
        EffiPlot[6][nRun] = myhisto6e->GetMean();
        GGMraw->Draw("channel6hv>>myhisto6hv");
        HVPlot[6][nRun] = 10*myhisto6hv->GetMean();
        cout << "CH=6, Efficiency = " << EffiPlot[6][nRun] << "     HV = " << HVPlot[6][nRun] << endl;

        GGMraw->Draw("efficiency7>>myhisto7e");
        EffiPlot[7][nRun] = myhisto7e->GetMean();
        GGMraw->Draw("channel7hv>>myhisto7hv");
        HVPlot[7][nRun] = 10*myhisto7hv->GetMean();
        cout << "CH=7, Efficiency = " << EffiPlot[7][nRun] << "     HV = " << HVPlot[7][nRun] << endl;

        GGMraw->Draw("efficiency8>>myhisto8e");
        EffiPlot[8][nRun] = myhisto8e->GetMean();
        GGMraw->Draw("channel8hv>>myhisto8hv");
        HVPlot[8][nRun] = 10*myhisto8hv->GetMean();
        cout << "CH=8, Efficiency = " << EffiPlot[8][nRun] << "     HV = " << HVPlot[8][nRun] << endl;

        GGMraw->Draw("efficiency9>>myhisto9e");
        EffiPlot[9][nRun] = myhisto9e->GetMean();
        GGMraw->Draw("channel9hv>>myhisto9hv");
        HVPlot[9][nRun] = 10*myhisto9hv->GetMean();
        cout << "CH=9, Efficiency = " << EffiPlot[9][nRun] << "     HV = " << HVPlot[9][nRun] << endl;
        nRun++;
    }

    TMultiGraph *AllEfficiecies = new TMultiGraph();
    AllEfficiecies -> SetTitle("Summary plot");

    int color = 1;
    for (int i = 2; i <= 9; i++){
          
        for (int k = 0; k < 100; k++){
            x[k] = HVPlot[i][k];
            y[k] = EffiPlot[i][k];
        }
        auto SingleEfficiency = new TGraph(nRun, x, y);
        SingleEfficiency -> SetTitle("HV scan");
        SingleEfficiency->SetMarkerColor(color);
        color++;
        SingleEfficiency->SetMarkerSize(0.8);
        SingleEfficiency->SetMarkerStyle(20+color);

        AllEfficiecies->Add(SingleEfficiency);

        // Perform the fit
        sigmoid->SetParameters(80.0, 0.01, 9.2);
        SingleEfficiency->Fit("sigmoid","EM");
        
        TAxis *exaxis = SingleEfficiency -> GetXaxis();
        SingleEfficiency ->SetMaximum(10000.0);
        SingleEfficiency ->SetMinimum(8300.0);
        SingleEfficiency -> GetXaxis() ->SetTitle("HV (kV)");
        TAxis *eyaxis = SingleEfficiency -> GetYaxis();
        SingleEfficiency ->SetMaximum(105.0);
        SingleEfficiency ->SetMinimum(0.0);
        SingleEfficiency -> GetYaxis() ->SetTitle("Efficiency %");

        TCanvas *c2 = new TCanvas("c2","GGM Efficiency plots",0,0,700,500);
        c2->SetGrid();
        SingleEfficiency->Draw("ap");
        c2->Modified();
        string OutputFilename = "/ggmdata/DATA/Gap" + std::to_string(i) + ".png";
        c2->Print( OutputFilename.c_str() );
        c2->Update();
    }
    
    // Cumulative Efficiencies plots
    TCanvas *c1 = new TCanvas("c1","GGM Efficiency plots",0,0,700,500);
    c1->SetGrid();
    
    AllEfficiecies -> Draw("ap");
    TAxis *exaxis = AllEfficiecies -> GetXaxis();
    AllEfficiecies -> SetMaximum(10000.0);
    AllEfficiecies -> SetMinimum(8300.0);
    AllEfficiecies -> GetXaxis() ->SetTitle("HV (kV)");
    TAxis *eyaxis = AllEfficiecies -> GetYaxis();
    AllEfficiecies -> SetMaximum(105.0);
    AllEfficiecies -> SetMinimum(0.0);
    AllEfficiecies -> GetYaxis() ->SetTitle("Efficiency %");

    /*
    auto leg = new TLegend(0.2,0.74,0.4,0.85);
    leg->SetHeader(" ");
    */
    
    c1->Modified();
    c1->Print("/ggmdata/DATA/AllEfficiencies.png");
    c1->Update();
}

