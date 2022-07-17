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

double* ArduinoRead();

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
        //try {
            GGMResults = PrintHistograms(myconfiguration, filel);
            std::cout << " DONE " << std::endl;
        //}
        //catch (...)  {
        //    std::cout << "Default Exception" << std::endl;
       // }
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

        uint16_t Iped = myconfiguration.IPED;
        write_reg(BHandle,BoardAddress,0x1060, Iped);  // Set pedestal
        write_reg(BHandle,BoardAddress, 0x1010, 0x60);  // enable BERR to close BLT at and of block
write_reg(BHandle,BoardAddress, 0x10BC, 0xf00);
write_reg(BHandle,BoardAddress, 0x10BE, 0xf00);
/*
        // Set LLD (low level threshold for ADC data)	
        write_reg(BHandle,BoardAddress,0x1034, 0x100);  // set threshold step = 16
        for(int i=0; i<16; i++) {
            write_reg(BHandle,BoardAddress,0x1080 + i*4, 0);
        }
*/
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
        	event = -1;
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
        GGMraw->Branch("pedchannel0",&pedcharge0,"pedcharge0/I");
        GGMraw->Branch("pedchannel1",&pedcharge1,"pedcharge1/I");
        GGMraw->Branch("pedchannel2",&pedcharge2,"pedcharge2/I");
        GGMraw->Branch("pedchannel3",&pedcharge3,"pedcharge3/I");
        GGMraw->Branch("pedchannel4",&pedcharge4,"pedcharge4/I");
        GGMraw->Branch("pedchannel5",&pedcharge5,"pedcharge5/I");
        GGMraw->Branch("pedchannel6",&pedcharge6,"pedcharge6/I");
        GGMraw->Branch("pedchannel7",&pedcharge7,"pedcharge7/I");
        GGMraw->Branch("pedchannel8",&pedcharge8,"pedcharge8/I");
        GGMraw->Branch("pedchannel9",&pedcharge9,"pedcharge9/I");
        GGMraw->Branch("pedchannel10",&pedcharge10,"pedcharge10/I");
        GGMraw->Branch("pedchannel11",&pedcharge11,"pedcharge11/I");
        GGMraw->Branch("pedchannel12",&pedcharge12,"pedcharge12/I");
        GGMraw->Branch("pedchannel13",&pedcharge13,"pedcharge13/I");
        GGMraw->Branch("pedchannel14",&pedcharge14,"pedcharge14/I");
        GGMraw->Branch("pedchannel15",&pedcharge15,"pedcharge15/I");

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

        GGMraw->Branch("ProcessedChannel0",&processedcharge0,"processedcharge0/I");
        GGMraw->Branch("ProcessedChannel1",&processedcharge1,"processedcharge1/I");
        GGMraw->Branch("ProcessedChannel2",&processedcharge2,"processedcharge2/I");
        GGMraw->Branch("ProcessedChannel3",&processedcharge3,"processedcharge3/I");
        GGMraw->Branch("ProcessedChannel4",&processedcharge4,"processedcharge4/I");
        GGMraw->Branch("ProcessedChannel5",&processedcharge5,"processedcharge5/I");
        GGMraw->Branch("ProcessedChannel6",&processedcharge6,"processedcharge6/I");
        GGMraw->Branch("ProcessedChannel7",&processedcharge7,"processedcharge7/I");
        GGMraw->Branch("ProcessedChannel8",&processedcharge8,"processedcharge8/I");
        GGMraw->Branch("ProcessedChannel9",&processedcharge9,"processedcharge9/I");
        GGMraw->Branch("ProcessedChannel10",&processedcharge10,"processedcharge10/I");
        GGMraw->Branch("ProcessedChannel11",&processedcharge11,"processedcharge11/I");
        GGMraw->Branch("ProcessedChannel12",&processedcharge12,"processedcharge12/I");
        GGMraw->Branch("ProcessedChannel13",&processedcharge13,"processedcharge13/I");
        GGMraw->Branch("ProcessedChannel14",&processedcharge14,"processedcharge14/I");
        GGMraw->Branch("ProcessedChannel15",&processedcharge15,"processedcharge15/I");

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

        GGMraw->Branch("temperatureFromFresh",&temperatureFromFresh,"temperatureFromFresh/D");
        GGMraw->Branch("temperatureToFresh",&temperatureToFresh,"temperatureToFresh/D");
        GGMraw->Branch("temperatureFromBefore",&temperatureFromBefore,"temperatureFromBefore/D");
        GGMraw->Branch("temperatureToBefore",&temperatureToBefore,"temperatureToBefore/D");
        GGMraw->Branch("temperatureFromAfter",&temperatureFromAfter,"temperatureFromAfter/D");
        GGMraw->Branch("temperatureToAfter",&temperatureToAfter,"temperatureToAfter/D");
        GGMraw->Branch("temperatureAmbient",&temperatureAmbient,"temperatureAmbient/D");

        GGMraw->Branch("pressureFromFresh",&pressureFromFresh,"pressureFromFresh/D");
        GGMraw->Branch("pressureToFresh",&pressureToFresh,"pressureToFresh/D");
        GGMraw->Branch("pressureFromBefore",&pressureFromBefore,"pressureFromBefore/D");
        GGMraw->Branch("pressureToBefore",&pressureToBefore,"pressureToBefore/D");
        GGMraw->Branch("pressureFromAfter",&pressureFromAfter,"pressureFromAfter/D");
        GGMraw->Branch("pressureToAfter",&pressureToAfter,"pressureToAfter/D");
        GGMraw->Branch("pressureAmbient",&pressureAmbient,"pressureAmbient/D");

        GGMraw->Branch("humidityFromFresh",&humidityFromFresh,"humidityFromFresh/D");
        GGMraw->Branch("humidityToFresh",&humidityToFresh,"humidityToFresh/D");
        GGMraw->Branch("humidityFromBefore",&humidityFromBefore,"humidityFromBefore/D");
        GGMraw->Branch("humidityToBefore",&humidityToBefore,"humidityToBefore/D");
        GGMraw->Branch("humidityFromAfter",&humidityFromAfter,"humidityFromAfter/D");
        GGMraw->Branch("humidityToAfter",&humidityToAfter,"humidityToAfter/D");
        GGMraw->Branch("humidityAmbient",&humidityAmbient,"humidityAmbient/D");

                // Preparing output files
                filel = OutputFile(myconfiguration);                

                // Opening the root data file
                TFile *hfile = 0;
                if (myconfiguration.rootfile) hfile = TFile::Open(filel.c_str(),"RECREATE");

                if ( (myconfiguration.info) && (myconfiguration.DaqMode =="Physics") ) printf("Reading the ADC controller...\n");
                if ( (myconfiguration.info) && (myconfiguration.DaqMode =="Montecarlo") ) printf("Simulating the ADC controller...\n");

                // Data taking routine
                pedestal = 0;
                pedestal = new int*[2];
                pedestal[0] = new int[16];
                pedestal[1] = new int[16];

                for (int m=0;m<16;m++) {                                
                    pedestal[0][m] = 0;
                    pedestal[1][m] = 0;
                }
