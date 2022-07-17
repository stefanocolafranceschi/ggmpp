//
//  ggmpp.c
//  
//
//  Created by Stefano Colafranceschi on 11/28/19.
//
#define _GLIBCXX_USE_C99 1
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

void PrintConnectionStatus(int BHandle);

void PrintStatus(int ret);

void PrintStatusIRQ(int ret);

int main() {
    
    printf("GGM++ Main routine");
    std::string configFile = "config.ini";
    
    std::string FileNamePrefix, FileNameSuffix;
    INIReader reader(configFile);
    
    if (reader.ParseError() != 0) {
        std::cout << "Can't load " << configFile <<" \n";
        return 1;
    }
    
    events = reader.GetInteger("daq", "events", 100);
    Board = reader.GetInteger("daq", "Board", 0);
    Link = reader.GetInteger("daq", "Link", 0);
    sBoardAddress = reader.Get("daq", "BoardAddress", "0");
    multiplereading = reader.GetBoolean("daq", "multiplereading", false);
    BLTsize = reader.GetInteger("daq", "BLTsize", 256);
    ulong BoardAddress = std::stoull(sBoardAddress,nullptr,16);
    NumberOfRuns = reader.GetInteger("daq", "NumberOfRuns", 1);
    DataFolder = reader.Get("daq", "DataFolder", "/tmp");
    FileNamePrefix = reader.Get("daq", "FileNamePrefix", "undefined");
    FileNameSuffix = reader.Get("daq", "FileNameSuffix", "");
    terminal = reader.GetBoolean("daq", "terminal", true);
    rootfile = reader.GetBoolean("daq", "rootfile", true);
    hexprint = reader.GetBoolean("daq", "hexprint", false);
    binprint = reader.GetBoolean("daq", "binprint", false);
    eventmonitor = reader.GetInteger("daq", "eventmonitor", 100);
    spychannel = reader.GetInteger("analysis", "spychannel", 666);
    
    std::cout << "Config loaded from " << configFile
    << ":\n events=" << events
    << ", VME_Board=" << VMEBoard
    << ", BoardAddress=" << std::hex << BoardAddress << std::dec
    << ", BoardID=" << Board
    << ", LinkID=" << Link
    << ", Multiple Reading=" << multiplereading
    << ", BLTsize=" << BLTsize
    << ", FileNamePrefix=" << FileNamePrefix
    << ", FileNameSuffix=" << FileNameSuffix
    << ", terminal=" << terminal
    << ", rootfile=" << rootfile
    << ", EventMonitor=" << eventmonitor
    << ", HexPrint=" << hexprint
    << ", Spy Channel=" << spychannel << "\n";
    
    static  man_par_t    man;
    man.addr = BoardAddress;
    man.level = 1 ;
    man.am = cvA32_U_DATA ;
    man.dtsize = cvD32 ;
    man.basaddr = BoardAddress ;
    man.blts = 170 ;
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
    
    cost[23]=8204288,cost[22]=4102144,cost[21]=2051072,cost[20]=1025536,cost[19]=512768,cost[18]=256384,cost[17]=128192,cost[16]=64096,cost[15]=32048,cost[14]=16024,cost[13]=8012,cost[12]=4096,cost[11]=2048,cost[10]=1024,cost[9]=512,cost[8]=256,cost[7]=128,cost[6]=64,cost[5]=32,cost[4]=16,cost[3]=8,cost[2]=4,cost[1]=2,cost[0]=1;
    
    hv[1]=9800, hv[2]=9800, hv[3]=9800, hv[4]=9800, hv[5]=9800, hv[6]=9800, hv[7]=9800, hv[8]=9800;
    
    
    // Allocate 32K for the software buffer containing data for blt
    man.buff = (ulong *)malloc(16*1024*1024);
    if (man.buff == NULL) {
        printf("Error. Can't allocate memory for BLT buffer\n");
        exit(-1);
    }
    else {
        printf("GGM - Inizialization variables done.\n");
    }
    
    
    // Handling the VME Board
    CAENVME_Init(VMEBoard, Board, Link, &BHandle);
    printf("Trying to connect to the controller:\n");
    PrintConnectionStatus(BHandle);
    
    if (NumberOfRuns == 0) jj=0;
    for ( jj; jj<=NumberOfRuns; jj++ ) {
        if (NumberOfRuns==0) jj--;
        
if (multiplereading) {
        // Setting the IRQ port
        ret = CAENVME_IRQEnable(0,1);
        dd = 100;
        ret=CAENVME_WriteCycle(Board, (BoardAddress+0x00001020), &dd, cvA32_U_DATA, cvD16);
        switch (ret) {
            case cvSuccess   :
                printf("IRQ enabled to 100 event.\n");
                break;
                
            case cvBusError     :
                printf("Bus Error! IRQ doesn't enabled.\n");
                break;
                
            case cvCommError :
                printf("Comunication Error! IRQ doesn't enabled.\n");
                break;
                
            default          :
                printf("Unknown error, IRQ doesn't enabled.\n");
                break ;
        }
        
        dd=1;
        ret=CAENVME_WriteCycle(Board, (BoardAddress+0x0000100A), &dd, cvA32_U_DATA, cvD16);
        PrintStatusIRQ(ret);
        }
        // Setting all channel ADC to 9999
        for (m=0;m<16;m++) {
            charge[m] = notfound;
        }
        
        // Setting the RUN number and unixtime timestamp
        time_t now;
        struct tm  ts;
        time(&now);
        
        ts = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y%m%d-%H%M", &ts);
        
        // Preparing output files
        strcpy (filel, DataFolder.c_str());
        strcat (filel, FileNamePrefix.c_str());
        strcat (filel, FileNameSuffix.c_str());
        strcat (filel, buf);
        strcat (filel,".root");
        
        strcpy (filerun, DataFolder.c_str());
        strcat (filerun, "running.txt");
        
        std::ofstream myfile;
        myfile.open (filerun);
        myfile << FileNamePrefix.c_str();
        myfile << FileNameSuffix.c_str();
        myfile << buf;
        myfile << ".root";
        myfile.close();
        
        // Opening the root data file
        TFile *hfile = 0;
        if (rootfile) hfile = TFile::Open(filel,"RECREATE");
        
        printf("Reading the ADC controller...\n");
        while (event<events) {
            if (rem == eventmonitor) {
                if (eventmonitor!=666) printf("Event number: %d\n",(event-1) );
                rem=0;
            }
            time (&start_countdown);
            
            if (multiplereading) {
                ret = CAENVME_BLTReadCycle(Board,BoardAddress,(char *)man.buff,BLTsize,cvA32_U_DATA,cvD32,&nb);
                PrintStatus(ret);
                
                time (&end_countdown);
                dif = difftime (end_countdown,start_countdown);
                if (dif>10) {
                    printf("DAQ halted... no trigger");
                    exit(-1);
                }
                
                
                d32=man.buff;
                
                for (h=0;h<34;h++) {                                // Reading 32 Event with BLT
                    sprintf(hex[h],"%x",d32[h]);                    // Trasforming DATA ulong into STRING HEX
                    
                    for (ii=0;ii<8;ii++) {
                        k=(hex[h][ii]<='9' && hex[h][ii]>='0' ? hex[h][ii]-'0' : hex[h][ii]-'a'+10);
                        strcpy(bin+4*ii,h2b[k]);
                    }
                    
              
                    
        
                    if (bin[5]=='0' && bin[6]=='1' && bin[7]=='0') {              // This is Begin of Block
                        
                            for (m=0;m<16;m++) {                    // Initializing charge
                                charge[m]=notfound;
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
                            strftime(buf, sizeof(buf), "%H%M%S", &ts);
                            
                            
                            event_buffer = 0;
                            for (l=0;l<=23;l++) {                           // PROCESSING EVENT BUFFER
                                event_buffer = event_buffer + dec_buff[l];
                            }
                            
                            unixtime = static_cast<long int> (now);

// HEX and/or BIN printout
                    if (hexprint) std::cout << " Rawdata HEX=" << hex[h];
                    if (binprint) std::cout << " Rawdata BIN=" << bin;
                    if ( (hexprint) || (binprint) ) std::cout << std::endl;

charge0=charge[0];
charge1=charge[1];
charge2=charge[2];
charge3=charge[3];
charge4=charge[4];
charge5=charge[5];
charge6=charge[6];
charge7=charge[7];
charge8=charge[8];
charge9=charge[9];
charge10=charge[10];
charge11=charge[11];
charge12=charge[12];
charge13=charge[13];
charge14=charge[14];
charge15=charge[15];
                            if (terminal) printf("%d %d %d %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",event,event_buffer,now,buf,charge[0],charge[1],charge[2],charge[3],charge[4],charge[5],charge[6],charge[7],charge[8],charge[9],charge[10],charge[11],charge[12],charge[13],charge[14],charge[15]);
                            if (rootfile) {
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
                            if (bin[15]=='1') range=0;
			    if (bin[15]=='0') range=1;
                            charge[number_of_channel]=dec[0]+dec[1]+dec[2]+dec[3]+dec[4]+dec[5]+dec[6]+dec[7]+dec[8]+dec[9]+dec[10]+dec[11];
                        }
                    
                }
            }
            else {
                ret = CAENVME_ReadCycle(BHandle,BoardAddress,&man.buff,cvA32_U_DATA,cvD32); // It was ""(char *)man.buff""
                PrintStatus(ret);
                
                time (&end_countdown);
                dif = difftime (end_countdown,start_countdown);
                if (dif>10) {
                    printf("DAQ halted... no trigger");
                    exit(-1);
                }
                
                d32=man.buff;
                
                sprintf(hexx,"%x",d32);                    // Trasforming DATA ulong into STRING HEX
                
                for (ii=0;ii<8;ii++) {
                    k=(hexx[ii]<='9' && hexx[ii]>='0' ? hexx[ii]-'0' : hexx[ii]-'a'+10);
                    strcpy(bin+4*ii,h2b[k]);
                }
                
                
                
                if (bin[5]=='0' && bin[6]=='1' && bin[7]=='0') {                    // This is Begin of Block
                    for (m=0;m<16;m++) {                                        // Initializing charge
                        charge[m]=notfound;
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
                    strftime(buf, sizeof(buf), "%H%M%S", &ts);
                    
                    event_buffer = 0;
                    for (l=0;l<=23;l++) {                                        // PROCESSING EVENT BUFFER
                        event_buffer = event_buffer + dec_buff[l];
                    }
                    unixtime = static_cast<long int> (now);

// HEX and/or BIN printout
                    if (hexprint) std::cout << " Rawdata HEX=" << hexx;
                    if (binprint) std::cout << " Rawdata BIN=" << bin;
                    if ( (hexprint) || (binprint) ) std::cout << std::endl;

                    if (terminal) printf("%d %d %d %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",event,event_buffer,now,buf,charge[0],charge[1],charge[2],charge[3],charge[4],charge[5],charge[6],charge[7],charge[8],charge[9],charge[10],charge[11],charge[12],charge[13],charge[14],charge[15]);
                    if (rootfile)    {
charge0=charge[0];
charge1=charge[1];
charge2=charge[2];
charge3=charge[3];
charge4=charge[4];
charge5=charge[5];
charge6=charge[6];
charge7=charge[7];
charge8=charge[8];
charge9=charge[9];
charge10=charge[10];
charge11=charge[11];
charge12=charge[12];
charge13=charge[13];
charge14=charge[14];
charge15=charge[15];
                        GGMraw->Fill();
                        //printf("filling");
                    }
                }
                if (bin[5]=='0' && bin[6]=='0' && bin[7]=='0' ) {       //This is valid data
// HEX and/or BIN printout
//                    if (hexprint) std::cout << " Rawdata HEX=" << hexx;
  //                  if (binprint) std::cout << " Rawdata BIN=" << bin;
    //                if ( (hexprint) || (binprint) ) std::cout << std::endl;
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
		if (bin[15]=='1') range=0;
                            if (bin[15]=='0') range=1;
                    charge[number_of_channel]=dec[0]+dec[1]+dec[2]+dec[3]+dec[4]+dec[5]+dec[6]+dec[7]+dec[8]+dec[9]+dec[10]+dec[11];
                    
                }
            }
        }
        if (rootfile)	{
            GGMraw->Write();
            hfile->Close();
            printf("Rootfile File written..");
        }
    }
}


void PrintStatus(int ret) {
    switch (ret) {
        case cvSuccess   :
            break;
            
        case cvBusError     :
            printf("Bus Error!");
            break;
            
        case cvCommError :
            printf("Comunication Error!");
            break;
            
        default          :
            printf("Unknown error!");
            break ;
    }
}

void PrintStatusIRQ(int ret) {
    
    switch (ret) {
        case cvSuccess   :
            printf("IRQ enabled to level 1\n");
            break;
            
        case cvBusError     :
            printf("Bus Error! IRQ doesn't enabled to any level.");
            break;
            
        case cvCommError :
            printf("Comunication Error! IRQ doesn't enabled to any level.\n");
            break;
            
        default          :
            printf("Unknown error, IRQ doesn't enabled to any level.\n");
            break ;
    }
}

void PrintConnectionStatus(int BHandle) {
    switch (BHandle) {
        case (0) :
            printf("Connection is OK\n");
            break;
            
        default :
            printf("Connection is NOT OK\n");
            exit(-1);
            break ;
    }
    
}
