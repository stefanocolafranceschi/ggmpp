#include "TInterpreter.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TFile.h"
#include "TH2.h"
#include "TNtuple.h"
#include "TPaveLabel.h"
#include "TPaveText.h"
#include "TFrame.h"
#include "TSystem.h"
#include "TInterpreter.h"

void FitTrial() {
    gStyle->SetOptFit(11111111);
    TH1* h1 = new TH1I("h1", "h1 title", 1095, 0, 4095); 	
    TRandom *r0 = new TRandom();	
    TRandom *r1 = new TRandom();
    int N=5000;
    double x;
    for (int i=0;i<N;i++) {
        x = r0->Landau(900,50);
        h1->Fill(x);
    }
    for (int i=0;i<10000-N;i++) {
        x = r1->Gaus(2000,250);
        h1->Fill(x);
    }
    h1->Draw();

    Double_t par[6];
    TF1 *g1    = new TF1("g1","gaus",1000,4095);
    TF1 *g2    = new TF1("g2","landau",500,1500);
    TF1 *total = new TF1("total","gaus(0)+landau(3)",500,4095);
    total->SetLineColor(2);
    h1->Fit(g1,"R0");
    h1->Fit(g2,"R+0");
    g1->GetParameters(&par[0]);
    g2->GetParameters(&par[3]);
    total->SetParameters(par);
    h1->Fit(total,"R+");
}
