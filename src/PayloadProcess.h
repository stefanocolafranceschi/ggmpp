#include "CAENVMElib.h"
#include <time.h>
#include "console.h"

int dec1, dec2, dec3, dec4, dec[14], number_of_channel, myrange, cost[23], dec_buff[24];
int UnderFlow, OverFlow, l, chindex;
//bool PayloadDecPrint, PayloadBinPrint, HeaderBinPrint, EOBBinPrint;
long int myunixtime;
int myevent = 1, eventmonitor, myrem;
int event_buffer, starting = 0, m, ii, k,j;
char bin[33], buf2[80];
char h2b[16][5]={"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"};
char hex[34][8];
ulong   *hexx;
double mydif;
ulong *mydata;
CVErrorCodes        myret;

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

