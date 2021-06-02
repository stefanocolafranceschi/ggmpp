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

// Returns true if x is in range [low..high], else false
bool inRange(int low, int high, int x)
{
    return ((x-high)*(x-low) <= 0);
}

double** PrintHistograms(ReadConf myconfiguration, std::string filel) {
    Float_t StatBoxX1 = 0.8;
    Float_t StatBoxX2 = 0.95;
    Float_t StatBoxY1 = 0.55;
    Float_t StatBoxY2 = 0.78;
    int NumberOfChannels, range, kk;

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

    double** FitResults = 0;
    FitResults = new double*[6];
    FitResults[0] = new double[16];
    FitResults[1] = new double[16];
    FitResults[2] = new double[16];
    FitResults[3] = new double[16];
    FitResults[4] = new double[16];
    FitResults[5] = new double[16];
    TFitResultPtr r1=-1, r2=-1, r3=-1;

    for (int m=0;m<16;m++) {                                     
        FitResults[0][m] = 0;
        FitResults[1][m] = 0;
        FitResults[2][m] = 0;
        FitResults[3][m] = 0;
        FitResults[4][m] = 0;
        FitResults[5][m] = 0;
    }

    int hv = 0;
    std::string title;

    TTree* GGMraw;
    TFile *f;
    TPaveStats *st;
    TGaxis *axis;

    // Establishing Fit
    Double_t par[6]={0};
    TF1 *StreamerFunc    = new TF1("StreamerFunc","gaus",myconfiguration.FitGausMin,myconfiguration.FitGausMax);
    TF1 *AvalancheFunc    = new TF1("AvalancheFunc","landau",myconfiguration.FitLandauMin,myconfiguration.FitLandauMax);
    TF1 *TotalFunc = new TF1("TotalFunc","landau(0)+gaus(3)",myconfiguration.FitLandauMin,myconfiguration.FitGausMax);
    TotalFunc->SetLineColor(2);

    // Opening RootFile or RAW (old GGM)
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
    
    TCanvas *c1 = new TCanvas("c1");
    c1->SetTopMargin(0.2);
    c1->SetLeftMargin(0.2);
    c1->SetBottomMargin(0.2);
    
    TH1F *histotemp = new TH1F("histotemp", "GGM raw", 1, 2, 10800);
    TH1F *htemp;

    if ( (myconfiguration.ADCboard) == "965" ) NumberOfChannels = 16;
    if ( (myconfiguration.ADCboard) == "965a" ) NumberOfChannels = 8;

    TString Filter;
    for (range = 0; range <= 1; range++) {           
        if (range==0) {
		htemp = new TH1F("htemp", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);		
	}
        if (range==1) {
		htemp = new TH1F("htemp", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);
	}
        for (int i = 0; i < NumberOfChannels; i++) {
            Filter = "channel" + std::to_string(i) + ">>htemp";

            TString RangeSelector = "range==" + std::to_string(range);
            GGMraw->Draw(Filter, RangeSelector);

            if (range==0) title = "GGM CH " + std::to_string(i) + " high-range ";
            if (range==1) title = "GGM CH " + std::to_string(i) + " low-range ";

            FitResults[5][i] = htemp->GetMean();
            FitResults[3][i] = double( htemp->GetEntries() ) / double(myconfiguration.events);

            double avalanche = 0, streamer = 0, ratio = 0, Avalanche_chi2normalized, chi2normalized;
            // Performing fit only for the Low Range
            if ( (myconfiguration.Fit) && (range == myconfiguration.FitRange) ) {

		// Setting up the Histo legend --------
                legend->Clear();
                sprintf(message,"Histo Entries: %.0f", htemp->GetEntries() );
                legend->AddEntry(htemp, message);
                sprintf(message,"Histo Mean: %.1f", htemp->GetMean() );
                legend->AddEntry(htemp, message);
                sprintf(message,"Histo Std Dev: %.1f", htemp->GetStdDev() );
                legend->AddEntry(htemp, message);
                int underflow, overflow;
                underflow = htemp->GetBinContent(0);

                if (underflow<0) underflow=0;
                sprintf(message,"Underflow: %d", underflow );
                legend->AddEntry(htemp, message);

                if (range==1) overflow = htemp->GetBinContent(myconfiguration.HistoBinningL+1);
                if (range==0) overflow = htemp->GetBinContent(myconfiguration.HistoBinningH+1);
                if (overflow<0) overflow=0;
                sprintf(message,"Overflow: %d", overflow );
                legend->AddEntry(htemp, message);

                //std::cout << "GGM CH " + std::to_string(i) + " htemp entries = " <<  htemp->GetEntries() << std::endl;
                if ( (htemp->GetEntries()) > 490 ) {
    		    r1=-1;
		    r2=-1;
		    r3=-1;
                    double integral1 = (double) htemp->Integral(htemp->FindBin(myconfiguration.FitLandauMin), htemp->FindBin(myconfiguration.FitLandauMax));
                    //std::cout << "GGM CH " + std::to_string(i) + " Integral(500,1500) is " <<  integral1 << std::endl;
                    //scanf ("%d",&kk);

                    if ( integral1 > 10 ) {
 			r1 = htemp->Fit(AvalancheFunc,"R");
                        //htemp->Fit(AvalancheFunc,"R0");
                        //std::cout << "GGM CH " + std::to_string(i) + " Fit Avalanche is " <<  r1 << std::endl;
                        AvalancheFunc->GetParameters(&par[0]);
		    }

                    if (r1==0) {
                        double integral2 = (double) htemp->Integral(htemp->FindBin(myconfiguration.FitGausMin), htemp->FindBin(myconfiguration.FitGausMax));
                        //std::cout << "GGM CH " + std::to_string(i) + " Integral(1500,4000) is " <<  integral2 << std::endl;
		        if ( integral2 > 10 ) {
                            r2 = htemp->Fit(StreamerFunc,"R+0");
                            //htemp->Fit(StreamerFunc,"R+0");
                            //std::cout << "GGM CH " + std::to_string(i) + " Fit Streamer is " <<  r2 << std::endl;                        
                            StreamerFunc->GetParameters(&par[3]);
                        }
                    }

                    if ( (r1==0) && (r2==0) ) {
                          TotalFunc->SetParameters(par);
                          r3 = htemp->Fit(TotalFunc,"R+");
                          //std::cout << "GGM CH " + std::to_string(i) + "Global fit performed" << std::endl;
                          //std::cout << "GGM CH " + std::to_string(i) + "Fit r3 is " << r3 << std::endl;
                          //scanf ("%d",&kk);
                    }
                
                    TF1 *fun;
                    if (r1==0) {
                        fun = htemp->GetFunction("AvalancheFunc");
                        Avalanche_chi2normalized = AvalancheFunc->GetChisquare() / AvalancheFunc->GetNDF();
                        if (Avalanche_chi2normalized<100) {
    		            Amplitude1 = AvalancheFunc->GetParameter(0);
                            Mean1 = AvalancheFunc->GetParameter(1);
                            Sigma1 = AvalancheFunc->GetParameter(2);
                            AmplitudeErr1 = AvalancheFunc->GetParError(0);
                            MeanErr1 = AvalancheFunc->GetParError(1);
                            SigmaErr1 = AvalancheFunc->GetParError(2);
                    
                            sprintf(message,"Fit Amplitude: %.1f #pm %.1f", Amplitude1, AmplitudeErr1 );
                            legend->AddEntry(fun, message);
                            sprintf(message,"Fit Mean: %.1f #pm %.1f", Mean1, MeanErr1 );
                            legend->AddEntry(fun, message);
                            sprintf(message,"Fit Sigma: %.1f #pm %.1f", Sigma1, SigmaErr1 );
                            legend->AddEntry(fun, message);

                            sprintf(message,"Fit #chi^{2}/NDF = %.1f", chi2normalized );
                            legend->AddEntry(fun, message);

                            if ( inRange(0, 2000, Mean1) && inRange(0, 2000, Sigma1) ) avalanche = fun->Integral(Mean1-3*Sigma1, Mean1+3*Sigma1);
                        }
			else {
			    htemp->GetListOfFunctions()->Remove(fun);
  			    gPad->Modified();
			}
                    }
                    if (r3==0) {                       
                        fun = htemp->GetFunction("TotalFunc");
                        chi2normalized = TotalFunc->GetChisquare() / TotalFunc->GetNDF();
                        if (chi2normalized<Avalanche_chi2normalized) {
                            TF1 *oldfun;
                            oldfun = htemp->GetFunction("AvalancheFunc");
                            htemp->GetListOfFunctions()->Remove(oldfun);
                            gPad->Modified();
                            Amplitude1 = TotalFunc->GetParameter(0);
                            Mean1 = TotalFunc->GetParameter(1);
                            Sigma1 = TotalFunc->GetParameter(2);
                            AmplitudeErr1 = TotalFunc->GetParError(0);
                            MeanErr1 = TotalFunc->GetParError(1);
                            SigmaErr1 = TotalFunc->GetParError(2);
                            Amplitude2 = TotalFunc->GetParameter(3);
                            Mean2 = TotalFunc->GetParameter(4);
                            Sigma2 = TotalFunc->GetParameter(5);
                            AmplitudeErr2 = TotalFunc->GetParError(3);
                            MeanErr2 = TotalFunc->GetParError(4);
                            SigmaErr2 = TotalFunc->GetParError(5);

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

                            sprintf(message,"Fit #chi^{2}/NDF = %.1f", chi2normalized );
                            legend->AddEntry(fun, message);
                            if ( inRange(0, 2000, Mean1) && inRange(0, 2000, Sigma1) ) avalanche = fun->Integral(Mean1-3*Sigma1, Mean1+3*Sigma1);
                            if ( inRange(0, 5000, Mean2) && inRange(0, 50000, Sigma2) ) streamer = fun->Integral(Mean2-3*Sigma2, Mean2+3*Sigma2);

                            if (streamer>0) double ratio = double(avalanche) / double(streamer);
			}
                        else {
                            htemp->GetListOfFunctions()->Remove(fun);
                            gPad->Modified();
                        }
                    }
    	            
                    FitResults[0][i] = avalanche;
                    FitResults[1][i] = streamer;
                    FitResults[2][i] = ratio;

                    //double efficiency = double( htemp->GetEntries() ) / double(myconfiguration.events); 
                    //FitResults[3][i] = efficiency;

                    FitResults[4][i] = chi2normalized;

                    //std::cout << "Avalanche " << avalanche <<  " Streamer " << streamer << " Ratio " << ratio << " Efficiency " << efficiency << std::endl;
                }
            }

            TString BranchFilter = "channel" + std::to_string(i) + "hv";
            if ( GGMraw->GetListOfBranches()->Contains(BranchFilter) ) {
                TString HvFilter = "channel" + std::to_string(i) + "hv>>histotemp";
                GGMraw->Draw(HvFilter);
                hv = histotemp->GetMean();
            }
            if (hv>1) title = title + "(HV=" + std::to_string( hv ) + "V)";

            if ( filel.find(".raw") != std::string::npos ) {
                if (range==0) imagesave = filel.substr (0, filel.length()-4) + "_CH" + std::to_string(i) + "h.pdf";
                if (range==1) imagesave = filel.substr (0, filel.length()-4) + "_CH" + std::to_string(i) + "l.pdf";
            }
            else {
                if (range==0) imagesave = filel.substr (0, filel.length()-5) + "_CH" + std::to_string(i) + "h.pdf";
                if (range==1) imagesave = filel.substr (0, filel.length()-5) + "_CH" + std::to_string(i) + "l.pdf";
            }
           
            htemp->SetTitle(title.c_str());
            htemp->GetXaxis()->SetLabelSize(label_size);
            htemp->GetXaxis()->SetTitleOffset(axis_offset);
            htemp->GetXaxis()->SetTitleSize(label_size*1.1);
            htemp->GetXaxis()->SetTitle("Charge <adc>");
            htemp->GetYaxis()->SetLabelSize(label_size);
            htemp->GetYaxis()->SetTitleOffset(axis_offset);
            htemp->GetYaxis()->SetTitleSize(label_size*1.1);
            htemp->GetYaxis()->SetTitle("Events");

            htemp->Draw();
            if ( (myconfiguration.Fit) && ( range == myconfiguration.FitRange) ) legend->Draw("same");
            c1->Update();

	    int xmin, xmax, chargemin, chargemax;
            if (range==1) {
       	        xmin = myconfiguration.HistoMinL;
                xmax = myconfiguration.HistoMaxL;
                chargemin = ChargeMinL;
                chargemax = ChargeMaxL;
            }
            if (range==0) {
                xmin = myconfiguration.HistoMinH;
                xmax = myconfiguration.HistoMaxH;
                chargemin = ChargeMinH;
                chargemax = ChargeMaxH;
            }

            axis = new TGaxis(xmin,gPad->GetUymax(),
                             xmax, gPad->GetUymax() ,chargemin, chargemax,510,"-L");
            axis->SetLabelSize(label_size);
            axis->SetLabelSize(0.04);
            axis->SetTitleOffset(axis_offset*3/4);
            axis->SetTitleSize(label_size);
            axis->SetTextFont(42);
            axis->SetLabelFont(42);
            axis->SetTitle("Charge [pC]");
            axis->Draw();

            st = (TPaveStats*)htemp->FindObject("stats");
            st->SetX1NDC(StatBoxX1); //new x start position
            st->SetX2NDC(StatBoxX2); //new x end position
            st->SetY1NDC(StatBoxY1); //new x start position
            st->SetY2NDC(StatBoxY2); //new x end position
            c1->Print(imagesave.c_str());
        }
    }
    return FitResults;
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

