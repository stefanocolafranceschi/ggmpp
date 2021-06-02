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
#include <TRandom.h>
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

uint16_t read_reg(long handle, ulong BaseAddress, uint16_t reg_addr) {
        uint16_t data=0;
        CVErrorCodes ret;
        ret = CAENVME_ReadCycle(handle, BaseAddress + reg_addr, &data, cvA32_U_DATA, cvD16);
        if(ret != cvSuccess) std::cout << "Cannot read at BaseAddress=" << std::to_string(BaseAddress) << " " << std::to_string(reg_addr) << std::endl;
        return(data);
}

void write_reg(long handle, ulong BaseAddress, uint16_t reg_addr, uint16_t data) {
        CVErrorCodes ret;
        ret = CAENVME_WriteCycle(handle, BaseAddress + reg_addr, &data, cvA32_U_DATA, cvD16);
        if(ret != cvSuccess) std::cout << "Cannot write at BaseAddress=" << std::to_string(BaseAddress) << " " << std::to_string(reg_addr) << std::endl;
}

void StartGGMPulser(long BHandle, ReadConf myconfiguration);

void StopGGMPulser(long BHandle, ReadConf myconfiguration);

void PrintConnectionStatus(int BHandle, bool info);

void PrintStatus(int ret, bool info);

void PrintUsage();

void PrintStatusIRQ(int ret, bool info, std::string MessageSuccess, std::string MessageFailure);

double** PrintHistograms(ReadConf myconfiguration, std::string filel);

void GGMmail(ReadConf myconfiguration, std::string filel);

void ReadQCD(TTree *GGMraw, ReadConf myconfiguration);

std::string OutputFile(ReadConf myconfiguration);

int** PayloadProcess(ReadConf myconfiguration, int BHandle, ulong BoardAddress);

int** PayloadSimulator(ReadConf myconfiguration, TRandom *ran);