lowpedcharge0=0;
lowpedcharge1=0;
lowpedcharge2=0;
lowpedcharge3=0;
lowpedcharge4=0;
lowpedcharge5=0;
lowpedcharge6=0;
lowpedcharge7=0;
lowpedcharge8=0;
lowpedcharge9=0;
lowpedcharge10=0;
lowpedcharge11=0;
lowpedcharge12=0;
lowpedcharge13=0;
lowpedcharge14=0;
lowpedcharge15=0;
highpedcharge0=0;
highpedcharge1=0;
highpedcharge2=0;
highpedcharge3=0;
highpedcharge4=0;
highpedcharge5=0;
highpedcharge6=0;
highpedcharge7=0;
highpedcharge8=0;
highpedcharge9=0;
highpedcharge10=0;
highpedcharge11=0;
highpedcharge12=0;
highpedcharge13=0;
highpedcharge14=0;
highpedcharge15=0;
bool FoundPed = true;

                if ( myconfiguration.RemovePed) FoundPed = false;

                while (event < myconfiguration.events) {
                    if ( (myconfiguration.DaqMode) == "Physics" ) charge = PayloadProcess(myconfiguration, BHandle, BoardAddress);
                    if ( (myconfiguration.DaqMode) == "Montecarlo" ) charge = PayloadSimulator(myconfiguration, ran);

                    // Assign the pedestal values every event
                    if ( myconfiguration.RemovePed && (charge[1][myconfiguration.SpyChannel] > 3800) ) {
                            FoundPed = true;
 
                            highpedcharge0=charge[0][myconfiguration.gap0];
                            highpedcharge1=charge[0][myconfiguration.gap1];
                            highpedcharge2=charge[0][myconfiguration.gap2];
                            highpedcharge3=charge[0][myconfiguration.gap3];
                            highpedcharge4=charge[0][myconfiguration.gap4];
                            highpedcharge5=charge[0][myconfiguration.gap5];
                            highpedcharge6=charge[0][myconfiguration.gap6];
                            highpedcharge7=charge[0][myconfiguration.gap7];
                            highpedcharge8=charge[0][myconfiguration.gap8];
                            highpedcharge9=charge[0][myconfiguration.gap9];
                            highpedcharge10=charge[0][myconfiguration.gap10];
                            highpedcharge11=charge[0][myconfiguration.gap11];
                            highpedcharge12=charge[0][myconfiguration.gap12];
                            highpedcharge13=charge[0][myconfiguration.gap13];
                            highpedcharge14=charge[0][myconfiguration.gap14];
                            highpedcharge15=charge[0][myconfiguration.gap15];
                    
                            lowpedcharge0=charge[1][myconfiguration.gap0];
                            lowpedcharge1=charge[1][myconfiguration.gap1];
                            lowpedcharge2=charge[1][myconfiguration.gap2];
                            lowpedcharge3=charge[1][myconfiguration.gap3];
                            lowpedcharge4=charge[1][myconfiguration.gap4];
                            lowpedcharge5=charge[1][myconfiguration.gap5];
                            lowpedcharge6=charge[1][myconfiguration.gap6];
                            lowpedcharge7=charge[1][myconfiguration.gap7];
                            lowpedcharge8=charge[1][myconfiguration.gap8];
                            lowpedcharge9=charge[1][myconfiguration.gap9];
                            lowpedcharge10=charge[1][myconfiguration.gap10];
                            lowpedcharge11=charge[1][myconfiguration.gap11];
                            lowpedcharge12=charge[1][myconfiguration.gap12];
                            lowpedcharge13=charge[1][myconfiguration.gap13];
                            lowpedcharge14=charge[1][myconfiguration.gap14];
                            lowpedcharge15=charge[1][myconfiguration.gap15];
                    }
                    else {
                    if (FoundPed) {
                        FoundPed = false;
                        event = event + 1;
                        rem = rem + 1;
                        rem2 = rem2 + 1;
                        unixtime = static_cast<long int> (now);

                        if ( event == 1 ) {
                           time(&now);
                           tempo1 = now;
                        }

                        if (rem == myconfiguration.eventmonitor) {
                            if (myconfiguration.eventmonitor!=666) printf("\nEvent number: %d\n\n\n",(event-1) );
                            rem = 0;
                        }

                        if (rem2 == myconfiguration.rateCheck) {
                            // Rate
                            time(&now);
                            tempo2 = now;
                            rate = ( tempo2 - tempo1 );
                            time(&now);
                            tempo1 = now;
                            rate = myconfiguration.rateCheck / rate;
                            
                            //std::cout << " The measured rate is : " << rate << std::endl;
                            command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":rpc_ggm_trg0.rate:_original.._value " + std::to_string(rate) + " " + myconfiguration.psxmachine;
                            system(command.c_str());
                            rem2 = 0;
                        }

                        //std::cout << man.data << std::endl;

                        // Rootfile filling
                        if ( (myconfiguration.rootfile) && (event>=1) ) {
                            range = 0;
                            charge0=charge[0][myconfiguration.gap0];
			    processedcharge0=charge0-highpedcharge0;

                            charge1=charge[0][myconfiguration.gap1];
			processedcharge1=charge1-highpedcharge1;
                            charge2=charge[0][myconfiguration.gap2];
			 processedcharge2=charge2-highpedcharge2;
                            charge3=charge[0][myconfiguration.gap3];
			 processedcharge3=charge3-highpedcharge3;
                            charge4=charge[0][myconfiguration.gap4];
			processedcharge4=charge4-highpedcharge4;
                            charge5=charge[0][myconfiguration.gap5];
			processedcharge5=charge5-highpedcharge5;
                            charge6=charge[0][myconfiguration.gap6];
			processedcharge6=charge6-highpedcharge6;
                            charge7=charge[0][myconfiguration.gap7];
			processedcharge7=charge7-highpedcharge7;
                            charge8=charge[0][myconfiguration.gap8];
			processedcharge8=charge8-highpedcharge8;
                            charge9=charge[0][myconfiguration.gap9];
			processedcharge9=charge9-highpedcharge9;
                            charge10=charge[0][myconfiguration.gap10];
			processedcharge10=charge10-highpedcharge10;
                            charge11=charge[0][myconfiguration.gap11];
			processedcharge11=charge11-highpedcharge11;
                            charge12=charge[0][myconfiguration.gap12];
			processedcharge12=charge12-highpedcharge12;
                            charge13=charge[0][myconfiguration.gap13];
			processedcharge13=charge13-highpedcharge13;
                            charge14=charge[0][myconfiguration.gap14];
			processedcharge14=charge14-highpedcharge14;
                            charge15=charge[0][myconfiguration.gap15];
                        processedcharge15=charge15-highpedcharge15;
pedcharge0= highpedcharge0;
pedcharge1= highpedcharge1;
pedcharge2= highpedcharge2;
pedcharge3= highpedcharge3;
pedcharge4= highpedcharge4;
pedcharge5= highpedcharge5;
pedcharge6= highpedcharge6;
pedcharge7= highpedcharge7;
pedcharge8= highpedcharge8;
pedcharge9= highpedcharge9;
pedcharge10= highpedcharge10;
pedcharge11= highpedcharge11;
pedcharge12= highpedcharge12;
pedcharge13= highpedcharge13;
pedcharge14= highpedcharge14;
pedcharge15= highpedcharge15;
                            GGMraw->Fill();

                            range = 1;
                            charge0=charge[1][myconfiguration.gap0];
			processedcharge0=charge0-lowpedcharge0;
                            charge1=charge[1][myconfiguration.gap1];
			processedcharge1=charge1-lowpedcharge1;
                            charge2=charge[1][myconfiguration.gap2];
			processedcharge2=charge2-lowpedcharge2;
                            charge3=charge[1][myconfiguration.gap3];
			processedcharge3=charge3-lowpedcharge3;
                            charge4=charge[1][myconfiguration.gap4];
			processedcharge4=charge4-lowpedcharge4;
                            charge5=charge[1][myconfiguration.gap5];
			processedcharge5=charge5-lowpedcharge5;
                            charge6=charge[1][myconfiguration.gap6];
			processedcharge6=charge6-lowpedcharge6;
                            charge7=charge[1][myconfiguration.gap7];
			processedcharge7=charge7-lowpedcharge7;
                            charge8=charge[1][myconfiguration.gap8];
			processedcharge8=charge8-lowpedcharge8;
                            charge9=charge[1][myconfiguration.gap9];
			processedcharge9=charge9-lowpedcharge9;
                            charge10=charge[1][myconfiguration.gap10];
			processedcharge10=charge10-lowpedcharge10;
                            charge11=charge[1][myconfiguration.gap11];
			processedcharge11=charge11-lowpedcharge11;
                            charge12=charge[1][myconfiguration.gap12];
			processedcharge12=charge12-lowpedcharge12;
                            charge13=charge[1][myconfiguration.gap13];
			processedcharge13=charge13-lowpedcharge13;
                            charge14=charge[1][myconfiguration.gap14];
			processedcharge14=charge14-lowpedcharge14;
                            charge15=charge[1][myconfiguration.gap15];
			processedcharge15=charge15-lowpedcharge15;
pedcharge0= lowpedcharge0;
pedcharge1= lowpedcharge1;
pedcharge2= lowpedcharge2;
pedcharge3= lowpedcharge3;
pedcharge4= lowpedcharge4;
pedcharge5= lowpedcharge5;
pedcharge6= lowpedcharge6;
pedcharge7= lowpedcharge7;
pedcharge8= lowpedcharge8;
pedcharge9= lowpedcharge9;
pedcharge10= lowpedcharge10;
pedcharge11= lowpedcharge11;
pedcharge12= lowpedcharge12;
pedcharge13= lowpedcharge13;
pedcharge14= lowpedcharge14;
pedcharge15= lowpedcharge15;
                            GGMraw->Fill();
                        }
                    }
                    }
                }
                // Reading the sensors
                if (myconfiguration.arduinoEnable) {
                    arduino = ArduinoRead();

                    pressureFromBefore = (arduino[0] + myconfiguration.arduinoOffset) / 32;
                    pressureToAfter = (arduino[1] + myconfiguration.arduinoOffset)/32;
                    pressureFromAfter = (arduino[3] + myconfiguration.arduinoOffset)/32;

                    temperatureFromFresh = ( (2000*(arduino[5]+ myconfiguration.arduinoOffset)) / (2500 - (arduino[5] + myconfiguration.arduinoOffset))-100)*2.6;  		
                    temperatureAmbient = ( (2000*(arduino[6]+ myconfiguration.arduinoOffset)) / (2500 - (arduino[6] + myconfiguration.arduinoOffset))-100)*2.6;
                    temperatureToFresh = ( (2000*(arduino[7]+ myconfiguration.arduinoOffset)) / (2500 - (arduino[7] + myconfiguration.arduinoOffset))-100)*2.6;
                    temperatureToBefore = ( (2000*(arduino[8]+ myconfiguration.arduinoOffset)) / (2500 - (arduino[8] + myconfiguration.arduinoOffset))-100)*2.6;

                    humidityToBefore = 1.51*(arduino[9] + myconfiguration.arduinoOffset)/10-1;
                    humidityAmbient = 1.51*(arduino[11] + myconfiguration.arduinoOffset)/10-1;
                    humidityFromFresh = 1.51*(arduino[13] + myconfiguration.arduinoOffset)/10-1;
                    humidityToFresh = 1.51*(arduino[15] + myconfiguration.arduinoOffset)/10-1;

       	       	    temperatureFromAfter = ( (2000*(arduino[0+16] + myconfiguration.arduinoOffset)) / (2500-(arduino[0+16]+ myconfiguration.arduinoOffset))-100)*2.6;
                    temperatureFromBefore = ( (2000*(arduino[1+16] + myconfiguration.arduinoOffset)) / (2500-(arduino[1+16]+ myconfiguration.arduinoOffset))-100)*2.6;
                    temperatureToAfter = ( (2000*(arduino[2+16] + myconfiguration.arduinoOffset)) / (2500-(arduino[2+16]+ myconfiguration.arduinoOffset))-100)*2.6;

                    pressureFromFresh = (arduino[4+16] + myconfiguration.arduinoOffset)/32;
                    pressureToBefore = (arduino[6+16] + myconfiguration.arduinoOffset)/32;
                    pressureToFresh = (arduino[8+16] + myconfiguration.arduinoOffset)/32;
                    pressureAmbient = 1000+(arduino[9+16] + myconfiguration.arduinoOffset)/32;

                    command = myconfiguration.xdaqcommand + " " + myconfiguration.rpcmachine + ":Environmental_SGAtmoPressure.value:_online.._value " + myconfiguration.psxmachine + " | grep -oP '(?<=<psx:dp name=\"" + myconfiguration.rpcmachine + ":Environmental_SGAtmoPressure.value:_online.._value\">)[^<]*'";

                    temporary = "";
                    p = popen(command.c_str(),"r");
                    while( (ch=fgetc(p)) != EOF)
                    //putchar(ch);
                    temporary = temporary + ch;
                    pclose(p);
                    //std::cout << temporary << std::endl;

                    try {
                        if (!temporary.empty()) {
                            pressureAmbient = std::stod(temporary);
                        }
                        else {
                            pressureAmbient = -1;
                        }
                    } catch (... ) {
                       pressureAmbient=0;
                       throw;
                    }

                    command = myconfiguration.xdaqcommand + " " + myconfiguration.rpcmachine + ":Environmental_SGAtmoPressure.value:_online.._value " + myconfiguration.psxmachine + " | grep -oP '(?<=<psx:dp name=\"" + myconfiguration.rpcmachine + ":Environmental_SGAtmoPressure.value:_online.._value\">)[^<]*'";

                    temporary = "";
                    p = popen(command.c_str(),"r");
                    while( (ch=fgetc(p)) != EOF)
                    //putchar(ch);
                    temporary = temporary + ch;
                    pclose(p);
                    //std::cout << temporary << std::endl;

                    try {
                        if (!temporary.empty()) {
                            pressureAmbient = std::stod(temporary);
                        }
                        else {
                            pressureAmbient = -1;
                        }
                    } catch (... ) {
                       pressureAmbient=0;
                       throw;
                    }

                    command = myconfiguration.xdaqcommand + " " + myconfiguration.rpcmachine + ":Environmental_SGAtmoPressure.value:_online.._value " + myconfiguration.psxmachine + " | grep -oP '(?<=<psx:dp name=\"" + myconfiguration.rpcmachine + ":Environmental_SGAtmoPressure.value:_online.._value\">)[^<]*'";

                    temporary = "";
                    p = popen(command.c_str(),"r");
                    while( (ch=fgetc(p)) != EOF)
                    //putchar(ch);
                    temporary = temporary + ch;
                    pclose(p);
                    //std::cout << temporary << std::endl;

                    try {
                        if (!temporary.empty()) {
                            pressureAmbient = std::stod(temporary);
                        }
                        else {
                            pressureAmbient = -1;
                        }
                    } catch (... ) {
                       pressureAmbient=0;
                       throw;
                    }


                    humidityFromAfter = (arduino[11+16] + myconfiguration.arduinoOffset)/10;
                    humidityToAfter = (arduino[13+16] + myconfiguration.arduinoOffset)/10;
                    humidityFromBefore = (arduino[15+16] + myconfiguration.arduinoOffset)/10;

                    GGMraw->Fill();
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

                        try {
                            if (!temporary.empty()) {
                                hv[i+1] = std::stod(temporary);
                            }
                            else {
                                hv[i+1] = -1;
                            }
                        } catch (... ) {
                           hv[i+1]=0;
                           throw;
                        }
                    }
                }
                //charge0hv = hv[0];
                //charge1hv = hv[1];
                charge2hv = hv[2];
                charge3hv = hv[3];
                charge4hv = hv[4];
                charge5hv = hv[5];
                charge6hv = hv[6];
                charge7hv = hv[7];
                charge8hv = hv[8];
                charge9hv = hv[9];
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
                    GGMraw->Write();
                    if (myconfiguration.info) printf("Rootfile File written..");
                    hfile->Close();
                    GGMraw->Delete();

                    if (myconfiguration.QuickHisto!="off") GGMResults = PrintHistograms(myconfiguration, filel);

                    if ( (myconfiguration.SendToPVSS) && (myconfiguration.DaqEnable == 1) && (myconfiguration.DaqMode =="Physics") ) {
                        std::string command;

                        gapPos[1] = 2;myconfiguration.gap2;
			gapPos[2] = 3;myconfiguration.gap3;
			gapPos[3] = 4;myconfiguration.gap4;
			gapPos[4] = 5;myconfiguration.gap5;
			gapPos[5] = 6;myconfiguration.gap6;
			gapPos[6] = 7;myconfiguration.gap7;
			gapPos[7] = 8;myconfiguration.gap8;
			gapPos[8] = 9;myconfiguration.gap9;

                        for (int i = 1; i <= 8; i++) {
                            std::string command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":rpc_ggm_ch0" + std::to_string(static_cast<long long>(i)) + ".daq.avalanche:_original.._value " + std::to_string(GGMResults[0][ gapPos[i] ]) + " " + myconfiguration.psxmachine;
        		            ///opt/xdaq/bin/dpSet.sh cms_rpc_dcs_1:rpc_ggm_ch04.daq.avalanche:_original.._value 100 http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30
                            //std::cout << "XDaq Send Command : " << command << std::endl;
                            system(command.c_str());
                            command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":rpc_ggm_ch0" + std::to_string(static_cast<long long>(i)) + ".daq.streamer:_original.._value " + std::to_string(GGMResults[1][ gapPos[i] ]) + " " + myconfiguration.psxmachine;
                            system(command.c_str());
                            command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":rpc_ggm_ch0" + std::to_string(static_cast<long long>(i)) + ".daq.ratio:_original.._value " + std::to_string(GGMResults[2][ gapPos[i] ]) + " " + myconfiguration.psxmachine;
                            system(command.c_str());
                            command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":rpc_ggm_ch0" + std::to_string(static_cast<long long>(i)) + ".daq.efficiency:_original.._value " + std::to_string(GGMResults[3][ gapPos[i] ]) + " " + myconfiguration.psxmachine;
                            system(command.c_str());
                            //command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":rpc_ggm_ch0" + std::to_string(static_cast<long long>(i)) + ".daq.chi2square:_original.._value " + std::to_string(GGMResults[4][ gapPos[i] ]) + " " + myconfiguration.psxmachine;
                            //system(command.c_str());
                            command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":rpc_ggm_ch0" + std::to_string(static_cast<long long>(i)) + ".daq.charge:_original.._value " + std::to_string(GGMResults[5][ gapPos[i] ]) + " " + myconfiguration.psxmachine;
                            system(command.c_str());

                            if (i>=3) {
                               std::string command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":rpc_ggm_ch0" + std::to_string(static_cast<long long>(i)) + ".daq.WP.avalanche:_original.._value " + std::to_string(GGMResults[0][ gapPos[i]] / GGMResults[0][ myconfiguration.ReferenceGap ] ) + " " + myconfiguration.psxmachine;
                               system(command.c_str());

                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":rpc_ggm_ch0" + std::to_string(static_cast<long long>(i)) + ".daq.WP.streamer:_original.._value " + std::to_string(GGMResults[1][ gapPos[i] ]/ GGMResults[1][ myconfiguration.ReferenceGap ]) + " " + myconfiguration.psxmachine;
                               system(command.c_str());                              

                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":rpc_ggm_ch0" + std::to_string(static_cast<long long>(i)) + ".daq.WP.charge:_original.._value " + std::to_string(GGMResults[5][ gapPos[i] ]/ GGMResults[5][ myconfiguration.ReferenceGap ]) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                            }

                           if (myconfiguration.arduinoEnable) {
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.pressure_before_A:_original.._value " + std::to_string(pressureFromBefore) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.pressure_after_B:_original.._value " + std::to_string(pressureToAfter) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.pressure_after_A:_original.._value " + std::to_string(pressureFromAfter) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.temperature_fresh_A:_original.._value " + std::to_string(temperatureFromFresh) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.temperature_box:_original.._value " + std::to_string(temperatureAmbient) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.temperature_fresh_B:_original.._value " + std::to_string(temperatureToFresh) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.temperature_before_B:_original.._value " + std::to_string(temperatureToBefore) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.humidity_before_B:_original.._value " + std::to_string(humidityToBefore) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.humidity_box:_original.._value " + std::to_string(humidityAmbient) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.humidity_fresh_A:_original.._value " + std::to_string(humidityFromFresh) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.humidity_fresh_B:_original.._value " + std::to_string(humidityToFresh) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.temperature_after_A:_original.._value " + std::to_string(temperatureFromAfter) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.temperature_before_A:_original.._value " + std::to_string(temperatureFromBefore) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.temperature_after_B:_original.._value " + std::to_string(temperatureToAfter) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.pressure_fresh_A:_original.._value " + std::to_string(pressureFromFresh) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.pressure_before_B:_original.._value " + std::to_string(pressureToBefore) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.pressure_fresh_B:_original.._value " + std::to_string(pressureToFresh) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.pressure_box:_original.._value " + std::to_string(pressureAmbient) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.humidity_before_A:_original.._value " + std::to_string(humidityFromAfter) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.humidity_after_B:_original.._value " + std::to_string(humidityToAfter) + " " + myconfiguration.psxmachine;
                               system(command.c_str());
                               command = myconfiguration.XDaqSendCommand + " " + myconfiguration.rpcmachine + ":sensors.humidity_before_A:_original.._value " + std::to_string(humidityFromBefore) + " " + myconfiguration.psxmachine;
                               system(command.c_str());


/*

                    pressureFromFresh = (arduino[4+16] + myconfiguration.arduinoOffset)/32;
                    pressureToBefore = (arduino[6+16] + myconfiguration.arduinoOffset)/32;
                    pressureToFresh = (arduino[8+16] + myconfiguration.arduinoOffset)/32;
                    pressureAmbient = (arduino[9+16] + myconfiguration.arduinoOffset)/32;

                    humidityFromAfter = (arduino[11+16] + myconfiguration.arduinoOffset)/10;
                    humidityToAfter = (arduino[13+16] + myconfiguration.arduinoOffset)/10;
                    humidityFromBefore = (arduino[15+16] + myconfiguration.arduinoOffset)/10;
*/
                        }


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

for (int kk=0; kk<=5; kk++) {
  for (int ll=0; ll<=15; ll++) {
    if ( isinf(GGMResults[kk][ll]) ) GGMResults[kk][ll] = 0;
  }
}
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

                        charge0 = GGMResults[5][0];
                        charge1 = GGMResults[5][1];
                        charge2 = GGMResults[5][2];
                        charge3 = GGMResults[5][3];
                        charge4 = GGMResults[5][4];
                        charge5 = GGMResults[5][5];
                        charge6 = GGMResults[5][6];
                        charge7 = GGMResults[5][7];
                        charge8 = GGMResults[5][8];
                        charge9 = GGMResults[5][9];
                        charge10 = GGMResults[5][10];
                        charge11 = GGMResults[5][11];
                        charge12 = GGMResults[5][12];
                        charge13 = GGMResults[5][13];
                        charge14 = GGMResults[5][14];
                        charge15 = GGMResults[5][15];
                    }

                    newtree->Fill();	 
                    newtree->Write();
                    newfile->Close();

/*
   TFile *hfiledst = 0;
   filedst = myconfiguration.DataFolder + "dst.root";
   hfiledst = TFile::Open(filedst.c_str(),"update");

   // Tree and Branch definitions
   TTree *GGMDST = new TTree("GGMDST","GGMDST");
   GGMDST->Branch("unixtime",&unixtime,"unixtime/L");
   GGMDST->Branch("avalanche2",&avalanche2,"avalanche2/D");
   GGMDST->Branch("avalanche3",&avalanche3,"avalanche3/D");
   GGMDST->Branch("avalanche4",&avalanche4,"avalanche4/D");
   GGMDST->Branch("avalanche5",&avalanche5,"avalanche5/D");
   GGMDST->Branch("avalanche6",&avalanche6,"avalanche6/D");
   GGMDST->Branch("avalanche7",&avalanche7,"avalanche7/D");
   GGMDST->Branch("avalanche8",&avalanche8,"avalanche8/D");
   GGMDST->Branch("avalanche9",&avalanche9,"avalanche9/D");
   GGMDST->Branch("streamer2",&streamer2,"streamer2/D");
   GGMDST->Branch("streamer3",&streamer3,"streamer3/D");
   GGMDST->Branch("streamer4",&streamer4,"streamer4/D");
   GGMDST->Branch("streamer5",&streamer5,"streamer5/D");
   GGMDST->Branch("streamer6",&streamer6,"streamer6/D");
   GGMDST->Branch("streamer7",&streamer7,"streamer7/D");
   GGMDST->Branch("streamer8",&streamer8,"streamer8/D");
   GGMDST->Branch("streamer9",&streamer9,"streamer9/D");
   GGMDST->Branch("charge2",&charge2,"charge2/D");
   GGMDST->Branch("charge3",&charge3,"charge3/D");
   GGMDST->Branch("charge4",&charge4,"charge4/D");
   GGMDST->Branch("charge5",&charge5,"charge5/D");
   GGMDST->Branch("charge6",&charge6,"charge6/D");
   GGMDST->Branch("charge7",&charge7,"charge7/D");
   GGMDST->Branch("charge8",&charge8,"charge8/D");
   GGMDST->Branch("charge9",&charge9,"charge9/D");
   GGMDST->Branch("efficiency2",&efficiency2,"efficiency2/D");
   GGMDST->Branch("efficiency3",&efficiency3,"efficiency3/D");
   GGMDST->Branch("efficiency4",&efficiency4,"efficiency4/D");
   GGMDST->Branch("efficiency5",&efficiency5,"efficiency5/D");
   GGMDST->Branch("efficiency6",&efficiency6,"efficiency6/D");
   GGMDST->Branch("efficiency7",&efficiency7,"efficiency7/D");
   GGMDST->Branch("efficiency8",&efficiency8,"efficiency8/D");
   GGMDST->Branch("efficiency9",&efficiency9,"efficiency9/D");
   GGMDST->Branch("chisquare2",&chisquare2,"chisquare2/D");
   GGMDST->Branch("chisquare3",&chisquare3,"chisquare3/D");
   GGMDST->Branch("chisquare4",&chisquare4,"chisquare4/D");
   GGMDST->Branch("chisquare5",&chisquare5,"chisquare5/D");
   GGMDST->Branch("chisquare6",&chisquare6,"chisquare6/D");
   GGMDST->Branch("chisquare7",&chisquare7,"chisquare7/D");
   GGMDST->Branch("chisquare8",&chisquare8,"chisquare8/D");
   GGMDST->Branch("chisquare9",&chisquare9,"chisquare9/D");
   GGMDST->Fill();
   GGMDST->Write();
   hfiledst->Close();
*/
                    std::ofstream outfile;
                    outfile.open("/ggmdata/DATA/database.txt", std::ios_base::app);
                    outfile << unixtime << " " << avalanche2 << " " << avalanche3 << " " << avalanche4 << " " << avalanche5 << " " << avalanche6 << " " << avalanche7 << " " << avalanche8 << " " << avalanche9 << " ";
                    outfile << streamer2 << " " << streamer3 << " " << streamer4 << " " << streamer5 << " " << streamer6 << " " << streamer7 << " " << streamer8 << " " << streamer9 << " " ;
                    outfile << charge2 << " " << charge3 << " " << charge4 << " " << charge5 << " " << charge6 << " " << charge7 << " " << charge8 << " " << charge9 << " ";
                    outfile << efficiency2 << " " << efficiency3 << " " << efficiency4 << " " << efficiency5 << " " << efficiency6 << " " << efficiency7 << " " << efficiency8  << " " << efficiency9 << " ";
                    outfile << chisquare2 << " " << chisquare3 << " " << chisquare4 << " " << chisquare5 << " " << chisquare6 << " " << chisquare7 << " " << chisquare8 << " " << chisquare9 << " ";
                    outfile << hv[2] << " " << hv[3] << " " << hv[4] << " " << hv[5] << " " << hv[6] << " " << hv[7] << " " << hv[8] << " " << hv[9] << std::endl;
                    outfile.close();

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
