#include <string.h>
#include <iostream>
#include <TPaveStats.h>
#include <fstream>
#include "CAENVMElib.h"
#include "TF1.h"
#include "TString.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TStyle.h"
#include "TLegend.h"
#include <TH2.h>
#include <TTree.h>
#include <TBranch.h>
#include "TROOT.h"
#include "TApplication.h"
#include "ReadConf.h"
#include "TGaxis.h"

void PrintUsage(void) {
        printf("Usage:\n");
        printf(" -a <name>\n");
        printf(" -c <name>\n");
        exit (8);
}

void PrintHistograms(ReadConf myconfiguration, std::string filel) {
    Float_t StatBoxX1 = 0.8;
    Float_t StatBoxX2 = 0.95;
    Float_t StatBoxY1 = 0.55;
    Float_t StatBoxY2 = 0.78;

    TLegend *legend = new TLegend( StatBoxX1, StatBoxY1, StatBoxX2, StatBoxY2,"GGM");
    Char_t message[80];

    Int_t ChargeMinH = int ( (float)myconfiguration.HistoMinH / 4095 * 900);
    Int_t ChargeMaxH = int ( (float)myconfiguration.HistoMaxH / 4095 * 900);
    Int_t ChargeMinL = int ( (float)myconfiguration.HistoMinL / 4095 * 100);
    Int_t ChargeMaxL = int ( (float)myconfiguration.HistoMaxL / 4095 * 100);

    float label_font = 0.025;
    float axis_offset = 1.30;
    float label_size = 0.05;
    gStyle->SetTitleFontSize(0.05);
    gStyle->SetTextFont(1);

    Double_t Amplitude1, Mean1, Sigma1, AmplitudeErr1, MeanErr1, SigmaErr1, ChiSquareNormalized;
    Double_t Amplitude2, Mean2, Sigma2, AmplitudeErr2, MeanErr2, SigmaErr2;

    TTree* GGMraw;
    TFile *f;
    TPaveStats *st;
    TGaxis *axis;

    Double_t par[6];
    TF1 *g1    = new TF1("g1","gaus",1000,4095);
    TF1 *g2    = new TF1("g2","landau",500,1500);
    TF1 *total = new TF1("total","gaus(0)+landau(3)",500,4095);
    total->SetLineColor(2);

    if (filel.find(".raw") != std::string::npos) {
        int range, event, counter, unixtime, buffer,charge0, charge1,charge2, charge3, charge4, charge5, charge6, charge7, charge8, charge9, charge10, charge11, charge12, charge13, charge14, charge15;

        GGMraw = new TTree("GGMraw","GGMraw");
        GGMraw->Branch("event",&event,"event/I");
        GGMraw->Branch("range",&range,"range/I");
        GGMraw->Branch("timestamp",&unixtime,"unixtime/L");
        GGMraw->Branch("channel0",&charge0,"charge0/I");
        GGMraw->Branch("channel1",&charge1,"charge1/I");
        GGMraw->Branch("channel2",&charge2,"charge2/I");
        GGMraw->Branch("channel3",&charge3,"charge3/I");
        GGMraw->Branch("channel4",&charge4,"charge4/I");
        GGMraw->Branch("channel5",&charge5,"charge5/I");
        GGMraw->Branch("channel6",&charge6,"charge6/I");
        GGMraw->Branch("channel7",&charge7,"charge7/I");
        GGMraw->Branch("channel8",&charge8,"charge8/I");
        GGMraw->Branch("channel9",&charge9,"charge9/I");
        GGMraw->Branch("channel10",&charge10,"charge10/I");
        GGMraw->Branch("channel11",&charge11,"charge11/I");
        GGMraw->Branch("channel12",&charge12,"charge12/I");
        GGMraw->Branch("channel13",&charge13,"charge13/I");
        GGMraw->Branch("channel14",&charge14,"charge14/I");
        GGMraw->Branch("channel15",&charge15,"charge15/I");

        std::ifstream myfile (filel);
        if (myfile.is_open()) {
            range = 1 ;
            while (myfile >> event >> counter >> unixtime >> buffer >> charge0 >> charge1>> charge2 >> charge3 >> charge4 >> charge5 >> charge6 >> charge7 >> charge8 >> charge9 >> charge10 >> charge11 >> charge12 >>  charge13 >> charge14 >> charge15) {
                GGMraw->Fill();
            }
            myfile.close();
        }
        else std::cout << "Unable to open file";          
    }
    else {
        f = TFile::Open(filel.c_str());
        GGMraw = (TTree*) f->Get("GGMraw");
    }

    std::string imagesave;
    gStyle->SetOptStat(111100);           // Stats added (overflow/underflow)
    gStyle->SetOptFit(11111111);
    int hv = 0;
    std::string title;
    
    TCanvas *c1 = new TCanvas("c1");
    c1->SetTopMargin(0.2);
    c1->SetLeftMargin(0.2);
    c1->SetBottomMargin(0.2);
    TH1F *histotemp = new TH1F("histotemp", "GGM raw", 1, 2, 10800);

    TH1F *h0h = new TH1F("h0h", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);
    TH1F *h1h = new TH1F("h1h", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);
    TH1F *h2h = new TH1F("h2h", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);
    TH1F *h3h = new TH1F("h3h", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);
    TH1F *h4h = new TH1F("h4h", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);
    TH1F *h5h = new TH1F("h5h", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);
    TH1F *h6h = new TH1F("h6h", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);
    TH1F *h7h = new TH1F("h7h", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);
    TH1F *h8h = new TH1F("h8h", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);
    TH1F *h9h = new TH1F("h9h", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);
    TH1F *h10h = new TH1F("h10h", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);
    TH1F *h11h = new TH1F("h11h", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);
    TH1F *h12h = new TH1F("h12h", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);
    TH1F *h13h = new TH1F("h13h", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);
    TH1F *h14h = new TH1F("h14h", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);
    TH1F *h15h = new TH1F("h15h", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);
    
    TH1F *h0l = new TH1F("h0l", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);
    TH1F *h1l = new TH1F("h1l", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);
    TH1F *h2l = new TH1F("h2l", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);
    TH1F *h3l = new TH1F("h3l", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);
    TH1F *h4l = new TH1F("h4l", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);
    TH1F *h5l = new TH1F("h5l", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);
    TH1F *h6l = new TH1F("h6l", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);
    TH1F *h7l = new TH1F("h7l", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);

    TH1F *h8l = new TH1F("h8l", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);
    TH1F *h9l = new TH1F("h9l", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);
    TH1F *h10l = new TH1F("h10l", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);
    TH1F *h11l = new TH1F("h11l", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);
    TH1F *h12l = new TH1F("h12l", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);
    TH1F *h13l = new TH1F("h13l", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);
    TH1F *h14l = new TH1F("h14l", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);
    TH1F *h15l = new TH1F("h15l", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);

    GGMraw->Draw("channel0>>h0l","range==1");
    GGMraw->Draw("channel1>>h1l","range==1");
    GGMraw->Draw("channel2>>h2l","range==1");
    GGMraw->Draw("channel3>>h3l","range==1");
    GGMraw->Draw("channel4>>h4l","range==1");
    GGMraw->Draw("channel5>>h5l","range==1");
    GGMraw->Draw("channel6>>h6l","range==1");
    GGMraw->Draw("channel7>>h7l","range==1");

    if ( (myconfiguration.ADCboard) == "965" ) {
        GGMraw->Draw("channel8>>h8l","range==1");
        GGMraw->Draw("channel9>>h9l","range==1");
        GGMraw->Draw("channel10>>h10l","range==1");
        GGMraw->Draw("channel11>>h11l","range==1");
        GGMraw->Draw("channel12>>h12l","range==1");
        GGMraw->Draw("channel13>>h13l","range==1");
        GGMraw->Draw("channel14>>h14l","range==1");
        GGMraw->Draw("channel15>>h15l","range==1");
    }

    GGMraw->Draw("channel0>>h0h","range==0");
    GGMraw->Draw("channel1>>h1h","range==0");
    GGMraw->Draw("channel2>>h2h","range==0");
    GGMraw->Draw("channel3>>h3h","range==0");
    GGMraw->Draw("channel4>>h4h","range==0");
    GGMraw->Draw("channel5>>h5h","range==0");
    GGMraw->Draw("channel6>>h6h","range==0");
    GGMraw->Draw("channel7>>h7h","range==0");

    if ( (myconfiguration.ADCboard) == "965" ) {
        GGMraw->Draw("channel8>>h8h","range==0");
        GGMraw->Draw("channel9>>h9h","range==0");
        GGMraw->Draw("channel10>>h10h","range==0");
        GGMraw->Draw("channel11>>h11h","range==0");
        GGMraw->Draw("channel12>>h12h","range==0");
        GGMraw->Draw("channel13>>h13h","range==0");
        GGMraw->Draw("channel14>>h14h","range==0");
        GGMraw->Draw("channel15>>h15h","range==0");
    }

    if (GGMraw->GetListOfBranches()->Contains("channel0hv")) {
        GGMraw->Draw("channel0hv>>histotemp");
        hv = histotemp->GetMean();
    }

    title = "GGM CH0 high-range ";
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";

    if ( filel.find(".raw") != std::string::npos ) {
        imagesave = filel.replace(filel.find(".raw"), sizeof(".raw") - 1, "_CH0h.pdf");
    }
    else {
        imagesave = filel.replace(filel.find(".root"), sizeof(".root") - 1, "_CH0h.pdf");    
    }

    h0h->SetTitle(title.c_str());
    h0h->GetXaxis()->SetLabelSize(label_size);
    h0h->GetXaxis()->SetTitleOffset(axis_offset);
    h0h->GetXaxis()->SetTitleSize(label_size*1.1);
    h0h->GetXaxis()->SetTitle("Charge <adc>");
    h0h->GetYaxis()->SetLabelSize(label_size);
    h0h->GetYaxis()->SetTitleOffset(axis_offset);
    h0h->GetYaxis()->SetTitleSize(label_size*1.1);
    h0h->GetYaxis()->SetTitle("Events");
    h0h->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinH,ChargeMaxH,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    st = (TPaveStats*)h0h->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel1hv")) {
        GGMraw->Draw("channel1hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH1 high-range ";
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH0h.pdf"), sizeof("_CH0h.pdf") - 1, "_CH1h.pdf");
    h1h->SetTitle(title.c_str());
    h1h->GetXaxis()->SetLabelSize(label_size);
    h1h->GetXaxis()->SetTitleOffset(axis_offset);
    h1h->GetXaxis()->SetTitleSize(label_size*1.1);
    h1h->GetXaxis()->SetTitle("Charge <adc>");
    h1h->GetYaxis()->SetLabelSize(label_size);
    h1h->GetYaxis()->SetTitleOffset(axis_offset);
    h1h->GetYaxis()->SetTitleSize(label_size*1.1);
    h1h->GetYaxis()->SetTitle("Events");
    h1h->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinH,ChargeMaxH,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    st = (TPaveStats*)h1h->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel2hv")) {
        GGMraw->Draw("channel2hv>>histotemp");
        hv = histotemp->GetMean();
    }

    title = "GGM CH2 high-range ";
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH1h.pdf"), sizeof("_CH1h.pdf") - 1, "_CH2h.pdf");
    h2h->SetTitle(title.c_str());
    h2h->GetXaxis()->SetLabelSize(label_size);
    h2h->GetXaxis()->SetTitleOffset(axis_offset);
    h2h->GetXaxis()->SetTitleSize(label_size*1.1);
    h2h->GetXaxis()->SetTitle("Charge <adc>");
    h2h->GetYaxis()->SetLabelSize(label_size);
    h2h->GetYaxis()->SetTitleOffset(axis_offset);
    h2h->GetYaxis()->SetTitleSize(label_size*1.1);
    h2h->GetYaxis()->SetTitle("Events");
    h2h->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinH,ChargeMaxH,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    st = (TPaveStats*)h2h->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel3hv")) {
        GGMraw->Draw("channel3hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH3 high-range ";
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH2h.pdf"), sizeof("_CH2h.pdf") - 1, "_CH3h.pdf");
    h3h->SetTitle(title.c_str());
    h3h->GetXaxis()->SetLabelSize(label_size);
    h3h->GetXaxis()->SetTitleOffset(axis_offset);
    h3h->GetXaxis()->SetTitleSize(label_size*1.1);
    h3h->GetXaxis()->SetTitle("Charge <adc>");
    h3h->GetYaxis()->SetLabelSize(label_size);
    h3h->GetYaxis()->SetTitleOffset(axis_offset);
    h3h->GetYaxis()->SetTitleSize(label_size*1.1);
    h3h->GetYaxis()->SetTitle("Events");
    h3h->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinH,ChargeMaxH,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    st = (TPaveStats*)h3h->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel4hv")) {
        GGMraw->Draw("channel4hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH4 high-range ";
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH3h.pdf"), sizeof("_CH3h.pdf") - 1, "_CH4h.pdf");
    h4h->SetTitle(title.c_str());
    h4h->GetXaxis()->SetLabelSize(label_size);
    h4h->GetXaxis()->SetTitleOffset(axis_offset);
    h4h->GetXaxis()->SetTitleSize(label_size*1.1);
    h4h->GetXaxis()->SetTitle("Charge <adc>");
    h4h->GetYaxis()->SetLabelSize(label_size);
    h4h->GetYaxis()->SetTitleOffset(axis_offset);
    h4h->GetYaxis()->SetTitleSize(label_size*1.1);
    h4h->GetYaxis()->SetTitle("Events");
    h4h->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinH,ChargeMaxH,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    st = (TPaveStats*)h4h->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel5hv")) {
        GGMraw->Draw("channel5hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH5 high-range ";
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH4h.pdf"), sizeof("_CH4h.pdf") - 1, "_CH5h.pdf");
    h5h->SetTitle(title.c_str());
    h5h->GetXaxis()->SetLabelSize(label_size);
    h5h->GetXaxis()->SetTitleOffset(axis_offset);
    h5h->GetXaxis()->SetTitleSize(label_size*1.1);
    h5h->GetXaxis()->SetTitle("Charge <adc>");
    h5h->GetYaxis()->SetLabelSize(label_size);
    h5h->GetYaxis()->SetTitleOffset(axis_offset);
    h5h->GetYaxis()->SetTitleSize(label_size*1.1);
    h5h->GetYaxis()->SetTitle("Events");
    h5h->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinH,ChargeMaxH,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    st = (TPaveStats*)h5h->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel6hv")) {
        GGMraw->Draw("channel6hv>>histotemp");
        hv = histotemp->GetMean();
    }

    title = "GGM CH6 high-range ";
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH5h.pdf"), sizeof("_CH5h.pdf") - 1, "_CH6h.pdf");
    h6h->SetTitle(title.c_str());
    h6h->GetXaxis()->SetLabelSize(label_size);
    h6h->GetXaxis()->SetTitleOffset(axis_offset);
    h6h->GetXaxis()->SetTitleSize(label_size*1.1);
    h6h->GetXaxis()->SetTitle("Charge <adc>");
    h6h->GetYaxis()->SetLabelSize(label_size);
    h6h->GetYaxis()->SetTitleOffset(axis_offset);
    h6h->GetYaxis()->SetTitleSize(label_size*1.1);
    h6h->GetYaxis()->SetTitle("Events");
    h6h->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinH,ChargeMaxH,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    st = (TPaveStats*)h6h->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel7hv")) {
        GGMraw->Draw("channel7hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH7 high-range ";
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH6h.pdf"), sizeof("_CH6h.pdf") - 1, "_CH7h.pdf");
    h7h->SetTitle(title.c_str());
    h7h->GetXaxis()->SetLabelSize(label_size);
    h7h->GetXaxis()->SetTitleOffset(axis_offset);
    h7h->GetXaxis()->SetTitleSize(label_size*1.1);
    h7h->GetXaxis()->SetTitle("Charge <adc>");
    h7h->GetYaxis()->SetLabelSize(label_size);
    h7h->GetYaxis()->SetTitleOffset(axis_offset);
    h7h->GetYaxis()->SetTitleSize(label_size*1.1);
    h7h->GetYaxis()->SetTitle("Events");
    h7h->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinH,ChargeMaxH,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    st = (TPaveStats*)h7h->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

  if ( (myconfiguration.ADCboard) == "965" ) {
    if (GGMraw->GetListOfBranches()->Contains("channel8hv")) {
        GGMraw->Draw("channel8hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH8 high-range ";
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH7h.pdf"), sizeof("_CH7h.pdf") - 1, "_CH8h.pdf");
    h8h->SetTitle(title.c_str());
    h8h->GetXaxis()->SetLabelSize(label_size);
    h8h->GetXaxis()->SetTitleOffset(axis_offset);
    h8h->GetXaxis()->SetTitleSize(label_size*1.1);
    h8h->GetXaxis()->SetTitle("Charge <adc>");
    h8h->GetYaxis()->SetLabelSize(label_size);
    h8h->GetYaxis()->SetTitleOffset(axis_offset);
    h8h->GetYaxis()->SetTitleSize(label_size*1.1);
    h8h->GetYaxis()->SetTitle("Events");
    h8h->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinH,ChargeMaxH,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    st = (TPaveStats*)h8h->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel9hv")) {
        GGMraw->Draw("channel9hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH9 high-range ";
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH8h.pdf"), sizeof("_CH8h.pdf") - 1, "_CH9h.pdf");
    h9h->SetTitle(title.c_str());
    h9h->GetXaxis()->SetLabelSize(label_size);
    h9h->GetXaxis()->SetTitleOffset(axis_offset);
    h9h->GetXaxis()->SetTitleSize(label_size*1.1);
    h9h->GetXaxis()->SetTitle("Charge <adc>");
    h9h->GetYaxis()->SetLabelSize(label_size);
    h9h->GetYaxis()->SetTitleOffset(axis_offset);
    h9h->GetYaxis()->SetTitleSize(label_size*1.1);
    h9h->GetYaxis()->SetTitle("Events");
    h9h->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinH,ChargeMaxH,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    st = (TPaveStats*)h9h->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel10hv")) {
        GGMraw->Draw("channel10hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH10 high-range ";
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH9h.pdf"), sizeof("_CH9h.pdf") - 1, "_CH10h.pdf");
    h10h->SetTitle(title.c_str());
    h10h->GetXaxis()->SetLabelSize(label_size);
    h10h->GetXaxis()->SetTitleOffset(axis_offset);
    h10h->GetXaxis()->SetTitleSize(label_size*1.1);
    h10h->GetXaxis()->SetTitle("Charge <adc>");
    h10h->GetYaxis()->SetLabelSize(label_size);
    h10h->GetYaxis()->SetTitleOffset(axis_offset);
    h10h->GetYaxis()->SetTitleSize(label_size*1.1);
    h10h->GetYaxis()->SetTitle("Events");
    h10h->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinH,ChargeMaxH,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    st = (TPaveStats*)h10h->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel11hv")) {
        GGMraw->Draw("channel11hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH11 high-range ";
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH10h.pdf"), sizeof("_CH10h.pdf") - 1, "_CH11h.pdf");
    h11h->SetTitle(title.c_str());
    h11h->GetXaxis()->SetLabelSize(label_size);
    h11h->GetXaxis()->SetTitleOffset(axis_offset);
    h11h->GetXaxis()->SetTitleSize(label_size*1.1);
    h11h->GetXaxis()->SetTitle("Charge <adc>");
    h11h->GetYaxis()->SetLabelSize(label_size);
    h11h->GetYaxis()->SetTitleOffset(axis_offset);
    h11h->GetYaxis()->SetTitleSize(label_size*1.1);
    h11h->GetYaxis()->SetTitle("Events");
    h11h->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinH,ChargeMaxH,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    st = (TPaveStats*)h11h->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel12hv")) {
        GGMraw->Draw("channel12hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH12 high-range ";
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH11h.pdf"), sizeof("_CH11h.pdf") - 1, "_CH12h.pdf");
    h12h->SetTitle(title.c_str());
    h12h->GetXaxis()->SetLabelSize(label_size);
    h12h->GetXaxis()->SetTitleOffset(axis_offset);
    h12h->GetXaxis()->SetTitleSize(label_size*1.1);
    h12h->GetXaxis()->SetTitle("Charge <adc>");
    h12h->GetYaxis()->SetLabelSize(label_size);
    h12h->GetYaxis()->SetTitleOffset(axis_offset);
    h12h->GetYaxis()->SetTitleSize(label_size*1.1);
    h12h->GetYaxis()->SetTitle("Events");
    h12h->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinH,ChargeMaxH,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    st = (TPaveStats*)h12h->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel13hv")) {
        GGMraw->Draw("channel13hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH13 high-range ";
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH12h.pdf"), sizeof("_CH12h.pdf") - 1, "_CH13h.pdf");
    h13h->SetTitle(title.c_str());
    h13h->GetXaxis()->SetLabelSize(label_size);
    h13h->GetXaxis()->SetTitleOffset(axis_offset);
    h13h->GetXaxis()->SetTitleSize(label_size*1.1);
    h13h->GetXaxis()->SetTitle("Charge <adc>");
    h13h->GetYaxis()->SetLabelSize(label_size);
    h13h->GetYaxis()->SetTitleOffset(axis_offset);
    h13h->GetYaxis()->SetTitleSize(label_size*1.1);
    h13h->GetYaxis()->SetTitle("Events");
    h13h->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinH,ChargeMaxH,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    st = (TPaveStats*)h13h->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel14hv")) {
        GGMraw->Draw("channel14hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH14 high-range ";
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH13h.pdf"), sizeof("_CH13h.pdf") - 1, "_CH14h.pdf");
    h14h->SetTitle(title.c_str());
    h14h->GetXaxis()->SetLabelSize(label_size);
    h14h->GetXaxis()->SetTitleOffset(axis_offset);
    h14h->GetXaxis()->SetTitleSize(label_size*1.1);
    h14h->GetXaxis()->SetTitle("Charge <adc>");
    h14h->GetYaxis()->SetLabelSize(label_size);
    h14h->GetYaxis()->SetTitleOffset(axis_offset);
    h14h->GetYaxis()->SetTitleSize(label_size*1.1);
    h14h->GetYaxis()->SetTitle("Events");
    h14h->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinH,ChargeMaxH,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    st = (TPaveStats*)h14h->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel15hv")) {
        GGMraw->Draw("channel15hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH15 high-range ";
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH14h.pdf"), sizeof("_CH14h.pdf") - 1, "_CH15h.pdf");
    h15h->SetTitle(title.c_str());
    h15h->GetXaxis()->SetLabelSize(label_size);
    h15h->GetXaxis()->SetTitleOffset(axis_offset);
    h15h->GetXaxis()->SetTitleSize(label_size*1.1);
    h15h->GetXaxis()->SetTitle("Charge <adc>");
    h15h->GetYaxis()->SetLabelSize(label_size);
    h15h->GetYaxis()->SetTitleOffset(axis_offset);
    h15h->GetYaxis()->SetTitleSize(label_size*1.1);
    h15h->GetYaxis()->SetTitle("Events");
    h15h->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinH,ChargeMaxH,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    st = (TPaveStats*)h15h->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());
  }
    if (GGMraw->GetListOfBranches()->Contains("channel0hv")) {
        GGMraw->Draw("channel0hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH0 low-range ";
    if (myconfiguration.Fit) {
        h0l->Fit(g1,"R0");
        h0l->Fit(g2,"R+0");
        g1->GetParameters(&par[0]);
        g2->GetParameters(&par[3]);
        total->SetParameters(par);
        h0l->Fit(total,"R+");
        legend->Clear();
        TF1 *fun = h0l->GetFunction("total");
        sprintf(message,"Histo Mean: %.1f", h0l->GetMean() );
        legend->AddEntry(h0l, message);

        sprintf(message,"Histo Std Dev: %.1f", h0l->GetStdDev() );
        legend->AddEntry(h0l, message);

        int underflow = h0l->GetBinContent(0);
        if (underflow<0) underflow=0;
        sprintf(message,"Underflow: %d", underflow );
        legend->AddEntry(h0l, message);

        int overflow = h0l->GetBinContent(myconfiguration.HistoBinningL+1);
        if (overflow<0) overflow=0;
        sprintf(message,"Overflow: %d", overflow );
        legend->AddEntry(h0l, message);

        Amplitude1 = total->GetParameter(0);
        Mean1 = total->GetParameter(1);
        Sigma1 = total->GetParameter(2);
        AmplitudeErr1 = total->GetParError(0);
        MeanErr1 = total->GetParError(1);
        SigmaErr1 = total->GetParError(2);
        Amplitude2 = total->GetParameter(3);
        Mean2 = total->GetParameter(4);
        Sigma2 = total->GetParameter(5);
        AmplitudeErr2 = total->GetParError(3);
        MeanErr2 = total->GetParError(4);
        SigmaErr2 = total->GetParError(5);
        ChiSquareNormalized = total->GetChisquare() / total->GetNDF() ;

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude1, AmplitudeErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean1, MeanErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma1, SigmaErr1 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude2, AmplitudeErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean2, MeanErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma2, SigmaErr2 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit #chi^{2}/NDF = %.1f",total->GetChisquare()/total->GetNDF() );
        legend->AddEntry(fun, message);
    }
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";

    if ( (myconfiguration.ADCboard) == "965" ) {
        imagesave = filel.replace(filel.find("_CH15h.pdf"), sizeof("_CH15h.pdf") - 1, "_CH0l.pdf");
    }
    else {
        imagesave = filel.replace(filel.find("_CH7h.pdf"), sizeof("_CH7h.pdf") - 1, "_CH0l.pdf");
    }
    h0l->SetTitle(title.c_str());
    h0l->GetXaxis()->SetLabelSize(label_size);
    h0l->GetXaxis()->SetTitleOffset(axis_offset);
    h0l->GetXaxis()->SetTitleSize(label_size*1.1);
    h0l->GetXaxis()->SetTitle("Charge <adc>");
    h0l->GetYaxis()->SetLabelSize(label_size);
    h0l->GetYaxis()->SetTitleOffset(axis_offset);
    h0l->GetYaxis()->SetTitleSize(label_size*1.1);
    h0l->GetYaxis()->SetTitle("Events");    
    h0l->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinL,ChargeMaxL,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    if (myconfiguration.Fit) legend->Draw("same");
    st = (TPaveStats*)h0l->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel1hv")) {
        GGMraw->Draw("channel1hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH1 low-range ";

    if (myconfiguration.Fit) {
        h1l->Fit(g1,"R0");
        h1l->Fit(g2,"R+0");
        g1->GetParameters(&par[0]);
        g2->GetParameters(&par[3]);
        total->SetParameters(par);
        h1l->Fit(total,"R+");
        legend->Clear();
        TF1 *fun = h1l->GetFunction("total");
        sprintf(message,"Histo Mean: %.1f", h1l->GetMean() );
        legend->AddEntry(h1l, message);

        sprintf(message,"Histo Std Dev: %.1f", h1l->GetStdDev() );
        legend->AddEntry(h1l, message);

        int underflow = h1l->GetBinContent(0);
        if (underflow<0) underflow=0;
        sprintf(message,"Underflow: %d", underflow );
        legend->AddEntry(h1l, message);

        int overflow = h1l->GetBinContent(myconfiguration.HistoBinningL+1);
        if (overflow<0) overflow=0;
        sprintf(message,"Overflow: %d", overflow );
        legend->AddEntry(h1l, message);

        Amplitude1 = total->GetParameter(0);
        Mean1 = total->GetParameter(1);
        Sigma1 = total->GetParameter(2);
        AmplitudeErr1 = total->GetParError(0);
        MeanErr1 = total->GetParError(1);
        SigmaErr1 = total->GetParError(2);
        Amplitude2 = total->GetParameter(3);
        Mean2 = total->GetParameter(4);
        Sigma2 = total->GetParameter(5);
        AmplitudeErr2 = total->GetParError(3);
        MeanErr2 = total->GetParError(4);
        SigmaErr2 = total->GetParError(5);
        ChiSquareNormalized = total->GetChisquare() / total->GetNDF() ;

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude1, AmplitudeErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean1, MeanErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma1, SigmaErr1 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude2, AmplitudeErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean2, MeanErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma2, SigmaErr2 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit #chi^{2}/NDF = %.1f",total->GetChisquare()/total->GetNDF() );
        legend->AddEntry(fun, message);
    }
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH0l.pdf"), sizeof("_CH0l.pdf") - 1, "_CH1l.pdf");
    h1l->SetTitle(title.c_str());
    h1l->GetXaxis()->SetLabelSize(label_size);
    h1l->GetXaxis()->SetTitleOffset(axis_offset);
    h1l->GetXaxis()->SetTitleSize(label_size*1.1);
    h1l->GetXaxis()->SetTitle("Charge <adc>");
    h1l->GetYaxis()->SetLabelSize(label_size);
    h1l->GetYaxis()->SetTitleOffset(axis_offset);
    h1l->GetYaxis()->SetTitleSize(label_size*1.1);
    h1l->GetYaxis()->SetTitle("Events");
    h1l->Draw();
    if (myconfiguration.Fit) legend->Draw("same");
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinL,ChargeMaxL,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    st = (TPaveStats*)h1l->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel2hv")) {
        GGMraw->Draw("channel2hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH2 low-range ";

    if (myconfiguration.Fit) {
        h2l->Fit(g1,"R0");
        h2l->Fit(g2,"R+0");
        g1->GetParameters(&par[0]);
        g2->GetParameters(&par[3]);
        total->SetParameters(par);
        h2l->Fit(total,"R+");
        legend->Clear();
        TF1 *fun = h2l->GetFunction("total");
        sprintf(message,"Histo Mean: %.1f", h2l->GetMean() );
        legend->AddEntry(h2l, message);

        sprintf(message,"Histo Std Dev: %.1f", h2l->GetStdDev() );
        legend->AddEntry(h2l, message);

        int underflow = h2l->GetBinContent(0);
        if (underflow<0) underflow=0;
        sprintf(message,"Underflow: %d", underflow );
        legend->AddEntry(h2l, message);

        int overflow = h2l->GetBinContent(myconfiguration.HistoBinningL+1);
        if (overflow<0) overflow=0;
        sprintf(message,"Overflow: %d", overflow );
        legend->AddEntry(h2l, message);

        Amplitude1 = total->GetParameter(0);
        Mean1 = total->GetParameter(1);
        Sigma1 = total->GetParameter(2);
        AmplitudeErr1 = total->GetParError(0);
        MeanErr1 = total->GetParError(1);
        SigmaErr1 = total->GetParError(2);
        Amplitude2 = total->GetParameter(3);
        Mean2 = total->GetParameter(4);
        Sigma2 = total->GetParameter(5);
        AmplitudeErr2 = total->GetParError(3);
        MeanErr2 = total->GetParError(4);
        SigmaErr2 = total->GetParError(5);
        ChiSquareNormalized = total->GetChisquare() / total->GetNDF() ;

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude1, AmplitudeErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean1, MeanErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma1, SigmaErr1 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude2, AmplitudeErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean2, MeanErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma2, SigmaErr2 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit #chi^{2}/NDF = %.1f",total->GetChisquare()/total->GetNDF() );
        legend->AddEntry(fun, message);
    }
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH1l.pdf"), sizeof("_CH1l.pdf") - 1, "_CH2l.pdf");
    h2l->SetTitle(title.c_str());
    h2l->GetXaxis()->SetLabelSize(label_size);
    h2l->GetXaxis()->SetTitleOffset(axis_offset);
    h2l->GetXaxis()->SetTitleSize(label_size*1.1);
    h2l->GetXaxis()->SetTitle("Charge <adc>");
    h2l->GetYaxis()->SetLabelSize(label_size);
    h2l->GetYaxis()->SetTitleOffset(axis_offset);
    h2l->GetYaxis()->SetTitleSize(label_size*1.1);
    h2l->GetYaxis()->SetTitle("Events");
    h2l->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinL,ChargeMaxL,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    if (myconfiguration.Fit) legend->Draw("same");
    st = (TPaveStats*)h2l->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel3hv")) {
        GGMraw->Draw("channel3hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH3 low-range ";

    if (myconfiguration.Fit) {
        h3l->Fit(g1,"R0");
        h3l->Fit(g2,"R+0");
        g1->GetParameters(&par[0]);
        g2->GetParameters(&par[3]);
        total->SetParameters(par);
        h3l->Fit(total,"R+");

        legend->Clear();
        TF1 *fun = h3l->GetFunction("total");
        sprintf(message,"Histo Mean: %.1f", h3l->GetMean() );
        legend->AddEntry(h3l, message);

        sprintf(message,"Histo Std Dev: %.1f", h3l->GetStdDev() );
        legend->AddEntry(h3l, message);

        int underflow = h3l->GetBinContent(0);
        if (underflow<0) underflow=0;
        sprintf(message,"Underflow: %d", underflow );
        legend->AddEntry(h3l, message);

        int overflow = h3l->GetBinContent(myconfiguration.HistoBinningL+1);
        if (overflow<0) overflow=0;
        sprintf(message,"Overflow: %d", overflow );
        legend->AddEntry(h3l, message);

        Amplitude1 = total->GetParameter(0);
        Mean1 = total->GetParameter(1);
        Sigma1 = total->GetParameter(2);
        AmplitudeErr1 = total->GetParError(0);
        MeanErr1 = total->GetParError(1);
        SigmaErr1 = total->GetParError(2);
        Amplitude2 = total->GetParameter(3);
        Mean2 = total->GetParameter(4);
        Sigma2 = total->GetParameter(5);
        AmplitudeErr2 = total->GetParError(3);
        MeanErr2 = total->GetParError(4);
        SigmaErr2 = total->GetParError(5);
        ChiSquareNormalized = total->GetChisquare() / total->GetNDF() ;

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude1, AmplitudeErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean1, MeanErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma1, SigmaErr1 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude2, AmplitudeErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean2, MeanErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma2, SigmaErr2 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit #chi^{2}/NDF = %.1f",total->GetChisquare()/total->GetNDF() );
        legend->AddEntry(fun, message);
    }

    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH2l.pdf"), sizeof("_CH2l.pdf") - 1, "_CH3l.pdf");
    h3l->SetTitle(title.c_str());
    h3l->GetXaxis()->SetLabelSize(label_size);
    h3l->GetXaxis()->SetTitleOffset(axis_offset);
    h3l->GetXaxis()->SetTitleSize(label_size*1.1);
    h3l->GetXaxis()->SetTitle("Charge <adc>");
    h3l->GetYaxis()->SetLabelSize(label_size);
    h3l->GetYaxis()->SetTitleOffset(axis_offset);
    h3l->GetYaxis()->SetTitleSize(label_size*1.1);
    h3l->GetYaxis()->SetTitle("Events");
    h3l->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinL,ChargeMaxL,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    if (myconfiguration.Fit) legend->Draw("same");
    st = (TPaveStats*)h3l->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel4hv")) {
        GGMraw->Draw("channel4hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH4 low-range ";

    if (myconfiguration.Fit) {
        h4l->Fit(g1,"R0");
        h4l->Fit(g2,"R+0");
        g1->GetParameters(&par[0]);
        g2->GetParameters(&par[3]);
        total->SetParameters(par);
        h4l->Fit(total,"R+");

        legend->Clear();
        TF1 *fun = h4l->GetFunction("total");
        sprintf(message,"Histo Mean: %.1f", h4l->GetMean() );
        legend->AddEntry(h4l, message);

        sprintf(message,"Histo Std Dev: %.1f", h4l->GetStdDev() );
        legend->AddEntry(h4l, message);

        int underflow = h4l->GetBinContent(0);
        if (underflow<0) underflow=0;
        sprintf(message,"Underflow: %d", underflow );
        legend->AddEntry(h4l, message);

        int overflow = h4l->GetBinContent(myconfiguration.HistoBinningL+1);
        if (overflow<0) overflow=0;
        sprintf(message,"Overflow: %d", overflow );
        legend->AddEntry(h4l, message);

        Amplitude1 = total->GetParameter(0);
        Mean1 = total->GetParameter(1);
        Sigma1 = total->GetParameter(2);
        AmplitudeErr1 = total->GetParError(0);
        MeanErr1 = total->GetParError(1);
        SigmaErr1 = total->GetParError(2);
        Amplitude2 = total->GetParameter(3);
        Mean2 = total->GetParameter(4);
        Sigma2 = total->GetParameter(5);
        AmplitudeErr2 = total->GetParError(3);
        MeanErr2 = total->GetParError(4);
        SigmaErr2 = total->GetParError(5);
        ChiSquareNormalized = total->GetChisquare() / total->GetNDF() ;

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude1, AmplitudeErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean1, MeanErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma1, SigmaErr1 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude2, AmplitudeErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean2, MeanErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma2, SigmaErr2 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit #chi^{2}/NDF = %.1f",total->GetChisquare()/total->GetNDF() );
        legend->AddEntry(fun, message);
    }

    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH3l.pdf"), sizeof("_CH3l.pdf") - 1, "_CH4l.pdf");
    h4l->SetTitle(title.c_str());
    h4l->GetXaxis()->SetLabelSize(label_size);
    h4l->GetXaxis()->SetTitleOffset(axis_offset);
    h4l->GetXaxis()->SetTitleSize(label_size*1.1);
    h4l->GetXaxis()->SetTitle("Charge <adc>");
    h4l->GetYaxis()->SetLabelSize(label_size);
    h4l->GetYaxis()->SetTitleOffset(axis_offset);
    h4l->GetYaxis()->SetTitleSize(label_size*1.1);
    h4l->GetYaxis()->SetTitle("Events");
    h4l->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinL,ChargeMaxL,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    if (myconfiguration.Fit) legend->Draw("same");
    st = (TPaveStats*)h4l->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel5hv")) {
        GGMraw->Draw("channel5hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH5 low-range ";

    if (myconfiguration.Fit) {
        h5l->Fit(g1,"R0");
        h5l->Fit(g2,"R+0");
        g1->GetParameters(&par[0]);
        g2->GetParameters(&par[3]);
        total->SetParameters(par);
        h5l->Fit(total,"R+");

        legend->Clear();
        TF1 *fun = h5l->GetFunction("total");
        sprintf(message,"Histo Mean: %.1f", h5l->GetMean() );
        legend->AddEntry(h5l, message);

        sprintf(message,"Histo Std Dev: %.1f", h5l->GetStdDev() );
        legend->AddEntry(h5l, message);

        int underflow = h5l->GetBinContent(0);
        if (underflow<0) underflow=0;
        sprintf(message,"Underflow: %d", underflow );
        legend->AddEntry(h5l, message);

        int overflow = h5l->GetBinContent(myconfiguration.HistoBinningL+1);
        if (overflow<0) overflow=0;
        sprintf(message,"Overflow: %d", overflow );
        legend->AddEntry(h5l, message);

        Amplitude1 = total->GetParameter(0);
        Mean1 = total->GetParameter(1);
        Sigma1 = total->GetParameter(2);
        AmplitudeErr1 = total->GetParError(0);
        MeanErr1 = total->GetParError(1);
        SigmaErr1 = total->GetParError(2);
        Amplitude2 = total->GetParameter(3);
        Mean2 = total->GetParameter(4);
        Sigma2 = total->GetParameter(5);
        AmplitudeErr2 = total->GetParError(3);
        MeanErr2 = total->GetParError(4);
        SigmaErr2 = total->GetParError(5);
        ChiSquareNormalized = total->GetChisquare() / total->GetNDF() ;

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude1, AmplitudeErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean1, MeanErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma1, SigmaErr1 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude2, AmplitudeErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean2, MeanErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma2, SigmaErr2 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit #chi^{2}/NDF = %.1f",total->GetChisquare()/total->GetNDF() );
        legend->AddEntry(fun, message);
    }

    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH4l.pdf"), sizeof("_CH4l.pdf") - 1, "_CH5l.pdf");
    h5l->SetTitle(title.c_str());
    h5l->GetXaxis()->SetLabelSize(label_size);
    h5l->GetXaxis()->SetTitleOffset(axis_offset);
    h5l->GetXaxis()->SetTitleSize(label_size*1.1);
    h5l->GetXaxis()->SetTitle("Charge <adc>");
    h5l->GetYaxis()->SetLabelSize(label_size);
    h5l->GetYaxis()->SetTitleOffset(axis_offset);
    h5l->GetYaxis()->SetTitleSize(label_size*1.1);
    h5l->GetYaxis()->SetTitle("Events");
    h5l->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinL,ChargeMaxL,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    if (myconfiguration.Fit) legend->Draw("same");
    st = (TPaveStats*)h5l->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel6hv")) {
        GGMraw->Draw("channel6hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH6 low-range ";

    if (myconfiguration.Fit) {
        h6l->Fit(g1,"R0");
        h6l->Fit(g2,"R+0");
        g1->GetParameters(&par[0]);
        g2->GetParameters(&par[3]);
        total->SetParameters(par);
        h6l->Fit(total,"R+");

        legend->Clear();
        TF1 *fun = h6l->GetFunction("total");
        sprintf(message,"Histo Mean: %.1f", h6l->GetMean() );
        legend->AddEntry(h6l, message);

        sprintf(message,"Histo Std Dev: %.1f", h6l->GetStdDev() );
        legend->AddEntry(h6l, message);

        int underflow = h6l->GetBinContent(0);
        if (underflow<0) underflow=0;
        sprintf(message,"Underflow: %d", underflow );
        legend->AddEntry(h6l, message);

        int overflow = h6l->GetBinContent(myconfiguration.HistoBinningL+1);
        if (overflow<0) overflow=0;
        sprintf(message,"Overflow: %d", overflow );
        legend->AddEntry(h6l, message);

        Amplitude1 = total->GetParameter(0);
        Mean1 = total->GetParameter(1);
        Sigma1 = total->GetParameter(2);
        AmplitudeErr1 = total->GetParError(0);
        MeanErr1 = total->GetParError(1);
        SigmaErr1 = total->GetParError(2);
        Amplitude2 = total->GetParameter(3);
        Mean2 = total->GetParameter(4);
        Sigma2 = total->GetParameter(5);
        AmplitudeErr2 = total->GetParError(3);
        MeanErr2 = total->GetParError(4);
        SigmaErr2 = total->GetParError(5);
        ChiSquareNormalized = total->GetChisquare() / total->GetNDF() ;

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude1, AmplitudeErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean1, MeanErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma1, SigmaErr1 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude2, AmplitudeErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean2, MeanErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma2, SigmaErr2 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit #chi^{2}/NDF = %.1f",total->GetChisquare()/total->GetNDF() );
        legend->AddEntry(fun, message);
    }

    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH5l.pdf"), sizeof("_CH5l.pdf") - 1, "_CH6l.pdf");
    h6l->SetTitle(title.c_str());
    h6l->GetXaxis()->SetLabelSize(label_size);
    h6l->GetXaxis()->SetTitleOffset(axis_offset);
    h6l->GetXaxis()->SetTitleSize(label_size*1.1);
    h6l->GetXaxis()->SetTitle("Charge <adc>");
    h6l->GetYaxis()->SetLabelSize(label_size);
    h6l->GetYaxis()->SetTitleOffset(axis_offset);
    h6l->GetYaxis()->SetTitleSize(label_size*1.1);
    h6l->GetYaxis()->SetTitle("Events");
    h6l->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinL,ChargeMaxL,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    if (myconfiguration.Fit) legend->Draw("same");
    st = (TPaveStats*)h6l->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel7hv")) {
        GGMraw->Draw("channel7hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH7 low-range ";

    if (myconfiguration.Fit) {
        h7l->Fit(g1,"R0");
        h7l->Fit(g2,"R+0");
        g1->GetParameters(&par[0]);
        g2->GetParameters(&par[3]);
        total->SetParameters(par);
        h7l->Fit(total,"R+");

        legend->Clear();
        TF1 *fun = h7l->GetFunction("total");
        sprintf(message,"Histo Mean: %.1f", h7l->GetMean() );
        legend->AddEntry(h7l, message);

        sprintf(message,"Histo Std Dev: %.1f", h7l->GetStdDev() );
        legend->AddEntry(h7l, message);

        int underflow = h7l->GetBinContent(0);
        if (underflow<0) underflow=0;
        sprintf(message,"Underflow: %d", underflow );
        legend->AddEntry(h7l, message);

        int overflow = h7l->GetBinContent(myconfiguration.HistoBinningL+1);
        if (overflow<0) overflow=0;
        sprintf(message,"Overflow: %d", overflow );
        legend->AddEntry(h7l, message);

        Amplitude1 = total->GetParameter(0);
        Mean1 = total->GetParameter(1);
        Sigma1 = total->GetParameter(2);
        AmplitudeErr1 = total->GetParError(0);
        MeanErr1 = total->GetParError(1);
        SigmaErr1 = total->GetParError(2);
        Amplitude2 = total->GetParameter(3);
        Mean2 = total->GetParameter(4);
        Sigma2 = total->GetParameter(5);
        AmplitudeErr2 = total->GetParError(3);
        MeanErr2 = total->GetParError(4);
        SigmaErr2 = total->GetParError(5);
        ChiSquareNormalized = total->GetChisquare() / total->GetNDF() ;

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude1, AmplitudeErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean1, MeanErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma1, SigmaErr1 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude2, AmplitudeErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean2, MeanErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma2, SigmaErr2 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit #chi^{2}/NDF = %.1f",total->GetChisquare()/total->GetNDF() );
        legend->AddEntry(fun, message);
    }

    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH6l.pdf"), sizeof("_CH6l.pdf") - 1, "_CH7l.pdf");
    h7l->SetTitle(title.c_str());
    h7l->GetXaxis()->SetLabelSize(label_size);
    h7l->GetXaxis()->SetTitleOffset(axis_offset);
    h7l->GetXaxis()->SetTitleSize(label_size*1.1);
    h7l->GetXaxis()->SetTitle("Charge <adc>");
    h7l->GetYaxis()->SetLabelSize(label_size);
    h7l->GetYaxis()->SetTitleOffset(axis_offset);
    h7l->GetYaxis()->SetTitleSize(label_size*1.1);
    h7l->GetYaxis()->SetTitle("Events");
    h7l->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinL,ChargeMaxL,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    if (myconfiguration.Fit) legend->Draw("same");
    st = (TPaveStats*)h7l->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

  if ( (myconfiguration.ADCboard) == "965" ) {
    if (GGMraw->GetListOfBranches()->Contains("channel8hv")) {
        GGMraw->Draw("channel8hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH8 low-range ";

    if (myconfiguration.Fit) {
        h8l->Fit(g1,"R0");
        h8l->Fit(g2,"R+0");
        g1->GetParameters(&par[0]);
        g2->GetParameters(&par[3]);
        total->SetParameters(par);
        h8l->Fit(total,"R+");

        legend->Clear();
        TF1 *fun = h8l->GetFunction("total");
        sprintf(message,"Histo Mean: %.1f", h8l->GetMean() );
        legend->AddEntry(h8l, message);

        sprintf(message,"Histo Std Dev: %.1f", h8l->GetStdDev() );
        legend->AddEntry(h8l, message);

        int underflow = h8l->GetBinContent(0);
        if (underflow<0) underflow=0;
        sprintf(message,"Underflow: %d", underflow );
        legend->AddEntry(h8l, message);

        int overflow = h8l->GetBinContent(myconfiguration.HistoBinningL+1);
        if (overflow<0) overflow=0;
        sprintf(message,"Overflow: %d", overflow );
        legend->AddEntry(h8l, message);

        Amplitude1 = total->GetParameter(0);
        Mean1 = total->GetParameter(1);
        Sigma1 = total->GetParameter(2);
        AmplitudeErr1 = total->GetParError(0);
        MeanErr1 = total->GetParError(1);
        SigmaErr1 = total->GetParError(2);
        Amplitude2 = total->GetParameter(3);
        Mean2 = total->GetParameter(4);
        Sigma2 = total->GetParameter(5);
        AmplitudeErr2 = total->GetParError(3);
        MeanErr2 = total->GetParError(4);
        SigmaErr2 = total->GetParError(5);
        ChiSquareNormalized = total->GetChisquare() / total->GetNDF() ;

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude1, AmplitudeErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean1, MeanErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma1, SigmaErr1 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude2, AmplitudeErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean2, MeanErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma2, SigmaErr2 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit #chi^{2}/NDF = %.1f",total->GetChisquare()/total->GetNDF() );
        legend->AddEntry(fun, message);
    }

    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH7l.pdf"), sizeof("_CH7l.pdf") - 1, "_CH8l.pdf");
    h8l->SetTitle(title.c_str());
    h8l->GetXaxis()->SetLabelSize(label_size);
    h8l->GetXaxis()->SetTitleOffset(axis_offset);
    h8l->GetXaxis()->SetTitleSize(label_size*1.1);
    h8l->GetXaxis()->SetTitle("Charge <adc>");
    h8l->GetYaxis()->SetLabelSize(label_size);
    h8l->GetYaxis()->SetTitleOffset(axis_offset);
    h8l->GetYaxis()->SetTitleSize(label_size*1.1);
    h8l->GetYaxis()->SetTitle("Events");
    h8l->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinL,ChargeMaxL,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    if (myconfiguration.Fit) legend->Draw("same");
    st = (TPaveStats*)h8l->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel9hv")) {
        GGMraw->Draw("channel9hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH9 low-range ";

    if (myconfiguration.Fit) {
        h9l->Fit(g1,"R0");
        h9l->Fit(g2,"R+0");
        g1->GetParameters(&par[0]);
        g2->GetParameters(&par[3]);
        total->SetParameters(par);
        h9l->Fit(total,"R+");

        legend->Clear();
        TF1 *fun = h9l->GetFunction("total");
        sprintf(message,"Histo Mean: %.1f", h9l->GetMean() );
        legend->AddEntry(h9l, message);

        sprintf(message,"Histo Std Dev: %.1f", h9l->GetStdDev() );
        legend->AddEntry(h9l, message);

        int underflow = h9l->GetBinContent(0);
        if (underflow<0) underflow=0;
        sprintf(message,"Underflow: %d", underflow );
        legend->AddEntry(h9l, message);

        int overflow = h9l->GetBinContent(myconfiguration.HistoBinningL+1);
        if (overflow<0) overflow=0;
        sprintf(message,"Overflow: %d", overflow );
        legend->AddEntry(h9l, message);

        Amplitude1 = total->GetParameter(0);
        Mean1 = total->GetParameter(1);
        Sigma1 = total->GetParameter(2);
        AmplitudeErr1 = total->GetParError(0);
        MeanErr1 = total->GetParError(1);
        SigmaErr1 = total->GetParError(2);
        Amplitude2 = total->GetParameter(3);
        Mean2 = total->GetParameter(4);
        Sigma2 = total->GetParameter(5);
        AmplitudeErr2 = total->GetParError(3);
        MeanErr2 = total->GetParError(4);
        SigmaErr2 = total->GetParError(5);
        ChiSquareNormalized = total->GetChisquare() / total->GetNDF() ;

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude1, AmplitudeErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean1, MeanErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma1, SigmaErr1 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude2, AmplitudeErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean2, MeanErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma2, SigmaErr2 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit #chi^{2}/NDF = %.1f",total->GetChisquare()/total->GetNDF() );
        legend->AddEntry(fun, message);
    }

    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH8l.pdf"), sizeof("_CH8l.pdf") - 1, "_CH9l.pdf");
    h9l->SetTitle(title.c_str());
    h9l->GetXaxis()->SetLabelSize(label_size);
    h9l->GetXaxis()->SetTitleOffset(axis_offset);
    h9l->GetXaxis()->SetTitleSize(label_size*1.1);
    h9l->GetXaxis()->SetTitle("Charge <adc>");
    h9l->GetYaxis()->SetLabelSize(label_size);
    h9l->GetYaxis()->SetTitleOffset(axis_offset);
    h9l->GetYaxis()->SetTitleSize(label_size*1.1);
    h9l->GetYaxis()->SetTitle("Events");
    h9l->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinL,ChargeMaxL,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    if (myconfiguration.Fit) legend->Draw("same");
    st = (TPaveStats*)h9l->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel10hv")) {
        GGMraw->Draw("channel10hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH10 low-range ";

    if (myconfiguration.Fit) {
        h10l->Fit(g1,"R0");
        h10l->Fit(g2,"R+0");
        g1->GetParameters(&par[0]);
        g2->GetParameters(&par[3]);
        total->SetParameters(par);
        h10l->Fit(total,"R+");

        legend->Clear();
        TF1 *fun = h10l->GetFunction("total");
        sprintf(message,"Histo Mean: %.1f", h10l->GetMean() );
        legend->AddEntry(h10l, message);

        sprintf(message,"Histo Std Dev: %.1f", h10l->GetStdDev() );
        legend->AddEntry(h10l, message);

        int underflow = h10l->GetBinContent(0);
        if (underflow<0) underflow=0;
        sprintf(message,"Underflow: %d", underflow );
        legend->AddEntry(h10l, message);

        int overflow = h10l->GetBinContent(myconfiguration.HistoBinningL+1);
        if (overflow<0) overflow=0;
        sprintf(message,"Overflow: %d", overflow );
        legend->AddEntry(h10l, message);

        Amplitude1 = total->GetParameter(0);
        Mean1 = total->GetParameter(1);
        Sigma1 = total->GetParameter(2);
        AmplitudeErr1 = total->GetParError(0);
        MeanErr1 = total->GetParError(1);
        SigmaErr1 = total->GetParError(2);
        Amplitude2 = total->GetParameter(3);
        Mean2 = total->GetParameter(4);
        Sigma2 = total->GetParameter(5);
        AmplitudeErr2 = total->GetParError(3);
        MeanErr2 = total->GetParError(4);
        SigmaErr2 = total->GetParError(5);
        ChiSquareNormalized = total->GetChisquare() / total->GetNDF() ;

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude1, AmplitudeErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean1, MeanErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma1, SigmaErr1 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude2, AmplitudeErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean2, MeanErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma2, SigmaErr2 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit #chi^{2}/NDF = %.1f",total->GetChisquare()/total->GetNDF() );
        legend->AddEntry(fun, message);
    }
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH9l.pdf"), sizeof("_CH9l.pdf") - 1, "_CH10l.pdf");
    h10l->SetTitle(title.c_str());
    h10l->GetXaxis()->SetLabelSize(label_size);
    h10l->GetXaxis()->SetTitleOffset(axis_offset);
    h10l->GetXaxis()->SetTitleSize(label_size*1.1);
    h10l->GetXaxis()->SetTitle("Charge <adc>");
    h10l->GetYaxis()->SetLabelSize(label_size);
    h10l->GetYaxis()->SetTitleOffset(axis_offset);
    h10l->GetYaxis()->SetTitleSize(label_size*1.1);
    h10l->GetYaxis()->SetTitle("Events");
    h10l->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinL,ChargeMaxL,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    if (myconfiguration.Fit) legend->Draw("same");
    st = (TPaveStats*)h10l->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel11hv")) {
        GGMraw->Draw("channel11hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH11 low-range ";

    if (myconfiguration.Fit) {
        h11l->Fit(g1,"R0");
        h11l->Fit(g2,"R+0");
        g1->GetParameters(&par[0]);
        g2->GetParameters(&par[3]);
        total->SetParameters(par);
        h11l->Fit(total,"R+");

        legend->Clear();
        TF1 *fun = h11l->GetFunction("total");
        sprintf(message,"Histo Mean: %.1f", h11l->GetMean() );
        legend->AddEntry(h11l, message);

        sprintf(message,"Histo Std Dev: %.1f", h11l->GetStdDev() );
        legend->AddEntry(h11l, message);

        int underflow = h11l->GetBinContent(0);
        if (underflow<0) underflow=0;
        sprintf(message,"Underflow: %d", underflow );
        legend->AddEntry(h11l, message);

        int overflow = h11l->GetBinContent(myconfiguration.HistoBinningL+1);
        if (overflow<0) overflow=0;
        sprintf(message,"Overflow: %d", overflow );
        legend->AddEntry(h11l, message);

        Amplitude1 = total->GetParameter(0);
        Mean1 = total->GetParameter(1);
        Sigma1 = total->GetParameter(2);
        AmplitudeErr1 = total->GetParError(0);
        MeanErr1 = total->GetParError(1);
        SigmaErr1 = total->GetParError(2);
        Amplitude2 = total->GetParameter(3);
        Mean2 = total->GetParameter(4);
        Sigma2 = total->GetParameter(5);
        AmplitudeErr2 = total->GetParError(3);
        MeanErr2 = total->GetParError(4);
        SigmaErr2 = total->GetParError(5);
        ChiSquareNormalized = total->GetChisquare() / total->GetNDF() ;

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude1, AmplitudeErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean1, MeanErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma1, SigmaErr1 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude2, AmplitudeErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean2, MeanErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma2, SigmaErr2 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit #chi^{2}/NDF = %.1f",total->GetChisquare()/total->GetNDF() );
        legend->AddEntry(fun, message);
    }
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH10l.pdf"), sizeof("_CH10l.pdf") - 1, "_CH11l.pdf");
    h11l->SetTitle(title.c_str());
    h11l->GetXaxis()->SetLabelSize(label_size);
    h11l->GetXaxis()->SetTitleOffset(axis_offset);
    h11l->GetXaxis()->SetTitleSize(label_size*1.1);
    h11l->GetXaxis()->SetTitle("Charge <adc>");
    h11l->GetYaxis()->SetLabelSize(label_size);
    h11l->GetYaxis()->SetTitleOffset(axis_offset);
    h11l->GetYaxis()->SetTitleSize(label_size*1.1);
    h11l->GetYaxis()->SetTitle("Events");
    h11l->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinL,ChargeMaxL,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    if (myconfiguration.Fit) legend->Draw("same");
    st = (TPaveStats*)h11l->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel12hv")) {
        GGMraw->Draw("channel12hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH12 low-range ";

    if (myconfiguration.Fit) {
        h12l->Fit(g1,"R0");
        h12l->Fit(g2,"R+0");
        g1->GetParameters(&par[0]);
        g2->GetParameters(&par[3]);
        total->SetParameters(par);
        h12l->Fit(total,"R+");

        legend->Clear();
        TF1 *fun = h12l->GetFunction("total");
        sprintf(message,"Histo Mean: %.1f", h12l->GetMean() );
        legend->AddEntry(h12l, message);

        sprintf(message,"Histo Std Dev: %.1f", h12l->GetStdDev() );
        legend->AddEntry(h12l, message);

        int underflow = h12l->GetBinContent(0);
        if (underflow<0) underflow=0;
        sprintf(message,"Underflow: %d", underflow );
        legend->AddEntry(h12l, message);

        int overflow = h12l->GetBinContent(myconfiguration.HistoBinningL+1);
        if (overflow<0) overflow=0;
        sprintf(message,"Overflow: %d", overflow );
        legend->AddEntry(h12l, message);

        Amplitude1 = total->GetParameter(0);
        Mean1 = total->GetParameter(1);
        Sigma1 = total->GetParameter(2);
        AmplitudeErr1 = total->GetParError(0);
        MeanErr1 = total->GetParError(1);
        SigmaErr1 = total->GetParError(2);
        Amplitude2 = total->GetParameter(3);
        Mean2 = total->GetParameter(4);
        Sigma2 = total->GetParameter(5);
        AmplitudeErr2 = total->GetParError(3);
        MeanErr2 = total->GetParError(4);
        SigmaErr2 = total->GetParError(5);
        ChiSquareNormalized = total->GetChisquare() / total->GetNDF() ;

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude1, AmplitudeErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean1, MeanErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma1, SigmaErr1 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude2, AmplitudeErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean2, MeanErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma2, SigmaErr2 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit #chi^{2}/NDF = %.1f",total->GetChisquare()/total->GetNDF() );
        legend->AddEntry(fun, message);
    }

    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH11l.pdf"), sizeof("_CH11l.pdf") - 1, "_CH12l.pdf");
    h12l->SetTitle(title.c_str());
    h12l->GetXaxis()->SetLabelSize(label_size);
    h12l->GetXaxis()->SetTitleOffset(axis_offset);
    h12l->GetXaxis()->SetTitleSize(label_size*1.1);
    h12l->GetXaxis()->SetTitle("Charge <adc>");
    h12l->GetYaxis()->SetLabelSize(label_size);
    h12l->GetYaxis()->SetTitleOffset(axis_offset);
    h12l->GetYaxis()->SetTitleSize(label_size*1.1);
    h12l->GetYaxis()->SetTitle("Events");
    h12l->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinL,ChargeMaxL,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    if (myconfiguration.Fit) legend->Draw("same");
    st = (TPaveStats*)h12l->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel13hv")) {
        GGMraw->Draw("channel13hv>>histotemp");
        hv = histotemp->GetMean();
    }

    title = "GGM CH13 low-range ";

    if (myconfiguration.Fit) {
        h13l->Fit(g1,"R0");
        h13l->Fit(g2,"R+0");
        g1->GetParameters(&par[0]);
        g2->GetParameters(&par[3]);
        total->SetParameters(par);
        h13l->Fit(total,"R+");

        legend->Clear();
        TF1 *fun = h13l->GetFunction("total");
        sprintf(message,"Histo Mean: %.1f", h13l->GetMean() );
        legend->AddEntry(h13l, message);

        sprintf(message,"Histo Std Dev: %.1f", h13l->GetStdDev() );
        legend->AddEntry(h13l, message);

        int underflow = h13l->GetBinContent(0);
        if (underflow<0) underflow=0;
        sprintf(message,"Underflow: %d", underflow );
        legend->AddEntry(h13l, message);

        int overflow = h13l->GetBinContent(myconfiguration.HistoBinningL+1);
        if (overflow<0) overflow=0;
        sprintf(message,"Overflow: %d", overflow );
        legend->AddEntry(h13l, message);

        Amplitude1 = total->GetParameter(0);
        Mean1 = total->GetParameter(1);
        Sigma1 = total->GetParameter(2);
        AmplitudeErr1 = total->GetParError(0);
        MeanErr1 = total->GetParError(1);
        SigmaErr1 = total->GetParError(2);
        Amplitude2 = total->GetParameter(3);
        Mean2 = total->GetParameter(4);
        Sigma2 = total->GetParameter(5);
        AmplitudeErr2 = total->GetParError(3);
        MeanErr2 = total->GetParError(4);
        SigmaErr2 = total->GetParError(5);
        ChiSquareNormalized = total->GetChisquare() / total->GetNDF() ;

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude1, AmplitudeErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean1, MeanErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma1, SigmaErr1 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude2, AmplitudeErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean2, MeanErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma2, SigmaErr2 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit #chi^{2}/NDF = %.1f",total->GetChisquare()/total->GetNDF() );
        legend->AddEntry(fun, message);
    }
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH12l.pdf"), sizeof("_CH12l.pdf") - 1, "_CH13l.pdf");
    h13l->SetTitle(title.c_str());
    h13l->GetXaxis()->SetLabelSize(label_size);
    h13l->GetXaxis()->SetTitleOffset(axis_offset);
    h13l->GetXaxis()->SetTitleSize(label_size*1.1);
    h13l->GetXaxis()->SetTitle("Charge <adc>");
    h13l->GetYaxis()->SetLabelSize(label_size);
    h13l->GetYaxis()->SetTitleOffset(axis_offset);
    h13l->GetYaxis()->SetTitleSize(label_size*1.1);
    h13l->GetYaxis()->SetTitle("Events");
    h13l->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinL,ChargeMaxL,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    if (myconfiguration.Fit) legend->Draw("same");
    st = (TPaveStats*)h13l->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel14hv")) {
        GGMraw->Draw("channel14hv>>histotemp");
        hv = histotemp->GetMean();
    }
    title = "GGM CH14 low-range ";

    if (myconfiguration.Fit) {
        h14l->Fit(g1,"R0");
        h14l->Fit(g2,"R+0");
        g1->GetParameters(&par[0]);
        g2->GetParameters(&par[3]);
        total->SetParameters(par);
        h14l->Fit(total,"R+");

        legend->Clear();
        TF1 *fun = h14l->GetFunction("total");
        sprintf(message,"Histo Mean: %.1f", h14l->GetMean() );
        legend->AddEntry(h14l, message);

        sprintf(message,"Histo Std Dev: %.1f", h14l->GetStdDev() );
        legend->AddEntry(h14l, message);

        int underflow = h14l->GetBinContent(0);
        if (underflow<0) underflow=0;
        sprintf(message,"Underflow: %d", underflow );
        legend->AddEntry(h14l, message);

        int overflow = h14l->GetBinContent(myconfiguration.HistoBinningL+1);
        if (overflow<0) overflow=0;
        sprintf(message,"Overflow: %d", overflow );
        legend->AddEntry(h14l, message);

        Amplitude1 = total->GetParameter(0);
        Mean1 = total->GetParameter(1);
        Sigma1 = total->GetParameter(2);
        AmplitudeErr1 = total->GetParError(0);
        MeanErr1 = total->GetParError(1);
        SigmaErr1 = total->GetParError(2);
        Amplitude2 = total->GetParameter(3);
        Mean2 = total->GetParameter(4);
        Sigma2 = total->GetParameter(5);
        AmplitudeErr2 = total->GetParError(3);
        MeanErr2 = total->GetParError(4);
        SigmaErr2 = total->GetParError(5);
        ChiSquareNormalized = total->GetChisquare() / total->GetNDF() ;

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude1, AmplitudeErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean1, MeanErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma1, SigmaErr1 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude2, AmplitudeErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean2, MeanErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma2, SigmaErr2 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit #chi^{2}/NDF = %.1f",total->GetChisquare()/total->GetNDF() );
        legend->AddEntry(fun, message);
    }
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH13l.pdf"), sizeof("_CH13l.pdf") - 1, "_CH14l.pdf");
    h14l->SetTitle(title.c_str());
    h14l->GetXaxis()->SetLabelSize(label_size);
    h14l->GetXaxis()->SetTitleOffset(axis_offset);
    h14l->GetXaxis()->SetTitleSize(label_size*1.1);
    h14l->GetXaxis()->SetTitle("Charge <adc>");
    h14l->GetYaxis()->SetLabelSize(label_size);
    h14l->GetYaxis()->SetTitleOffset(axis_offset);
    h14l->GetYaxis()->SetTitleSize(label_size*1.1);
    h14l->GetYaxis()->SetTitle("Events");
    h14l->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinL,ChargeMaxL,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    if (myconfiguration.Fit) legend->Draw("same");
    st = (TPaveStats*)h14l->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());

    if (GGMraw->GetListOfBranches()->Contains("channel15hv")) {
        GGMraw->Draw("channel15hv>>histotemp");
        hv = histotemp->GetMean();
    }

    title = "GGM CH15 low-range ";

    if (myconfiguration.Fit) {
        h15l->Fit(g1,"R0");
        h15l->Fit(g2,"R+0");
        g1->GetParameters(&par[0]);
        g2->GetParameters(&par[3]);
        total->SetParameters(par);
        h15l->Fit(total,"R+");

        legend->Clear();
        TF1 *fun = h15l->GetFunction("total");
        sprintf(message,"Histo Mean: %.1f", h15l->GetMean() );
        legend->AddEntry(h15l, message);

        sprintf(message,"Histo Std Dev: %.1f", h15l->GetStdDev() );
        legend->AddEntry(h15l, message);

        int underflow = h15l->GetBinContent(0);
        if (underflow<0) underflow=0;
        sprintf(message,"Underflow: %d", underflow );
        legend->AddEntry(h15l, message);

        int overflow = h15l->GetBinContent(myconfiguration.HistoBinningL+1);
        if (overflow<0) overflow=0;
        sprintf(message,"Overflow: %d", overflow );
        legend->AddEntry(h15l, message);

        Amplitude1 = total->GetParameter(0);
        Mean1 = total->GetParameter(1);
        Sigma1 = total->GetParameter(2);
        AmplitudeErr1 = total->GetParError(0);
        MeanErr1 = total->GetParError(1);
        SigmaErr1 = total->GetParError(2);
        Amplitude2 = total->GetParameter(3);
        Mean2 = total->GetParameter(4);
        Sigma2 = total->GetParameter(5);
        AmplitudeErr2 = total->GetParError(3);
        MeanErr2 = total->GetParError(4);
        SigmaErr2 = total->GetParError(5);
        ChiSquareNormalized = total->GetChisquare() / total->GetNDF() ;

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude1, AmplitudeErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean1, MeanErr1 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma1, SigmaErr1 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude2, AmplitudeErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean2, MeanErr2 );
        legend->AddEntry(fun, message);
        sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma2, SigmaErr2 );
        legend->AddEntry(fun, message);

        sprintf(message,"Fit #chi^{2}/NDF = %.1f",total->GetChisquare()/total->GetNDF() );
        legend->AddEntry(fun, message);
    }
    if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";
    imagesave = filel.replace(filel.find("_CH14l.pdf"), sizeof("_CH14l.pdf") - 1, "_CH15l.pdf");
    h15l->SetTitle(title.c_str());
    h15l->GetXaxis()->SetLabelSize(label_size);
    h15l->GetXaxis()->SetTitleOffset(axis_offset);
    h15l->GetXaxis()->SetTitleSize(label_size*1.1);
    h15l->GetXaxis()->SetTitle("Charge <adc>");
    h15l->GetYaxis()->SetLabelSize(label_size);
    h15l->GetYaxis()->SetTitleOffset(axis_offset);
    h15l->GetYaxis()->SetTitleSize(label_size*1.1);
    h15l->GetYaxis()->SetTitle("Events");
    h15l->Draw();
    c1->Update();
    axis = new TGaxis(myconfiguration.HistoMinH,gPad->GetUymax(),
         myconfiguration.HistoMaxH, gPad->GetUymax() ,ChargeMinL,ChargeMaxL,510,"-L");
    axis->SetLabelSize(label_size);
    axis->SetLabelSize(0.04);
    axis->SetTitleOffset(axis_offset*3/4);
    axis->SetTitleSize(label_size);
    axis->SetTextFont(42);
    axis->SetLabelFont(42);
    axis->SetTitle("Charge [pC]");
    axis->Draw();
    if (myconfiguration.Fit) legend->Draw("same");
    st = (TPaveStats*)h15l->FindObject("stats");
    st->SetX1NDC(StatBoxX1); //new x start position
    st->SetX2NDC(StatBoxX2); //new x end position
    st->SetY1NDC(StatBoxY1); //new x start position
    st->SetY2NDC(StatBoxY2); //new x end position
    c1->Print(imagesave.c_str());
  }
}

void PrintConnectionStatus(int BHandle, bool info) {
    switch (BHandle) {
        case (0) :
            if (info) printf("Connection is OK\n");
            break;

        default :
            if (info) printf("Connection is NOT OK\n");
            exit(-1);
            break ;
    }

}

void PrintStatusIRQ(int ret, bool info, std::string MessageSuccess, std::string MessageFailure) {
    switch (ret) {
        case cvSuccess   :
            if (info) std::cout << "IRQ enabled " << MessageSuccess << std::endl;
            break;

        case cvBusError     :
            if (info) std::cout << "Bus Error! " << MessageFailure << std::endl;
            break;

        case cvCommError :
            if (info) std::cout << "Comunication Error! " << MessageFailure << std::endl;
            break;

        default          :
            if (info) std::cout << "Unknown error, " <<  MessageFailure << std::endl;
            break ;
    }
}

void PrintStatus(int ret, bool info) {
    switch (ret) {
        case cvSuccess   :
            break;

        case cvBusError     :
            if (info) printf("Bus Error!");
            break;

        case cvCommError :
            if (info) printf("Comunication Error!");
            break;

        default          :
            if (info) printf("Unknown error!");
            break ;
    }
}
