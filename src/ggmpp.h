//
//  ggmpp.h
//  
//
//  Created by Stefano Colafranceschi on 11/28/20.
//
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "CAENVMElib.h"
#include <time.h>
#include "console.h"


    // Declaration board/chip variables (default)
    CVBoardTypes          VMEBoard; //=cvV2718;
    short                 Link;
    short                 Device;
    short                 dd;
    long                  BHandle;
    int                  nb;
    CVAddressModifier    am;
    CVErrorCodes        ret;

    typedef struct man_par
    {
        ulong        addr ;            // Address
        ulong        data ;              // Data
        ushort       level ;            // Interrupt level
        uchar        irqstat ;        // IRQ status
        ushort        am ;            // Addressing Mode
        CVDataWidth     dtsize ;        // Data Format
        ulong        basaddr ;        // Base Address
        ulong        blts ;            // Block size for blt (bytes)
        ushort        ncyc ;            // Number of cycles
        ushort        autoinc ;        // Auto increment address
        ulong        *buff ;             // Memory buffer for blt
    } man_par_t ;
  
    // Declaration C variables
    //char hex[34][8], hexx[8];
    char ch, buf[80]; // filel[100], filerun[100];
    std::string filel;
    //char bin[33], buf2[80];
    //char h2b[16][5]={"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"};
    //int number_of_channel, 
    //int event_buffer, starting = 0, 
int Board;
    int event = 1, rem = 0; // dec[14], eventmonitor;
    //int cost[23], dec_buff[24], 
    int BLTsize;
    int** charge;
    double** GGMResults;
    ulong *d32, data;
    double hv[16], dif;
    int jj = 1, h, EventsToRead; //l,ii,k,m
    int events = 50, spychannel = 2, num_events = 10000;
    int  NumberOfRuns;
    bool printhv, printconfiguration, printdatafile, analysis=false;
    std::string recipient, sBoardAddress, DataFolder, hvmode;
    std::string configFile = "config.ini";

    double charge0hv,charge1hv,charge2hv,charge3hv,charge4hv,charge5hv,charge6hv,charge7hv,charge8hv,charge9hv,charge10hv,charge11hv,charge12hv,charge13hv,charge14hv,charge15hv;
    int charge0,charge1,charge2,charge3,charge4,charge5,charge6,charge7,charge8,charge9,charge10,charge11,charge12,charge13,charge14,charge15, range;
    double avalanche0,avalanche1,avalanche2,avalanche3,avalanche4,avalanche5,avalanche6,avalanche7,avalanche8,avalanche9,avalanche10,avalanche11,avalanche12,avalanche13,avalanche14,avalanche15;
    double streamer0,streamer1,streamer2,streamer3,streamer4,streamer5,streamer6,streamer7,streamer8,streamer9,streamer10,streamer11,streamer12,streamer13,streamer14,streamer15;
    double ratio0,ratio1,ratio2,ratio3,ratio4,ratio5,ratio6,ratio7,ratio8,ratio9,ratio10,ratio11,ratio12,ratio13,ratio14,ratio15;
    double efficiency0,efficiency1,efficiency2,efficiency3,efficiency4,efficiency5,efficiency6,efficiency7,efficiency8,efficiency9,efficiency10,efficiency11,efficiency12,efficiency13,efficiency14,efficiency15;
    double chisquare0,chisquare1,chisquare2,chisquare3,chisquare4,chisquare5,chisquare6,chisquare7,chisquare8,chisquare9,chisquare10,chisquare11,chisquare12,chisquare13,chisquare14,chisquare15;

    long int unixtime;

    FILE *p;
    std::string temporary="";
    std::string rpcmachine= "cms_rpc_dcs_1";
    std::string xdaqcommand = "/opt/xdaq/bin/dpGet.sh";
    std::string psxmachine= "http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30";
ulong BoardAddress;
// int dec1, dec2, dec3, dec4;
