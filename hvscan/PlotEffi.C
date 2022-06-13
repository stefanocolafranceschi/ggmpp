
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
#include <sstream>
#include <iostream>

{
    gStyle->SetOptStat(111111);
    gStyle->SetOptFit(0);
        
    Float_t HVPlot[16][100];
    Float_t EffiPlot[16][100], Charge[16][100];
    Float_t x[100], y[100],ex[100],ey[100];
    TString listarun;
    string RootFile;
    int nRun=0;
    Char_t message[80];

    TF1 *sigmoid = new TF1("sigmoid"," [0]/(1 + exp( [1] * ([2] - x) ) )");
    sigmoid->SetParNames("Efficiency","Slope","HV @ 50 efficiency");
    
    ifstream runlist("filelist.txt");
    
    if(!runlist) {      // Check if listarun.txt exists
        cout << "Error opening file filelist.txt" << endl;
        return -1;
    }
    
    while ( !runlist.eof() ) { // Reads the list of ROOTFILES you want to analyze
        runlist >> RootFile;
        
        cout << "\n Analyzing run: " << RootFile << endl;
        TFile *f=new TFile( RootFile.c_str() );
        TTree *GGMraw=(TTree*)f->Get("GGMraw");

        GGMraw->Draw("efficiency2>>myhisto2e");
        EffiPlot[2][nRun] = myhisto2e->GetMean();
        GGMraw->Draw("channel2hv>>myhisto2hv");
        HVPlot[2][nRun] = 10*myhisto2hv->GetMean();
        cout << "CH=2, Efficiency = " << EffiPlot[2][nRun] << "     HV = " << HVPlot[2][nRun] << endl;
        GGMraw->Draw("charge2>>mycharge2e");
        Charge[2][nRun] = mycharge2e->GetMean();
        cout << "CH=2, Charge = " << Charge[2][nRun] << "     HV = " << HVPlot[2][nRun] << endl;

        GGMraw->Draw("efficiency3>>myhisto3e");
        EffiPlot[3][nRun] = myhisto3e->GetMean();
        GGMraw->Draw("channel3hv>>myhisto3hv");
        HVPlot[3][nRun] = 10*myhisto3hv->GetMean();
        cout << "CH=3, Efficiency = " << EffiPlot[3][nRun] << "     HV = " << HVPlot[3][nRun] << endl;
        GGMraw->Draw("charge3>>mycharge3e");
        Charge[3][nRun] = mycharge3e->GetMean();
        cout << "CH=3, Charge = " << Charge[3][nRun] << "     HV = " << HVPlot[3][nRun] << endl;



        GGMraw->Draw("efficiency4>>myhisto4e");
        EffiPlot[4][nRun] = myhisto4e->GetMean();
        GGMraw->Draw("channel4hv>>myhisto4hv");
        HVPlot[4][nRun] = 10*myhisto4hv->GetMean();
        cout << "CH=4, Efficiency = " << EffiPlot[4][nRun] << "     HV = " << HVPlot[4][nRun] << endl;
        GGMraw->Draw("charge4>>mycharge4e");
        Charge[4][nRun] = mycharge4e->GetMean();
        cout << "CH=4, Charge = " << Charge[4][nRun] << "     HV = " << HVPlot[4][nRun] << endl;


        GGMraw->Draw("efficiency5>>myhisto5e");
        EffiPlot[5][nRun] = myhisto5e->GetMean();
        GGMraw->Draw("channel5hv>>myhisto5hv");
        HVPlot[5][nRun] = 10*myhisto5hv->GetMean();
        cout << "CH=5, Efficiency = " << EffiPlot[5][nRun] << "     HV = " << HVPlot[5][nRun] << endl;
        GGMraw->Draw("charge5>>mycharge5e");
        Charge[5][nRun] = mycharge5e->GetMean();
        cout << "CH=5, Charge = " << Charge[5][nRun] << "     HV = " << HVPlot[5][nRun] << endl;



        GGMraw->Draw("efficiency6>>myhisto6e");
        EffiPlot[6][nRun] = myhisto6e->GetMean();
        GGMraw->Draw("channel6hv>>myhisto6hv");
        HVPlot[6][nRun] = 10*myhisto6hv->GetMean();
        cout << "CH=6, Efficiency = " << EffiPlot[6][nRun] << "     HV = " << HVPlot[6][nRun] << endl;
        GGMraw->Draw("charge6>>mycharge6e");
        Charge[6][nRun] = mycharge6e->GetMean();
        cout << "CH=6, Charge = " << Charge[6][nRun] << "     HV = " << HVPlot[6][nRun] << endl;


        GGMraw->Draw("efficiency7>>myhisto7e");
        EffiPlot[7][nRun] = myhisto7e->GetMean();
        GGMraw->Draw("channel7hv>>myhisto7hv");
        HVPlot[7][nRun] = 10*myhisto7hv->GetMean();
        cout << "CH=7, Efficiency = " << EffiPlot[7][nRun] << "     HV = " << HVPlot[7][nRun] << endl;
        GGMraw->Draw("charge7>>mycharge7e");
        Charge[7][nRun] = mycharge7e->GetMean();
        cout << "CH=7, Charge = " << Charge[7][nRun] << "     HV = " << HVPlot[7][nRun] << endl;


        GGMraw->Draw("efficiency8>>myhisto8e");
        EffiPlot[8][nRun] = myhisto8e->GetMean();
        GGMraw->Draw("channel8hv>>myhisto8hv");
        HVPlot[8][nRun] = 10*myhisto8hv->GetMean();
        cout << "CH=8, Efficiency = " << EffiPlot[8][nRun] << "     HV = " << HVPlot[8][nRun] << endl;
        GGMraw->Draw("charge8>>mycharge8e");
        Charge[8][nRun] = mycharge8e->GetMean();
        cout << "CH=8, Charge = " << Charge[8][nRun] << "     HV = " << HVPlot[8][nRun] << endl;


        GGMraw->Draw("efficiency9>>myhisto9e");
        EffiPlot[9][nRun] = myhisto9e->GetMean();
        GGMraw->Draw("channel9hv>>myhisto9hv");
        HVPlot[9][nRun] = 10*myhisto9hv->GetMean();
        cout << "CH=9, Efficiency = " << EffiPlot[9][nRun] << "     HV = " << HVPlot[9][nRun] << endl;
        GGMraw->Draw("charge9>>mycharge9e");
        Charge[9][nRun] = mycharge9e->GetMean();
        cout << "CH=9, Charge = " << Charge[9][nRun] << "     HV = " << HVPlot[9][nRun] << endl;


        nRun++;
    }

    TMultiGraph *AllEfficiecies = new TMultiGraph();
    AllEfficiecies -> SetTitle("Summary plot");

    int color = 1;
    for (int i = 2; i <= 9; i++){
          
        for (int k = 0; k < 100; k++){
            x[k] = HVPlot[i][k];
            y[k] = EffiPlot[i][k];
            ey[k] = 100 * sqrt( EffiPlot[i][k]/100 * (1.-EffiPlot[i][k]/100)/10000);
            ex[k] = 0.01;
        }
        auto SingleEfficiency = new TGraphErrors(nRun, x, y,ex,ey);
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

        double par[3], errpar[3];
        sigmoid->GetParameters(par);
        errpar[0] = sigmoid->GetParError(0);
        errpar[1] = sigmoid->GetParError(1);
        errpar[2] = sigmoid->GetParError(2);

        TLegend *l = new TLegend(0.15,0.7,0.59,0.89);
        //l->AddEntry(sigmoid,"Fit result", "l");
        sprintf(message,"{Max Efficiency: %.2f #pm %.2f \%}", par[0], errpar[0] );
        l->AddEntry(sigmoid, message);
        sprintf(message,"{Slope: %.3f #pm %.3f}", par[1], errpar[1] );
        l->AddEntry(sigmoid, message);
        sprintf(message,"{HV@50_{\%} efficiency: %.2f #pm %.5f V}", 1000*par[2], errpar[2] );
        l->AddEntry(sigmoid, message);
        l->Draw();

        c2->Modified();

    stringstream temp;
    temp<<i;

        string OutputFilename = "Gap" + temp.str() + ".pdf";
        c2->SaveAs( OutputFilename.c_str() );
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
    c1->SaveAs("AllEfficiencies.pdf");
    c1->Update();
}

