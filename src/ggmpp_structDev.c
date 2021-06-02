//
//  ggmpp.c
//  
//
//  Created by Stefano Colafranceschi on 11/28/20.
//
#define _GLIBCXX_USE_C99 1
#include "ReadConf.h"
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "CAENVMElib.h"
#include <time.h>
#include <iostream> 
#include <fstream>
#include "ggmpp.h"
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
#include <TLorentzVector.h>        
#include "TRint.h"

#include "INIReader.h"
#include "console.h"

void StartGGMPulser(long BHandle, ReadConf myconfiguration);

void StopGGMPulser(long BHandle, ReadConf myconfiguration);

void PrintConnectionStatus(int BHandle, bool info);

//void PrintStatus(int ret, bool info);

//void PrintStatusIRQ(int ret, bool info);

void PrintHistograms(TTree *GGMraw, ReadConf myconfiguration, char *buf);

void GGMmail(ReadConf myconfiguration, char *buf);

void ReadQCD(TTree *GGMraw, ReadConf myconfiguration, int BHandle);

std::string OutputFile(ReadConf myconfiguration);

int main(int argc, char** argv) {
    
    if (argc>1) {
         //std::cout << "Using custom file config file: " << argv[1] << "\n";
         configFile = argv[1];
    }

    ReadConf myconfiguration;
    myconfiguration.InputFile = configFile;
    myconfiguration.LoadConfiguration();
    if (myconfiguration.info) printf("GGM++ Main routine\n\n");
    if (myconfiguration.ConfigurationPrint) myconfiguration.PrintConfiguration();

    // Tree and Branch definitions
    TTree *GGMraw = new TTree("GGMraw","GGMraw");
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

    GGMraw->Branch("channel0hv",&charge0hv,"charge0hv/D");
    GGMraw->Branch("channel1hv",&charge1hv,"charge1hv/D");
    GGMraw->Branch("channel2hv",&charge2hv,"charge2hv/D");
    GGMraw->Branch("channel3hv",&charge3hv,"charge3hv/D");
    GGMraw->Branch("channel4hv",&charge4hv,"charge4hv/D");
    GGMraw->Branch("channel5hv",&charge5hv,"charge5hv/D");
    GGMraw->Branch("channel6hv",&charge6hv,"charge6hv/D");
    GGMraw->Branch("channel7hv",&charge7hv,"charge7hv/D");
    GGMraw->Branch("channel8hv",&charge8hv,"charge8hv/D");
    GGMraw->Branch("channel9hv",&charge9hv,"charge9hv/D");
    GGMraw->Branch("channel10hv",&charge10hv,"charge10hv/D");
    GGMraw->Branch("channel11hv",&charge11hv,"charge11hv/D");
    GGMraw->Branch("channel12hv",&charge12hv,"charge12hv/D");
    GGMraw->Branch("channel13hv",&charge13hv,"charge13hv/D");
    GGMraw->Branch("channel14hv",&charge14hv,"charge14hv/D");
    GGMraw->Branch("channel15hv",&charge15hv,"charge15hv/D");
    

    // Handling the VME Board -------------------------------------
    if (myconfiguration.VMEcontroller == "V2718") VMEBoard=cvV2718;
    if (myconfiguration.VMEcontroller == "V1718") VMEBoard=cvV1718;
    CAENVME_Init(VMEBoard, myconfiguration.Board, myconfiguration.Link, &BHandle);
    if (myconfiguration.info) printf("Trying to connect to the controller:\n");
    PrintConnectionStatus(BHandle, myconfiguration.info);


    // Controller Pulser  -----------------------------------------  
    if (myconfiguration.PulserEnable == 1) {
        StartGGMPulser(BHandle, myconfiguration);
    }
    else {
        StopGGMPulser(BHandle, myconfiguration);
    }



    // QCD Data-taking ------------------------------------------
    if (myconfiguration.DaqEnable == 1) {

    if (myconfiguration.NumberOfRuns == 0) jj=0;
    for ( jj; jj <= myconfiguration.NumberOfRuns; jj++ ) {
        if (myconfiguration.NumberOfRuns==0) jj--;          


        // Opening the root data file
        filel = OutputFile(myconfiguration);
        TFile *hfile = 0;
        if (myconfiguration.rootfile) hfile = TFile::Open(filel.c_str(),"RECREATE");

        if (myconfiguration.info) printf("Reading the ADC controller...\n");
	while (event < myconfiguration.events) {
            if (rem == myconfiguration.eventmonitor) {
                if (myconfiguration.eventmonitor!=666) printf("\nEvent number: %d\n\n\n",(event-1) );
	        rem=0;
            }
            ggmpayload = ReadQCD(GGMraw, myconfiguration, BHandle);
            event++;

            if (myconfiguration.rootfile) {
                range = ggmpayload.range;
                charge0=ggmpayload.charge[0];
                charge1=ggmpayload.charge[1];
                charge2=ggmpayload.charge[2];
                charge3=ggmpayload.charge[3];
                charge4=ggmpayload.charge[4];
                charge5=ggmpayload.charge[5];
                charge6=ggmpayload.charge[6];
                charge7=ggmpayload.charge[7];
                charge8=ggmpayload.charge[8];
                charge9=ggmpayload.charge[9];
                charge10=ggmpayload.charge[10];
                charge11=ggmpayload.charge[11];
                charge12=ggmpayload.charge[12];
                charge13=ggmpayload.charge[13];
                charge14=ggmpayload.charge[14];
                charge15=ggmpayload.charge[15];
                GGMraw->Fill();
           }
        }


        if (myconfiguration.rootfile)   {
            GGMraw->Write();
            if (myconfiguration.info) printf("Rootfile File written..");
        }


        // Reading the HV
        if ( (myconfiguration.hvmode) == "config" ) {
            hv[0] = myconfiguration.hv[0];
            hv[1] = myconfiguration.hv[1];
            hv[2] = myconfiguration.hv[2];
            hv[3] = myconfiguration.hv[3];
            hv[4] = myconfiguration.hv[4];
            hv[5] = myconfiguration.hv[5];
            hv[6] = myconfiguration.hv[6];
            hv[7] = myconfiguration.hv[7];
            hv[8] = myconfiguration.hv[8];
            hv[9] = myconfiguration.hv[9];
            hv[10] = myconfiguration.hv[10];
            hv[11] = myconfiguration.hv[11];
        }
        if ( (myconfiguration.hvmode) == "xdaq" ) {
            for (int i = 1; i <= 8; i++) {
                std::string command = myconfiguration.xdaqcommand + " " + myconfiguration.rpcmachine + ":rpc_ggm_ch0" + std::to_string(static_cast<long long>(i)) + ".hv:_online.._value " + myconfiguration.psxmachine + " | grep -oP '(?<=<psx:dp name=\"" + myconfiguration.rpcmachine + ":rpc_ggm_ch0" + std::to_string(static_cast<long long>(i)) + ".hv:_online.._value\">)[^<]*'";
                //std::cout << command;

                p = popen(command.c_str(),"r");
                while( (chh=fgetc(p)) != EOF)
                    //putchar(chh);
                    temporary = temporary + chh;
                pclose(p);
                //std::cout << temporary << std::endl;

                hv[i] = std::stod(temporary);
            }
        }
             
        charge0hv=hv[0];
        charge1hv=hv[1];
        charge2hv=hv[2];
        charge3hv=hv[3];
        charge4hv=hv[4];
        charge5hv=hv[5];
        charge6hv=hv[6];
        charge7hv=hv[7];
        charge8hv=hv[8];
        charge9hv=hv[9];
        charge10hv=hv[10];
        charge11hv=hv[11];
        charge12hv=hv[12];
        charge13hv=hv[13];
        charge14hv=hv[14];
        charge15hv=hv[15];
        GGMraw->Fill();

        if (myconfiguration.PrintHv) {
            for (int i = 0; i <= 15; i++) {
                std::cout << "Value of hv[" << i << "] = " << hv[i] << std::endl;
            }
        }

        if (myconfiguration.rootfile)	{
            GGMraw->Write();
            if (myconfiguration.info) printf("Rootfile File written..");
        }

        if (myconfiguration.QuickHisto) {
            //PrintHistograms(GGMraw, myconfiguration, buf);
        }
        hfile->Close();

        if (myconfiguration.SendMail) {
            //GGMmail(myconfiguration, buf);
        }
   }
   }
}