int main(int argc, char** argv) {

    TRandom *ran = new TRandom();
int lll;
    if ((argc > 1) && (argv[1][0] == '-')) {        
        switch (argv[1][1]) {            
            case 'a':                
                //printf("%s\n",&argv[1][3]);                                
                analysis = true;
                filel = &argv[1][3];
                break;

            case 'c':                                
                configFile = &argv[1][3];
                break;
           
           case 'u':
                PrintUsage();
                exit(-1);

            default:
                printf("Wrong Argument: %s\n", argv[1]);
                PrintUsage();
        }
        ++argv;
        --argc;
    }
    ReadConf myconfiguration;
    myconfiguration.InputFile = configFile;
    myconfiguration.LoadConfiguration();
    if (myconfiguration.info) printf("GGM++ Main routine\n\n");
    if (myconfiguration.ConfigurationPrint) myconfiguration.PrintConfiguration();


    if (analysis) {
        try {
            GGMResults = PrintHistograms(myconfiguration, filel);
        }
        catch (...)  {
            std::cout << "Default Exception" << std::endl;
        }
        if (myconfiguration.SendMail) GGMmail(myconfiguration, filel);
    }

    if (!analysis) {
        BoardAddress = std::stoull(myconfiguration.sBoardAddress,nullptr,16);

        static  man_par_t    man;
        man.addr = BoardAddress;
        man.level = 1 ;
        man.am = cvA32_U_DATA ;
        man.dtsize = cvD32 ;
        man.basaddr = BoardAddress ;
        man.blts = 256 ;
        man.ncyc = 1 ;
        man.autoinc = 0 ;
        
        time_t now;
        time_t start_countdown,end_countdown;

        // Tree and Branch definitions
        TTree *GGMraw;// = new TTree("GGMraw","GGMraw");
/*
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
  */      
        // Allocate 32K for the software buffer containing data for blt
        man.buff = (ulong *)malloc(16*1024*1024);
        if (man.buff == NULL) {
            printf("Error. Can't allocate memory for BLT buffer\n");
            exit(-1);
        }
        

        // Handling the VME Board
        if (myconfiguration.VMEcontroller == "V2718") VMEBoard=cvV2718;
        if (myconfiguration.VMEcontroller == "V1718") VMEBoard=cvV1718;
        CAENVME_Init(VMEBoard, myconfiguration.Board, myconfiguration.Link, &BHandle);
        if (myconfiguration.info) printf("Trying to connect to the controller:\n");
        PrintConnectionStatus(BHandle, myconfiguration.info);


        // Reset the board
        write_reg(BHandle,BoardAddress,0x1016, 0);

        // Read the firmware Revision
        uint16_t fwrev = read_reg(BHandle,BoardAddress,0x1000);	
        if (myconfiguration.info) printf("FW Revision = %d.%d\n", (fwrev >> 8) & 0xFF, fwrev & 0xFF);

        uint16_t Iped = 100;
        write_reg(BHandle,BoardAddress,0x1060, Iped);  // Set pedestal
        write_reg(BHandle,BoardAddress, 0x1010, 0x60);  // enable BERR to close BLT at and of block


        // Set LLD (low level threshold for ADC data)	
        write_reg(BHandle,BoardAddress,0x1034, 0x100);  // set threshold step = 16
        for(int i=0; i<16; i++) {
            write_reg(BHandle,BoardAddress,0x1080 + i*4, 0);
        }

        //if (!EnableSuppression) {
        write_reg(BHandle,BoardAddress, 0x1032, 0x0010);  // disable zero suppression
        write_reg(BHandle,BoardAddress, 0x1032, 0x0008);  // disable overrange suppression
        write_reg(BHandle,BoardAddress, 0x1032, 0x1000);  // enable empty events
        //}

        if (myconfiguration.PulserEnable == 1) {
            StartGGMPulser(BHandle, myconfiguration);
        }
        else {
            StopGGMPulser(BHandle, myconfiguration);
        }
	
        // clear Event Counter	
        write_reg(BHandle,BoardAddress, 0x1040, 0x0);
    	
        // clear QTP	
        write_reg(BHandle,BoardAddress, 0x1032, 0x4);	
        write_reg(BHandle,BoardAddress, 0x1034, 0x4);

        if (myconfiguration.DaqEnable == 1) {
            if (myconfiguration.NumberOfRuns == 0) jj=0;
            for ( jj; jj <= myconfiguration.NumberOfRuns; jj++ ) {
        	event = 0;
                if (myconfiguration.NumberOfRuns==0) jj--;
                        
                // Setting the RUN number and unixtime timestamp
                time_t now;
                struct tm  ts;
                time(&now);
                
                ts = *localtime(&now);
                strftime(buf, sizeof(buf), "%Y%m%d-%H%M", &ts);

        // Tree and Branch definitions
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

                // Preparing output files
                filel = OutputFile(myconfiguration);                

                // Opening the root data file
                TFile *hfile = 0;
                if (myconfiguration.rootfile) hfile = TFile::Open(filel.c_str(),"RECREATE");

                if ( (myconfiguration.info) && (myconfiguration.DaqMode =="Physics") ) printf("Reading the ADC controller...\n");
                if ( (myconfiguration.info) && (myconfiguration.DaqMode =="Montecarlo") ) printf("Simulating the ADC controller...\n");

                // Data taking routine
                while (event < myconfiguration.events) {
                    if ( (myconfiguration.DaqMode) == "Physics" ) charge = PayloadProcess(myconfiguration, BHandle, BoardAddress);
                    if ( (myconfiguration.DaqMode) == "Montecarlo" ) charge = PayloadSimulator(myconfiguration, ran);

                    event = event + 1;
                    unixtime = static_cast<long int> (now);

                    rem = rem + 1;
                    if (rem == myconfiguration.eventmonitor) {
                        if (myconfiguration.eventmonitor!=666) printf("\nEvent number: %d\n\n\n",(event-1) );
                        rem = 0;
                    }
                    //std::cout << man.data << std::endl;

                    // Rootfile filling
                    if (myconfiguration.rootfile) {
                        range = 0;
                        charge0=charge[0][0];
                        charge1=charge[0][1];
                        charge2=charge[0][2];
                        charge3=charge[0][3];
                        charge4=charge[0][4];
                        charge5=charge[0][5];
                        charge6=charge[0][6];
                        charge7=charge[0][7];
                        charge8=charge[0][8];
                        charge9=charge[0][9];
                        charge10=charge[0][10];
                        charge11=charge[0][11];
                        charge12=charge[0][12];
                        charge13=charge[0][13];
                        charge14=charge[0][14];
                        charge15=charge[0][15];
                        GGMraw->Fill();

                        range = 1;
                        charge0=charge[1][0];
                        charge1=charge[1][1];
                        charge2=charge[1][2];
                        charge3=charge[1][3];
                        charge4=charge[1][4];
                        charge5=charge[1][5];
                        charge6=charge[1][6];
                        charge7=charge[1][7];
                        charge8=charge[1][8];
                        charge9=charge[1][9];
                        charge10=charge[1][10];
                        charge11=charge[1][11];
                        charge12=charge[1][12];
                        charge13=charge[1][13];
                        charge14=charge[1][14];
                        charge15=charge[1][15];
                        GGMraw->Fill();
                    }
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
                        temporary = "";
                        p = popen(command.c_str(),"r");
                        while( (ch=fgetc(p)) != EOF)
                            //putchar(ch);
                            temporary = temporary + ch;
                        pclose(p);
                        //std::cout << temporary << std::endl;

                        hv[i] = std::stod(temporary);
                    }
                }
                //charge0hv = hv[0];
                //charge1hv = hv[1];
                charge2hv = hv[1];
                charge3hv = hv[2];
                charge4hv = hv[3];
                charge5hv = hv[4];
                charge6hv = hv[5];
                charge7hv = hv[6];
                charge8hv = hv[7];
                charge9hv = hv[8];
                //charge10hv = hv[10];
                //charge11hv = hv[11];
                //charge12hv = hv[12];
                //charge13hv = hv[13];
                //charge14hv = hv[14];
                //charge15hv = hv[15];
                GGMraw->Fill();

                if (myconfiguration.PrintHv) {
                    for (int i = 0; i <= 15; i++) {
                        std::cout << "Value of hv[" << i << "] = " << hv[i] << std::endl;
                    }
                }

                if (myconfiguration.rootfile)	{
//std::cout << "before writing";
//scanf ("%d",&lll);
                    GGMraw->Write();
//std::cout << "after writing";
//scanf ("%d",&lll);
                    if (myconfiguration.info) printf("Rootfile File written..");
                    hfile->Close();
                    GGMraw->Delete();

                    if (myconfiguration.QuickHisto) GGMResults = PrintHistograms(myconfiguration, filel);
                    if ( (myconfiguration.SendToPVSS) && (myconfiguration.DaqEnable == 1) && (myconfiguration.DaqMode =="Physics") ) {
                        std::string command;
                        for (int i = 1; i <= 8; i++) {
                            std::string command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":rpc_ggm_ch0" + std::to_string(static_cast<long long>(i)) + ".daq.avalanche:_original.._value " + std::to_string(GGMResults[0][i]) + " " + myconfiguration.psxmachine;
        		            ///opt/xdaq/bin/dpSet.sh cms_rpc_dcs_1:rpc_ggm_ch04.daq.avalanche:_original.._value 100 http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30
                            //std::cout << "XDaq Send Command : " << command << std::endl;
                            system(command.c_str());
                            command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":rpc_ggm_ch0" + std::to_string(static_cast<long long>(i)) + ".daq.streamer:_original.._value " + std::to_string(GGMResults[1][i]) + " " + myconfiguration.psxmachine;
                            system(command.c_str());
                            command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":rpc_ggm_ch0" + std::to_string(static_cast<long long>(i)) + ".daq.ratio:_original.._value " + std::to_string(GGMResults[2][i]) + " " + myconfiguration.psxmachine;
                            system(command.c_str());
                            command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":rpc_ggm_ch0" + std::to_string(static_cast<long long>(i)) + ".daq.efficiecy:_original.._value " + std::to_string(GGMResults[3][i]) + " " + myconfiguration.psxmachine;
                            system(command.c_str());
                            command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":rpc_ggm_ch0" + std::to_string(static_cast<long long>(i)) + ".daq.chi2square:_original.._value " + std::to_string(GGMResults[4][i]) + " " + myconfiguration.psxmachine;
                            system(command.c_str());
                            command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":rpc_ggm_ch0" + std::to_string(static_cast<long long>(i)) + ".daq.charge:_original.._value " + std::to_string(GGMResults[5][i]) + " " + myconfiguration.psxmachine;
                            system(command.c_str());
                        }
                    }


                    auto oldFile = TFile::Open( filel.c_str() );
                    TTree *oldtree = (TTree*)oldFile->Get("GGMraw");
                    TFile *newfile = new TFile( filel.c_str(),"recreate" );
                    auto newtree = oldtree->CloneTree();

            	    newtree->Branch("avalanche0",&avalanche0,"avalanche0/D");
                    newtree->Branch("avalanche1",&avalanche1,"avalanche1/D");
                    newtree->Branch("avalanche2",&avalanche2,"avalanche2/D");
                    newtree->Branch("avalanche3",&avalanche3,"avalanche3/D");
                    newtree->Branch("avalanche4",&avalanche4,"avalanche4/D");
                    newtree->Branch("avalanche5",&avalanche5,"avalanche5/D");
                    newtree->Branch("avalanche6",&avalanche6,"avalanche6/D");
                    newtree->Branch("avalanche7",&avalanche7,"avalanche7/D");
                    newtree->Branch("avalanche8",&avalanche8,"avalanche8/D");
                    newtree->Branch("avalanche9",&avalanche9,"avalanche9/D");
                    newtree->Branch("avalanche10",&avalanche10,"avalanche10/D");
                    newtree->Branch("avalanche11",&avalanche11,"avalanche11/D");
                    newtree->Branch("avalanche12",&avalanche12,"avalanche12/D");
                    newtree->Branch("avalanche13",&avalanche13,"avalanche13/D");
                    newtree->Branch("avalanche14",&avalanche14,"avalanche14/D");
                    newtree->Branch("avalanche15",&avalanche15,"avalanche15/D");

                    newtree->Branch("streamer0",&streamer0,"streamer0/D");
                    newtree->Branch("streamer1",&streamer1,"streamer1/D");
                    newtree->Branch("streamer2",&streamer2,"streamer2/D");
                    newtree->Branch("streamer3",&streamer3,"streamer3/D");
                    newtree->Branch("streamer4",&streamer4,"streamer4/D");
                    newtree->Branch("streamer5",&streamer5,"streamer5/D");
                    newtree->Branch("streamer6",&streamer6,"streamer6/D");
                    newtree->Branch("streamer7",&streamer7,"streamer7/D");
                    newtree->Branch("streamer8",&streamer8,"streamer8/D");
                    newtree->Branch("streamer9",&streamer9,"streamer9/D");
                    newtree->Branch("streamer10",&streamer10,"streamer10/D");
                    newtree->Branch("streamer11",&streamer11,"streamer11/D");
                    newtree->Branch("streamer12",&streamer12,"streamer12/D");
                    newtree->Branch("streamer13",&streamer13,"streamer13/D");
                    newtree->Branch("streamer14",&streamer14,"streamer14/D");
                    newtree->Branch("streamer15",&streamer15,"streamer15/D");

                    newtree->Branch("ratio0",&ratio0,"ratio0/D");
                    newtree->Branch("ratio1",&ratio1,"ratio1/D");
                    newtree->Branch("ratio2",&ratio2,"ratio2/D");
                    newtree->Branch("ratio3",&ratio3,"ratio3/D");
                    newtree->Branch("ratio4",&ratio4,"ratio4/D");
                    newtree->Branch("ratio5",&ratio5,"ratio5/D");
                    newtree->Branch("ratio6",&ratio6,"ratio6/D");
                    newtree->Branch("ratio7",&ratio7,"ratio7/D");
                    newtree->Branch("ratio8",&ratio8,"ratio8/D");
                    newtree->Branch("ratio9",&ratio9,"ratio9/D");
                    newtree->Branch("ratio10",&ratio10,"ratio10/D");
                    newtree->Branch("ratio11",&ratio11,"ratio11/D");
                    newtree->Branch("ratio12",&ratio12,"ratio12/D");
                    newtree->Branch("ratio13",&ratio13,"ratio13/D");
                    newtree->Branch("ratio14",&ratio14,"ratio14/D");
                    newtree->Branch("ratio15",&ratio15,"ratio15/D");
                 
                    newtree->Branch("efficiency0",&efficiency0,"efficiency0/D");
            	    newtree->Branch("efficiency1",&efficiency1,"efficiency1/D");
                    newtree->Branch("efficiency2",&efficiency2,"efficiency2/D");
            	    newtree->Branch("efficiency3",&efficiency3,"efficiency3/D");
          	        newtree->Branch("efficiency4",&efficiency4,"efficiency4/D");
            	    newtree->Branch("efficiency5",&efficiency5,"efficiency5/D");
            	    newtree->Branch("efficiency6",&efficiency6,"efficiency6/D");
            	    newtree->Branch("efficiency7",&efficiency7,"efficiency7/D");
            	    newtree->Branch("efficiency8",&efficiency8,"efficiency8/D");
                    newtree->Branch("efficiency9",&efficiency9,"efficiency9/D");
                    newtree->Branch("efficiency10",&efficiency10,"efficiency10/D");
                    newtree->Branch("efficiency11",&efficiency11,"efficiency11/D");
                    newtree->Branch("efficiency12",&efficiency12,"efficiency12/D");
                    newtree->Branch("efficiency13",&efficiency13,"efficiency13/D");
                    newtree->Branch("efficiency14",&efficiency14,"efficiency14/D");
                    newtree->Branch("efficiency15",&efficiency15,"efficiency15/D");

                    newtree->Branch("chisquare0",&chisquare0,"chisquare0/D");
                    newtree->Branch("chisquare1",&chisquare1,"chisquare1/D");
                    newtree->Branch("chisquare2",&chisquare2,"chisquare2/D");
                    newtree->Branch("chisquare3",&chisquare3,"chisquare3/D");
                    newtree->Branch("chisquare4",&chisquare4,"chisquare4/D");
                    newtree->Branch("chisquare5",&chisquare5,"chisquare5/D");
                    newtree->Branch("chisquare6",&chisquare6,"chisquare6/D");
                    newtree->Branch("chisquare7",&chisquare7,"chisquare7/D");
                    newtree->Branch("chisquare8",&chisquare8,"chisquare8/D");
                    newtree->Branch("chisquare9",&chisquare9,"chisquare9/D");
                    newtree->Branch("chisquare10",&chisquare10,"chisquare10/D");
                    newtree->Branch("chisquare11",&chisquare11,"chisquare11/D");
                    newtree->Branch("chisquare12",&chisquare12,"chisquare12/D");
                    newtree->Branch("chisquare13",&chisquare13,"chisquare13/D");
                    newtree->Branch("chisquare14",&chisquare14,"chisquare14/D");
                    newtree->Branch("chisquare15",&chisquare15,"chisquare15/D");

                    if (myconfiguration.rootfile) {
        		        avalanche0 = GGMResults[0][0];
                        avalanche1 = GGMResults[0][1];
                        avalanche2 = GGMResults[0][2];
                        avalanche3 = GGMResults[0][3];
                        avalanche4 = GGMResults[0][4];
                        avalanche5 = GGMResults[0][5];
                        avalanche6 = GGMResults[0][6];
                        avalanche7 = GGMResults[0][7];
                        avalanche8 = GGMResults[0][8];
                        avalanche9 = GGMResults[0][9];
                        avalanche10 = GGMResults[0][10];
                        avalanche11 = GGMResults[0][11];
                        avalanche12 = GGMResults[0][12];
                        avalanche13 = GGMResults[0][13];
                        avalanche14 = GGMResults[0][14];
                        avalanche15 = GGMResults[0][15];

                        streamer0 = GGMResults[1][0];
                        streamer1 = GGMResults[1][1];
                        streamer2 = GGMResults[1][2];
                        streamer3 = GGMResults[1][3];
                        streamer4 = GGMResults[1][4];
                        streamer5 = GGMResults[1][5];
                        streamer6 = GGMResults[1][6];
                        streamer7 = GGMResults[1][7];
                        streamer8 = GGMResults[1][8];
                        streamer9 = GGMResults[1][9];
                        streamer10 = GGMResults[1][10];
                        streamer11 = GGMResults[1][11];
                        streamer12 = GGMResults[1][12];
                        streamer13 = GGMResults[1][13];
                        streamer14 = GGMResults[1][14];
                        streamer15 = GGMResults[1][15];

                        ratio0 = GGMResults[2][0];
                        ratio1 = GGMResults[2][1];
                        ratio2 = GGMResults[2][2];
                        ratio3 = GGMResults[2][3];
                        ratio4 = GGMResults[2][4];
                        ratio5 = GGMResults[2][5];
                        ratio6 = GGMResults[2][6];
                        ratio7 = GGMResults[2][7];
                        ratio8 = GGMResults[2][8];
                        ratio9 = GGMResults[2][9];
                        ratio10 = GGMResults[2][10];
                        ratio11 = GGMResults[2][11];
                        ratio12 = GGMResults[2][12];
                        ratio13 = GGMResults[2][13];
                        ratio14 = GGMResults[2][14];
                        ratio15 = GGMResults[2][15];

                        efficiency0 = GGMResults[3][0];
                        efficiency1 = GGMResults[3][1];
                        efficiency2 = GGMResults[3][2];
                        efficiency3 = GGMResults[3][3];
                        efficiency4 = GGMResults[3][4];
                        efficiency5 = GGMResults[3][5];
                        efficiency6 = GGMResults[3][6];
                        efficiency7 = GGMResults[3][7];
                        efficiency8 = GGMResults[3][8];
                        efficiency9 = GGMResults[3][9];
                        efficiency10 = GGMResults[3][10];
                        efficiency11 = GGMResults[3][11];
                        efficiency12 = GGMResults[3][12];
                        efficiency13 = GGMResults[3][13];
                        efficiency14 = GGMResults[3][14];
                        efficiency15 = GGMResults[3][15];

                        chisquare0 = GGMResults[4][0];
                        chisquare1 = GGMResults[4][1];
                        chisquare2 = GGMResults[4][2];
                        chisquare3 = GGMResults[4][3];
                        chisquare4 = GGMResults[4][4];
                        chisquare5 = GGMResults[4][5];
                        chisquare6 = GGMResults[4][6];
                        chisquare7 = GGMResults[4][7];
                        chisquare8 = GGMResults[4][8];
                        chisquare9 = GGMResults[4][9];
                        chisquare10 = GGMResults[4][10];
                        chisquare11 = GGMResults[4][11];
                        chisquare12 = GGMResults[4][12];
                        chisquare13 = GGMResults[4][13];
                        chisquare14 = GGMResults[4][14];
                        chisquare15 = GGMResults[4][15];
                    }

                    newtree->Fill();	 
                    newtree->Write();
                    newfile->Close();

                    if (myconfiguration.SendMail) GGMmail(myconfiguration, filel);
                }
            }
        }
    }
    if (analysis) {
       std::string arg1(&argv[1][3]);
       std::cout << arg1.assign(argv[3]) << std::endl;
       //PrintHistograms(myconfiguration, arg1);
    }
}
