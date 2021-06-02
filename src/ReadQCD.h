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


struct ggmpayload
{
   int event;
   int range;
   int timestamp;
   mycharge[2][16]
};

    // Declaration board/chip variables (default)
    CVBoardTypes          VMEBoard; //=cvV2718;
    short                 Link;
    short                 Device;
    short                 dd;
    long                  myBHandle;
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
    char ch, hex[34][8], hexx[8], bin[33], buf[80], buf2[80];
    char h2b[16][5]={"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"};
    int number_of_channel, event, event_buffer, starting = 0, Board;
    int myevent = 1, rem = 0, dec[14], eventmonitor;
    int mycharge[2][16], cost[23], dec_buff[24], BLTsize;
    ulong *d32;
    ulong mydata;
    double dif;
    int m, l, ii, k, h, EventsToRead;
    int events = 50, spychannel = 2, num_events = 10000;
    int dec1, dec2, dec3, dec4, NumberOfRuns;
    bool printdatafile, info, printraw, rootfile, multiplereading, hexprint, binprint,irqenabled=false;
    long int myunixtime;
    std::string FileNamePrefix, FileNameSuffix;
    int mycharge0,mycharge1,mycharge2,mycharge3,mycharge4,mycharge5,mycharge6,mycharge7,mycharge8,mycharge9,mycharge10,mycharge11,mycharge12,mycharge13,mycharge14,mycharge15;
    int myrange, UnderFlow, OverFlow;

//    FILE *p;
    ulong BoardAddress;
