#include <string.h>
#include <iomanip>
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
#include "TSpectrum.h"

void PrintUsage(void) {
        printf("Usage:\n");
        printf(" -a <name>\n");
        printf(" -c <name>\n");
        exit (8);
}

// Returns true if x is in range [low..high], else false
bool inRange(int low, int high, int x) {
    return ((x-high)*(x-low) <= 0);
}

double** PrintHistograms(ReadConf myconfiguration, std::string filel) {
    Float_t StatBoxX1 = 0.8;
    Float_t StatBoxX2 = 0.95;
    Float_t StatBoxY1 = 0.55;
    Float_t StatBoxY2 = 0.78;
    int NumberOfChannels, range, kk, hv=0;
    std::string title;

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
    Double_t PedAmplitude, PedMean, PedSigma, PedAmplitudeErr, PedMeanErr, PedSigmaErr;

    double** Results = 0;
    Results = new double*[6];
    Results[0] = new double[16];
    Results[1] = new double[16];
    Results[2] = new double[16];
    Results[3] = new double[16];
    Results[4] = new double[16];
    Results[5] = new double[16];
    TFitResultPtr r1=-1, r2=-1, r3=-1;

    for (int m=0;m<16;m++) {                                     
        Results[0][m] = 0;
        Results[1][m] = 0;
        Results[2][m] = 0;
        Results[3][m] = 0;
        Results[4][m] = 0;
        Results[5][m] = 0;
    }

    TTree* GGMraw;
    TFile *f;
    TFile ff;
    TPaveStats *st;
    TGaxis *axis;


    // Establishing Fits -----------------------------------------------
    Double_t par[9] = { 0 };
    TF1 *StreamerFunc = new TF1("StreamerFunc","gaus",myconfiguration.FitGausMin,myconfiguration.FitGausMax);

    TF1 *AvalancheFunc;
    if (myconfiguration.RemovePed) {
        AvalancheFunc   = new TF1("AvalancheFunc","landau(0)",myconfiguration.FitLandauMin,myconfiguration.FitLandauMax);
    }
    else {
        AvalancheFunc   = new TF1("AvalancheFunc","landau(0)+gaus(3)",myconfiguration.FitLandauMin,myconfiguration.FitLandauMax);
    }

    TF1 *TotalFunc = new TF1("TotalFunc","landau(0)+gaus(3)+gaus(3)",myconfiguration.FitLandauMin,myconfiguration.FitGausMax);
    TotalFunc->SetLineColor(2);
    // ----------------------------------------------------------------------



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
        TFile ff(filel.c_str());
        if (ff.IsZombie()) return Results;
        ff.Close();
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
    TH1F *htemp, *htemp2, *htempped, *htempsubtracted, *hAvalanche, *hStreamer;
    TF1 *fun;

    if ( (myconfiguration.ADCboard) == "965a" ) NumberOfChannels = 8;
    if ( (myconfiguration.ADCboard) == "965" ) NumberOfChannels = 16;

    TString Filter;
    int start,stop;
    if (myconfiguration.QuickHisto == "high") {
        start = 0;
        stop = 0;
    }
    if (myconfiguration.QuickHisto == "low") {     
       	start = 1;
       	stop = 1;
    }
    if (myconfiguration.QuickHisto == "both") {     
       	start = 0; 
       	stop = 1;
    }
    if (myconfiguration.QuickHisto == "off") {
        start = 0;
        stop = -1;
    }

    for (range = start; range <= stop; range++) {           
        if (range==0) {
	    htemp = new TH1F("htemp", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);
            htemp2 = new TH1F("htemp2", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);
            htempsubtracted = new TH1F("htempsubtracted", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);
            htempped = new TH1F("htempped", "GGM raw", myconfiguration.HistoBinningH, myconfiguration.HistoMinH, myconfiguration.HistoMaxH);
            hAvalanche = new TH1F("hAvalanche", "GGM raw", myconfiguration.FitLandauBins, myconfiguration.FitLandauMin,myconfiguration.FitLandauMax);
            hStreamer = new TH1F("hStreamer", "GGM raw", myconfiguration.FitGausBins, myconfiguration.FitGausMin,myconfiguration.FitGausMax);
	    }
        if (range==1) {
            htemp = new TH1F("htemp", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);
            htemp2 = new TH1F("htemp2", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);
            htempsubtracted = new TH1F("htempsubtracted", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);
            htempped = new TH1F("htempped", "GGM raw", myconfiguration.HistoBinningL, myconfiguration.HistoMinL, myconfiguration.HistoMaxL);
            hAvalanche = new TH1F("hAvalanche", "GGM raw", 200, myconfiguration.FitLandauMin,myconfiguration.FitLandauMax);
            hStreamer = new TH1F("hStreamer", "GGM raw", 200, myconfiguration.FitGausMin,myconfiguration.FitGausMax);
	    }

        Double_t *xpeaks;
        double PedPosition, RealPedPosition, ScaleFactor, PedAmplitude, RealPedAmplitude;
        int FirstBinAboveTHRped, FirstBinAboveTHRrealped, AboveTHRrealped, AboveTHRped;

        for (int i = 0; i <= 15; i++) {

            //
            // Produce Pedestal histograms (find the first bin above 1o counts)
            //
            if (myconfiguration.RemovePed) {
                Filter = "pedchannel" + std::to_string(i) + ">>htempped";
                TString RangeSelector = "range==" + std::to_string(range);
                GGMraw->Draw(Filter, RangeSelector);
                int binmax = htempped->GetMaximumBin(); PedPosition = htempped->GetXaxis()->GetBinCenter(binmax);
                PedAmplitude = htempped->GetMaximum();

                int pedbins = htempped->GetXaxis()->GetNbins();
                for (int jj=1; jj<pedbins; jj++) {
                    if (htempped->GetBinContent(jj)>30) {
                        FirstBinAboveTHRped = jj;
                        AboveTHRped = htempped->GetBinCenter(FirstBinAboveTHRped);
                        break;
                    } 
                }
                std::cout << i << " AboveTHRped " << AboveTHRped << std::endl;
                std::cout << i << " Ped amplitude and position (second gate) " << PedAmplitude << " " << PedPosition << std::endl;
            }


            //
            // Producing the signal histogram
            //
            Filter = "channel" + std::to_string(i) + ">>htemp";
            TString RangeSelector = "range==" + std::to_string(range);
            GGMraw->Draw(Filter, RangeSelector);

            if (myconfiguration.RemovePed) {
                //
                // Calculate the offset of the RealPed
                //
                int signalbins = htemp->GetXaxis()->GetNbins();
                for (int jj=1; jj<signalbins; jj++) {
                    if (htemp->GetBinContent(jj)>10) {
                        FirstBinAboveTHRrealped = jj;
                        AboveTHRrealped = htemp->GetBinCenter(FirstBinAboveTHRrealped);
                        break;
                    }
                }
                std::cout << i << " AboveTHRrealped " << AboveTHRrealped << std::endl;

                RealPedAmplitude = htemp->GetBinContent(htemp->FindBin(PedPosition) + FirstBinAboveTHRrealped - FirstBinAboveTHRped);
                double Shift = AboveTHRrealped-AboveTHRped;

                //
                // Two Peaks Finder (overwrites RealPedAmplitude and Shift)
                //
                TSpectrum *s = new TSpectrum(2);
                s->Search(htemp);
                xpeaks = s->GetPositionX();
                if (  (xpeaks[0]<xpeaks[1]) && (xpeaks[0]>50) ) {
                    RealPedAmplitude = htemp->GetBinContent( htemp->FindBin(xpeaks[0]) );
                    RealPedPosition = xpeaks[0];
                    Shift = RealPedPosition - PedPosition;
                    std::cout << i << " TWO PEAKS FOUND" << std::endl;
                }
                if (  (xpeaks[1]<xpeaks[0]) && (xpeaks[1]>50) ) {
                    RealPedAmplitude = htemp->GetBinContent( htemp->FindBin(xpeaks[1]) );
                    RealPedPosition = xpeaks[1];
                    Shift = RealPedPosition - PedPosition;
                    std::cout << i << " TWO	PEAKS FOUND" <<	std::endl; 
                }

                //
                // Shift, replot and scale the PED histo
                //
                Filter = "pedchannel" + std::to_string(i) + "+"+std::to_string(int(Shift) )+">>htempped";
                GGMraw->Draw(Filter, RangeSelector);
                ScaleFactor = PedAmplitude / RealPedAmplitude ;
                htempped->Scale(1/ScaleFactor);

                std::cout << i << " RealPedAmplitude " << RealPedAmplitude << std::endl;
                //
        	    // Producing the ped subtracted signal histogram
                //
                Filter = "channel" + std::to_string(i) + ">>htempsubtracted";
                TString RangeSelector = "range==" + std::to_string(range);
                GGMraw->Draw(Filter, RangeSelector);
                htempsubtracted->Add(htempped,-1);
                if (myconfiguration.ChargeADC=="SignalOnly") Results[5][i] = htempsubtracted->GetMean() - PedPosition;
                if (myconfiguration.ChargeADC=="MeanSubtraction") {
                    std::cout << "DEBUG histoMean = " << htemp->GetMean() << " PedMean = " << htempped->GetMean() << std::endl;
                    Results[5][i] = htemp->GetMean() - PedPosition;
                }
                //std::cout << " RealPedPosition="<<RealPedPosition << " PedPos" << PedPosition << std::endl;
            }

            Filter = "channel" + std::to_string(i) + ">>hAvalanche";
            GGMraw->Draw(Filter, RangeSelector);

            Filter = "channel" + std::to_string(i) + ">>hStreamer";
            GGMraw->Draw(Filter, RangeSelector);

            if (range==0) title = "GGM CH " + std::to_string(i) + " high-range ";
            if (range==1) title = "GGM CH " + std::to_string(i) + " low-range ";

            double avalanche = 0, streamer = 0, efficiency=0, ratio = 0, Avalanche_chi2normalized, Streamer_chi2normalized, chi2normalized;

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

            // Performing fit
            if ( (myconfiguration.Fit) && (range == myconfiguration.FitRange) && (htemp->GetEntries() > 490) ) {
                //std::cout << "GGM CH " + std::to_string(i) + " htemp entries = " <<  htemp->GetEntries() << std::endl;
		r1=-1;
	        r2=-1;
                double integral1 = (double) htemp->Integral(htemp->FindBin(myconfiguration.FitLandauMin), htemp->FindBin(myconfiguration.FitLandauMax));
                   
                if ( integral1 > myconfiguration.AvalancheThreshold ) {
                    //AvalancheFunc->SetParameter(0, 6.0);
                    AvalancheFunc->SetParameter(1, myconfiguration.PedAvg[i]+100);
                    AvalancheFunc->SetParLimits(1, myconfiguration.PedAvg[i]+3*myconfiguration.PedSigma[i], myconfiguration.FitLandauMax);
                    AvalancheFunc->SetParameter(2, 10);
                    AvalancheFunc->SetParLimits(2,1,70);
     
                    r1 = htempsubtracted->Fit(AvalancheFunc,"R");
                    //AvalancheFunc->GetParameters(&par[0]);
                    std::cout << "GGM CH " + std::to_string(i) + " Fit Avalanche is " <<  r1 << std::endl;

                    if (r1==0) {
                        fun = htemp->GetFunction("AvalancheFunc");     

                        Amplitude1 = AvalancheFunc->GetParameter(0);
                        Mean1 = AvalancheFunc->GetParameter(1);
                        avalanche = Amplitude1;
                        Sigma1 = AvalancheFunc->GetParameter(2);
                        AmplitudeErr1 = StreamerFunc->GetParError(0);
                        MeanErr1 = StreamerFunc->GetParError(1);
                        SigmaErr1 = StreamerFunc->GetParError(2);

                        Avalanche_chi2normalized = AvalancheFunc->GetChisquare() / AvalancheFunc->GetNDF();
                        efficiency = 100*(1-1/ScaleFactor);
                        if ( (isnan(efficiency)) || (1-1/ScaleFactor<0) ) efficiency = 0;

                        if (Avalanche_chi2normalized<1000) {
                            sprintf(message,"Landau Amplitude: %.1f #pm %.1f", Amplitude1, AmplitudeErr1 );
                            legend->AddEntry(fun, message);
                            sprintf(message,"Landau Mean: %.1f #pm %.1f", Mean1, MeanErr1 );
                            legend->AddEntry(fun, message);
                            sprintf(message,"Landau Sigma: %.1f #pm %.1f", Sigma1, SigmaErr1 );
                            legend->AddEntry(fun, message);
                            sprintf(message,"Fit #chi^{2}/NDF = %.1f", Avalanche_chi2normalized );
                            legend->AddEntry(fun, message);
                            sprintf(message,"Efficiency = %.1f", efficiency );
                            legend->AddEntry(fun, message);
                        }

                        // Try to fit the Streamer (if any)
                        double integral2 = (double) htemp->Integral(htemp->FindBin(myconfiguration.FitGausMin), htemp->FindBin(myconfiguration.FitGausMax));
                        if ( integral2 > myconfiguration.StreamerThreshold ) {
                            r2 = hStreamer->Fit(StreamerFunc,"R+0");
                            std::cout << "GGM CH " + std::to_string(i) + " Fit Streamer is " <<  r2 << std::endl;                        
                            //StreamerFunc->GetParameters(&par[6]);

                            if (r2==0) {
                                fun = hStreamer->GetFunction("StreamerFunc");
                                Streamer_chi2normalized = StreamerFunc->GetChisquare() / StreamerFunc->GetNDF();
                                if (Streamer_chi2normalized<50) {
                                    Amplitude1 = StreamerFunc->GetParameter(0);
                                    Mean1 = StreamerFunc->GetParameter(1);
                                    streamer = Amplitude1;
                                    Sigma1 = StreamerFunc->GetParameter(2);

                                    AmplitudeErr1 = StreamerFunc->GetParError(0);
                                    MeanErr1 = StreamerFunc->GetParError(1);
                                    SigmaErr1 = StreamerFunc->GetParError(2);

                                    sprintf(message,"Streamer Amplitude: %.1f #pm %.1f", Amplitude1, AmplitudeErr1 );
                                    legend->AddEntry(fun, message);
                                    sprintf(message,"Streamer Mean: %.1f #pm %.1f", Mean1, MeanErr1 );
                                    legend->AddEntry(fun, message);
                                    sprintf(message,"Streamer Sigma: %.1f #pm %.1f", Sigma1, SigmaErr1 );
                                    legend->AddEntry(fun, message);
                                    sprintf(message,"Fit #chi^{2}/NDF = %.1f", Streamer_chi2normalized );
                                    legend->AddEntry(fun, message);
                                }
                            }
                        }

                    }
		        }
			    else {
			        htemp->GetListOfFunctions()->Remove(fun);
  			        gPad->Modified();
                }
			}
                    
            if (!isnan(avalanche)) Results[0][i] = avalanche;
            if (!isnan(streamer))  Results[1][i] = streamer;
            if (streamer == 0) {
                Results[2][i] = 0;
            }   
            else {
                Results[2][i] = avalanche/streamer; //ratio;
            }
            if (!isnan(efficiency)) Results[3][i] = efficiency;
            if (!isnan(Avalanche_chi2normalized)) Results[4][i] = Avalanche_chi2normalized;
            if (!isnan(htemp->GetMean())) {
		if (myconfiguration.ChargeADC=="Total") Results[5][i] = htemp->GetMean();
                if (myconfiguration.ChargeADC=="EventByEvent") {
                    Filter = "ProcessedChannel" + std::to_string(i) + ">>htemp2";
                    TString RangeSelector = "range==" + std::to_string(range);
                    GGMraw->Draw(Filter, RangeSelector);
                    Results[5][i] = htemp2->GetMean();
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
            if (myconfiguration.RemovePed) {
                htempped->SetLineColor(kCyan);
                htempped->Draw("same");
                htempsubtracted->SetLineColor(kPink);
                htempsubtracted->Draw("same");
            }

            if (r1==0) AvalancheFunc->Draw("same");
            //if (r2==0) StreamerFunc->Draw("same");
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

            if (i>=2 && i<=9) {
               std::ofstream ofs;
               ofs.open ("hvdata.txt", std::ofstream::out | std::ofstream::app);
               ofs << i << " " << hv << " " << efficiency << std::endl;
               ofs.close();
               std::cout << "INFOSCAN HV "<< std::to_string(hv) << " Efficiency " << efficiency << std::endl;
            }
        }
    }
    f->Close();

    //std::cout << "Returning... " << Results[0][0] << std::endl;

    return Results;
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


