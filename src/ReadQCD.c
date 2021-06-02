#include <string.h>
#include <iostream>
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
#include "ReadQCD.h"

void PrintStatus(int ret, bool info);

void PrintStatusIRQ(int ret, bool info);

struct ggmpayload
    {
       int event;
       int range;
       int unixtime;
       int charge[2][16]
    };

ggmpayload ReadQCD(ReadConf myconfiguration, int myBHandle) {

    ggmpayload myggmpayload;

    static  man_par_t    man;
    man.addr = BoardAddress;
    man.level = 1 ;
    man.am = cvA32_U_DATA ;
    man.dtsize = cvD32 ;
    man.basaddr = BoardAddress ;
    man.blts = 256 ;
    man.ncyc = 1 ;
    man.autoinc = 0 ;

    cost[23]=8204288,cost[22]=4102144,cost[21]=2051072,cost[20]=1025536,cost[19]=512768,cost[18]=256384,cost[17]=128192,cost[16]=64096,cost[15]=32048,cost[14]=16024,cost[13]=8012,cost[12]=4096,cost[11]=2048,cost[10]=1024,cost[9]=512,cost[8]=256,cost[7]=128,cost[6]=64,cost[5]=32,cost[4]=16,cost[3]=8,cost[2]=4,cost[1]=2,cost[0]=1;

    // Setting the RUN number and unixtime timestamp
    time_t now;
    struct tm  ts;
    time(&now);

    ts = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y%m%d-%H%M", &ts);


    // Allocate 32K for the software buffer containing data for blt
    man.buff = (ulong *)malloc(16*1024*1024);
    if (man.buff == NULL) {
        printf("Error. Can't allocate memory for BLT buffer\n");
        exit(-1);
    }

    time_t start_countdown,end_countdown;
        
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
            myggmpayload.charge[0][m] = myconfiguration.NotFound;
            myggmpayload.charge[1][m] = myconfiguration.NotFound;
        }  
    
        
        if (myconfiguration.info) printf("Reading the ADC controller...\n");
        while (myggmpayload.event < myconfiguration.events) {
            if (rem == myconfiguration.eventmonitor) {
                if (myconfiguration.eventmonitor!=666) printf("\nEvent number: %d\n\n\n",(myggmpayload.event-1) );
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
                            myggmpayload.charge[0][m] = myconfiguration.NotFound;
                            myggmpayload.charge[1][m] = myconfiguration.NotFound;
                        }
                        starting=1;
                        
                    }
                    if (bin[5]=='1' && bin[6]=='0' && bin[7]=='0' && starting==1) {     //this is end of block
                        
                        
                        myggmpayload.event = myggmpayload.event+1;
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
                        
                        myggmpayload.unixtime = static_cast<long int> (now);
                        
                        // HEX and/or BIN printout
                        if ( (myconfiguration.PayloadHexPrint) || (myconfiguration.PayloadBinPrint) ) std::cout << "PAYLOAD RAW: ";
                        if (myconfiguration.PayloadHexPrint) std::cout << "HEX=" << hex[h] << "      ";
                        if (myconfiguration.PayloadBinPrint) std::cout << "BIN=" << bin;
                        if ( (myconfiguration.PayloadHexPrint) || (myconfiguration.PayloadBinPrint) ) std::cout << std::endl;

                        return myggmpayload;
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
                        if (bin[15]=='1') myggmpayload.range=1;
                        if (bin[15]=='0') myggmpayload.range=0;
                        myggmpayload.charge[myggmpayload.range][number_of_channel]=dec[0]+dec[1]+dec[2]+dec[3]+dec[4]+dec[5]+dec[6]+dec[7]+dec[8]+dec[9]+dec[10]+dec[11];
                    }
                    
                }
            }
            else {
                //ret = CAENVME_ReadCycle(myBHandle,man.addr,&man.data,cvA32_U_DATA,cvD32);
                ret = CAENVME_ReadCycle(myBHandle, BoardAddress,&man.data,cvA32_U_DATA,cvD32); // It was ""(char *)man.buff""
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
                        myggmpayload.charge[0][m] = myconfiguration.NotFound;
                        myggmpayload.charge[1][m] = myconfiguration.NotFound;
                    }
                    starting=1;
                }
                if (bin[5]=='1' && bin[6]=='0' && bin[7]=='0' && starting==1) {     // This is End of Block
                    myggmpayload.event = myggmpayload.event+1;
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
                    myggmpayload.unixtime = static_cast<long int> (now);
                    
                    // HEX and/or BIN printout
                    if (myconfiguration.EOBHexPrint) std::cout << "EOB Rawdata HEX=" << hexx;
                    if (myconfiguration.EOBBinPrint) std::cout << "EOB Rawdata BIN=" << bin;
                    if ( (myconfiguration.EOBHexPrint) || (myconfiguration.EOBBinPrint) ) std::cout << std::endl;
                    
                    return myggmpayload;
                }
                if (bin[5]=='0' && bin[6]=='0' && bin[7]=='0' ) {       //This is valid data
  

                    // High vs Range selector ------
                    if ( (myconfiguration.ADCboard) == "965" ) {
                        if (bin[15]=='1') myggmpayload.range=1;
                        if (bin[15]=='0') myggmpayload.range=0;
                    }
                    else if ( (myconfiguration.ADCboard) == "965a" ) {
                        if (bin[14]=='1') myggmpayload.range=1;
                        if (bin[14]=='0') myggmpayload.range=0;
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

                    myggmpayoad.charge[myggmpayload.range][number_of_channel]=dec[0]+dec[1]+dec[2]+dec[3]+dec[4]+dec[5]+dec[6]+dec[7]+dec[8]+dec[9]+dec[10]+dec[11];
                    if (bin[18]=='1') myggmpayload.charge[myggmpayload.range][number_of_channel]=-1;      // underflow
                    if (bin[19]=='1') myggmpayload.charge[myggmpayload.range][number_of_channel]=5000;    // overflow

                    // HEX and/or BIN printout
                    if ( (myconfiguration.PayloadHexPrint) || (myconfiguration.PayloadBinPrint) ) std::cout << "PAYLOAD RAW: ";
                    if (myconfiguration.PayloadHexPrint) std::cout << "HEX = " << hexx << "    ";
                    if (myconfiguration.PayloadBinPrint) std::cout << "BIN = " << bin  << "    ";
                    if (myconfiguration.PayloadDecPrint) std::cout << "DEC: Range = " << myggmpayload.range << ", Channel = " << number_of_channel << ", <adc> = " << myggmpayload.charge[number_of_channel];
                    if ( (myconfiguration.PayloadHexPrint) || (myconfiguration.PayloadBinPrint) ) std::cout << std::endl;
                }
            }
        }
}
