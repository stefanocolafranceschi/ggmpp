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

void PrintStatus(int ret, bool info);

void PrintStatusIRQ(int ret, bool info);

void PrintHistograms(TTree *GGMraw, ReadConf myconfiguration, char *buf);

void GGMmail(ReadConf myconfiguration, char *buf);

void ReadQCD(TTree *GGMraw, ReadConf myconfiguration);

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

    cost[23]=8204288,cost[22]=4102144,cost[21]=2051072,cost[20]=1025536,cost[19]=512768,cost[18]=256384,cost[17]=128192,cost[16]=64096,cost[15]=32048,cost[14]=16024,cost[13]=8012,cost[12]=4096,cost[11]=2048,cost[10]=1024,cost[9]=512,cost[8]=256,cost[7]=128,cost[6]=64,cost[5]=32,cost[4]=16,cost[3]=8,cost[2]=4,cost[1]=2,cost[0]=1;
    
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
  
    if (myconfiguration.PulserEnable == 1) {
        StartGGMPulser(BHandle, myconfiguration);
    }
    else {
        StopGGMPulser(BHandle, myconfiguration);
    }

    if (myconfiguration.DaqEnable == 1) {
    if (myconfiguration.NumberOfRuns == 0) jj=0;
    for ( jj; jj <= myconfiguration.NumberOfRuns; jj++ ) {
        if (myconfiguration.NumberOfRuns==0) jj--;
        
        if (myconfiguration.multiplereading) {
            // Setting the IRQ port
            ret = CAENVME_IRQEnable(0,1);
            dd = 100;
            ret=CAENVME_WriteCycle(myconfiguration.Board, (BoardAddress+0x00001020), &dd, cvA32_U_DATA, cvD16);
            switch (ret) {
                case cvSuccess   :
                    if (myconfiguration.info) printf("IRQ enabled to 100 event.\n");
                    break;
                    
                case cvBusError     :
                    if (myconfiguration.info) printf("Bus Error! IRQ doesn't enabled.\n");
                    break;
                    
                case cvCommError :
                    if (myconfiguration.info) printf("Comunication Error! IRQ doesn't enabled.\n");
                    break;
                    
                default          :
                    if (myconfiguration.info) printf("Unknown error, IRQ doesn't enabled.\n");
                    break ;
            }
            
            dd=1;
            ret=CAENVME_WriteCycle(myconfiguration.Board, (BoardAddress+0x0000100A), &dd, cvA32_U_DATA, cvD16);
            PrintStatusIRQ(ret, myconfiguration.info);
        }
        // Setting all channel ADC to notfound
        for (m=0;m<16;m++) {
            charge[0][m] = myconfiguration.NotFound;
            charge[1][m] = myconfiguration.NotFound;
        }
        
        // Setting the RUN number and unixtime timestamp
        time_t now;
        struct tm  ts;
        time(&now);
        
        ts = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y%m%d-%H%M", &ts);
        
        // Preparing output files
        strcpy (filel, myconfiguration.DataFolder.c_str());
        strcat (filel, myconfiguration.FileNamePrefix.c_str());
        strcat (filel, myconfiguration.FileNameSuffix.c_str());
        strcat (filel, buf);
        strcat (filel,".root");
        
        strcpy (filerun, myconfiguration.DataFolder.c_str());
        strcat (filerun, "running.txt");
        
        std::ofstream myfile;
        myfile.open (filerun);
        myfile << myconfiguration.FileNamePrefix.c_str();
        myfile << myconfiguration.FileNameSuffix.c_str();
        myfile << buf;
        myfile << ".root";
        myfile.close();

        if (myconfiguration.PrintDataFile) std::cout << "File being written: " << filel << "\n" << std::endl;

        // Opening the root data file
        TFile *hfile = 0;
        if (myconfiguration.rootfile) hfile = TFile::Open(filel,"RECREATE");
        
        if (myconfiguration.info) printf("Reading the ADC controller...\n");
        while (event < myconfiguration.events) {
            if (rem == myconfiguration.eventmonitor) {
                if (myconfiguration.eventmonitor!=666) printf("\nEvent number: %d\n\n\n",(event-1) );
                rem=0;
            }
            time (&start_countdown);
            
            if (myconfiguration.multiplereading) {
                ret = CAENVME_BLTReadCycle(myconfiguration.Board, BoardAddress,(char *)man.buff, myconfiguration.BLTsize,cvA32_U_DATA,cvD32,&nb);
                PrintStatus(ret, myconfiguration.info);
                
                time (&end_countdown);
                dif = difftime (end_countdown,start_countdown);
                if (dif>10) {
                    if (myconfiguration.info) printf("DAQ halted... no trigger");
                    exit(-1);
                }
                
                
                d32 = man.buff;
                
                for (h=0;h<34;h++) {                                // Reading 32 Event with BLT
                    sprintf(hex[h],"%x",d32[h]);                    // Trasforming DATA ulong into STRING HEX
                    
                    for (ii=0;ii<8;ii++) {
                        k=(hex[h][ii]<='9' && hex[h][ii]>='0' ? hex[h][ii]-'0' : hex[h][ii]-'a'+10);
                        strcpy(bin+4*ii,h2b[k]);
                    }
                    
                    
                    
                    
                    if (bin[5]=='0' && bin[6]=='1' && bin[7]=='0') {              // This is Begin of Block
                        
                        for (m=0;m<16;m++) {                    // Initializing charge
                            charge[0][m] = myconfiguration.NotFound;
                            charge[1][m] = myconfiguration.NotFound;
                        }
                        starting=1;
                        
                    }
                    if (bin[5]=='1' && bin[6]=='0' && bin[7]=='0' && starting==1) {     //this is end of block
                        
                        
                        event=event+1;
                        rem=rem+1;
                        // PROCESSING EVENT BUFFER
                        for (l=0;l<23;l++) {
                            if (bin[9+l]=='1') {
                                dec_buff[l]=cost[23-l];
                            }
                            else {
                                dec_buff[l]=0;
                            }
                        }
                        time(&now);
                        ts = *localtime(&now);
                        strftime(buf2, sizeof(buf2), "%H%M%S", &ts);
                        
                        
                        event_buffer = 0;
                        for (l=0;l<=23;l++) {                           // PROCESSING EVENT BUFFER
                            event_buffer = event_buffer + dec_buff[l];
                        }
                        
                        unixtime = static_cast<long int> (now);
                        
                        // HEX and/or BIN printout
                        if ( (myconfiguration.PayloadHexPrint) || (myconfiguration.PayloadBinPrint) ) std::cout << "PAYLOAD RAW: ";
                        if (myconfiguration.PayloadHexPrint) std::cout << "HEX=" << hex[h] << "      ";
                        if (myconfiguration.PayloadBinPrint) std::cout << "BIN=" << bin;
                        if ( (myconfiguration.PayloadHexPrint) || (myconfiguration.PayloadBinPrint) ) std::cout << std::endl;
                        
                        if (myconfiguration.rootfile) {
                            charge0 = charge[0][0];
                            charge1 = charge[0][1];
                            charge2 = charge[0][2];
                            charge3 = charge[0][3];
                            charge4 = charge[0][4];
                            charge5 = charge[0][5];
                            charge6 = charge[0][6];
                            charge7 = charge[0][7];
                            charge8 = charge[0][8];
                            charge9 = charge[0][9];
                            charge10 = charge[0][10];
                            charge11 = charge[0][11];
                            charge12 = charge[0][12];
                            charge13 = charge[0][13];
                            charge14 = charge[0][14];
                            charge15 = charge[0][15];
                            GGMraw->Fill();

                            charge0 = charge[0][0];
                            charge1 = charge[0][1];
                            charge2 = charge[0][2];
                            charge3 = charge[0][3];
                            charge4 = charge[0][4];
                            charge5 = charge[0][5];
                            charge6 = charge[0][6];
                            charge7 = charge[0][7];
                            charge8 = charge[0][8];
                            charge9 = charge[0][9];
                            charge10 = charge[0][10];
                            charge11 = charge[0][11];
                            charge12 = charge[0][12];
                            charge13 = charge[0][13];
                            charge14 = charge[0][14];
                            charge15 = charge[0][15];
                            GGMraw->Fill();

                            //printf("filling");
                        }
                        
                    }
                    if (bin[5]=='0' && bin[6]=='0' && bin[7]=='0') {		// this is valid data
                        
                        
                        if (bin[11]=='1') {
                            dec1=8;}
                        else {
                            dec1=0;
                        }
                        
                        if (bin[12]=='1') {
                            dec2=4;
                        }
                        else {
                            dec2=0;
                        }
                        if (bin[13]=='1') {
                            dec3=2;
                        }
                        else {
                            dec3=0;
                        }
                        if (bin[14]=='1') {
                            dec4=1;
                        }
                        else {
                            dec4=0;
                        }
                        number_of_channel=dec1+dec2+dec3+dec4;
                        
                        for (l=0;l<12;l++) {        // PROCESSING CHARGE OF CHANNEL
                            if (bin[20+l]=='1') {
                                dec[l]=cost[11-l];
                            }
                            else {
                                dec[l]=0;
                            }
                        }
                        if (bin[15]=='1') range=1;
                        if (bin[15]=='0') range=0;
                        charge[range][number_of_channel]=dec[0]+dec[1]+dec[2]+dec[3]+dec[4]+dec[5]+dec[6]+dec[7]+dec[8]+dec[9]+dec[10]+dec[11];
                    }
                    
                }
            }
            else {
                //ret = CAENVME_ReadCycle(BHandle,man.addr,&man.data,cvA32_U_DATA,cvD32);
                ret = CAENVME_ReadCycle(BHandle, BoardAddress,&man.data,cvA32_U_DATA,cvD32); // It was ""(char *)man.buff""
                PrintStatus(ret, myconfiguration.info);
                
                time (&end_countdown);
                dif = difftime (end_countdown,start_countdown);
                if (dif>10) {
                    if (myconfiguration.info) printf("DAQ halted... no trigger");
                    exit(-1);
                }
                
                mydata = man.data;
                //std::cout << man.data << std::endl;
                sprintf(hexx,"%08x",mydata);                    // Trasforming DATA ulong into STRING HEX
                
                for (ii=0;ii<8;ii++) {
                    k=(hexx[ii]<='9' && hexx[ii]>='0' ? hexx[ii]-'0' : hexx[ii]-'a'+10);
                    strcpy(bin+4*ii,h2b[k]);
                }
                
                
                if (bin[5]=='0' && bin[6]=='1' && bin[7]=='0') {                    // This is Begin of Block
                    if ( (myconfiguration.HeaderHexPrint) || (myconfiguration.HeaderBinPrint) ) std::cout << "HEADER RAW: ";
                    if (myconfiguration.HeaderHexPrint) std::cout << " HEX = " << hexx << "    ";
                    if (myconfiguration.HeaderBinPrint) std::cout << "BIN = " << bin;
                    if ( (myconfiguration.HeaderHexPrint) || (myconfiguration.HeaderBinPrint) ) std::cout << std::endl;
                    
                    int ReadChannels = 0;
                    if ( bin[18]=='1') ReadChannels = ReadChannels + 32;
                    if ( bin[19]=='1') ReadChannels = ReadChannels + 16;
                    if ( bin[20]=='1') ReadChannels = ReadChannels + 8;
                    if ( bin[21]=='1') ReadChannels = ReadChannels + 4;
                    if ( bin[22]=='1') ReadChannels = ReadChannels + 2;
                    if ( bin[23]=='1') ReadChannels = ReadChannels + 1;
                    if ( ReadChannels < myconfiguration.ExpectedWorkingChannels) {
                        std::cerr << "The minimum number of expected channel is set to " << myconfiguration.ExpectedWorkingChannels << " while the header only shows " << ReadChannels << std::endl;
                        exit(-1);
		    }

                    for (m=0;m<16;m++) {                                        // Initializing charge
                        charge[0][m] = myconfiguration.NotFound;
                        charge[1][m] = myconfiguration.NotFound;
                    }
                    starting=1;
                }
                if (bin[5]=='1' && bin[6]=='0' && bin[7]=='0' && starting==1) {     // This is End of Block
                    event=event+1;
                    rem=rem+1;
                    for (l=0;l<23;l++) {                                        // PROCESSING EVENT BUFFER
                        
                        if (bin[9+l]=='1') {
                            dec_buff[l]=cost[23-l];
                        }
                        else {
                            dec_buff[l]=0;
                        }
                    }
                    time(&now);
                    ts = *localtime(&now);
                    strftime(buf2, sizeof(buf2), "%H%M%S", &ts);
                    
                    event_buffer = 0;
                    for (l=0;l<=23;l++) {                                        // PROCESSING EVENT BUFFER
                        event_buffer = event_buffer + dec_buff[l];
                    }
                    unixtime = static_cast<long int> (now);
                    
                    // HEX and/or BIN printout
                    if (myconfiguration.EOBHexPrint) std::cout << "EOB Rawdata HEX=" << hexx;
                    if (myconfiguration.EOBBinPrint) std::cout << "EOB Rawdata BIN=" << bin;
                    if ( (myconfiguration.EOBHexPrint) || (myconfiguration.EOBBinPrint) ) std::cout << std::endl;
                    
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
                        //printf("filling");
                    }
                }
                if (bin[5]=='0' && bin[6]=='0' && bin[7]=='0' ) {       //This is valid data
  

                    // High vs Range selector ------
                    if ( (myconfiguration.ADCboard) == "965" ) {
                        if (bin[15]=='1') range=1;
                        if (bin[15]=='0') range=0;
                    }
                    else if ( (myconfiguration.ADCboard) == "965a" ) {
                        if (bin[14]=='1') range=1;
                        if (bin[14]=='0') range=0;
                    }
                    else {
                       std::cout << "The module " << myconfiguration.ADCboard << " is not recognized" << std::endl;
                       exit(-1);
                    }
                    // -----------------------------

                    // OverFlow and UnderFlow  -----
                    UnderFlow=0;
                    OverFlow=0;
                    if (bin[18]=='1') UnderFlow=1;
                    if (bin[19]=='1') OverFlow=1;
                    // ----------------------------

                    dec1=0;
		    dec2=0;
                    dec3=0;
                    dec4=0;
                    if ( (myconfiguration.ADCboard) == "965" ) {
                        if (bin[11]=='1') dec1 = 8;
                        if (bin[12]=='1') dec2 = 4;
                        if (bin[13]=='1') dec3 = 2;
                        if (bin[14]=='1') dec4 = 1;
                        number_of_channel=dec1+dec2+dec3+dec4;
                    }

                    if ( (myconfiguration.ADCboard) == "965a" ) {
                        if (bin[11]=='1') dec1 = 4;
                        if (bin[12]=='1') dec2 = 2;
                        if (bin[13]=='1') dec3 = 1;
                        number_of_channel=dec1+dec2+dec3+dec4;
                    }                    

                    for (l=0;l<12;l++) {        // PROCESSING CHARGE OF CHANNEL
                        if (bin[20+l]=='1') {
                            dec[l]=cost[11-l];
                        }
                        else {
                            dec[l]=0;
                        }
                    }

                    charge[range][number_of_channel]=dec[0]+dec[1]+dec[2]+dec[3]+dec[4]+dec[5]+dec[6]+dec[7]+dec[8]+dec[9]+dec[10]+dec[11];
                    if (bin[18]=='1') charge[range][number_of_channel]=-1;      // underflow
                    if (bin[19]=='1') charge[range][number_of_channel]=5000;    // overflow

                    // HEX and/or BIN printout
                    if ( (myconfiguration.PayloadHexPrint) || (myconfiguration.PayloadBinPrint) ) std::cout << "PAYLOAD RAW: ";
                    if (myconfiguration.PayloadHexPrint) std::cout << "HEX = " << hexx << "    ";
                    if (myconfiguration.PayloadBinPrint) std::cout << "BIN = " << bin  << "    ";
                    if (myconfiguration.PayloadDecPrint) std::cout << "DEC: Range = " << range << ", Channel = " << number_of_channel << ", <adc> = " << charge[number_of_channel];
                    if ( (myconfiguration.PayloadHexPrint) || (myconfiguration.PayloadBinPrint) ) std::cout << std::endl;
                }
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

                p = popen(command.c_str(),"r");
                while( (ch=fgetc(p)) != EOF)
                    //putchar(ch);
                    temporary = temporary + ch;
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
            PrintHistograms(GGMraw, myconfiguration, buf);
        }
        hfile->Close();

        if (myconfiguration.SendMail) {
            GGMmail(myconfiguration, buf);
        }
   }
   }
}
