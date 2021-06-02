#include <string.h>
#include <iostream>
#include <fstream>
#include "CAENVMElib.h"
#include "ReadConf.h"
#include "PayloadProcess.h"

#define MAX_BLT_SIZE        (256*1024)
#define DATATYPE_MASK       0x06000000
#define DATATYPE_HEADER     0x02000000
#define DATATYPE_CHDATA     0x00000000
#define DATATYPE_EOB        0x04000000
#define DATATYPE_FILLER     0x06000000

void PrintStatus(int myret, bool info);

void write_register(long handle, ulong BaseAddress, uint16_t reg_addr, uint16_t data) {
        CVErrorCodes ret;
        ret = CAENVME_WriteCycle(handle, BaseAddress + reg_addr, &data, cvA32_U_DATA, cvD16);
        if(ret != cvSuccess) std::cout << "Cannot write at BaseAddress=" << std::to_string(BaseAddress) << " " << std::to_string(reg_addr) << std::endl;
}

int** PayloadProcess(ReadConf myconfiguration, int BHandle, ulong BoardAddress) {

    uint32_t buffer[MAX_BLT_SIZE/4];// readout buffer (raw data from the board)

    int** charge = 0;
    int bcnt, wcnt, nch, pnt, totnb, DataError=0, nev=0;
    int DataType = DATATYPE_HEADER;

    charge = new int*[2];
    charge[0] = new int[16];
    charge[1] = new int[16];

    for (int m=0;m<16;m++) {                                        // Initializing charge
        charge[0][m] = myconfiguration.NotFound;
        charge[1][m] = myconfiguration.NotFound;
    }

    time_t now;
    time_t start_countdown,end_countdown;

    while (true) {

        // if needed, read a new block of data from the board 
        if ((pnt == wcnt) || ((buffer[pnt] & DATATYPE_MASK) == DATATYPE_FILLER)) {
            CAENVME_FIFOMBLTReadCycle(BHandle, BoardAddress, (char *)buffer, 256, cvA32_U_MBLT, &bcnt);
            if ( (myconfiguration.info) && (bcnt>0)) {
                int b;
                //printf("Read Data Block: size = %d bytes\n", bcnt);
                //for(b=0; b<(bcnt/4); b++)
                    //printf("%2d: %08X\n", b, buffer[b]);
            }
            wcnt = bcnt/4;
            totnb += bcnt;
            pnt = 0;
        }
        if (wcnt == 0)  // no data available
            continue;

        // header
        switch (DataType) {
            case DATATYPE_HEADER :
                if((buffer[pnt] & DATATYPE_MASK) != DATATYPE_HEADER) {
                    if ( myconfiguration.info) printf("Header not found: %08X (pnt=%d)\n", buffer[pnt], pnt);
                    DataError = 1;
                } 
                else {
                if ( (myconfiguration.HeaderHexPrint) ) std::cout << "HEADER RAW: ";
                if (myconfiguration.HeaderHexPrint) std::cout << " HEX = " << std::hex << buffer[pnt] << "    " << std::dec << std::endl;
                

                    nch = (buffer[pnt] >> 8) & 0x3F;
                    chindex = 0;
                    nev++;
                    //memset(ADCdata, 0xFFFF, 32*sizeof(uint16_t));
                    if (nch>0)
                        DataType = DATATYPE_CHDATA;
                    else
                        DataType = DATATYPE_EOB;
                }
                break;

            // Channel data 
            case DATATYPE_CHDATA :
                if((buffer[pnt] & DATATYPE_MASK) != DATATYPE_CHDATA) {
                    if ( myconfiguration.info) printf("Wrong Channel Data: %08X (pnt=%d)\n", buffer[pnt], pnt);
                    DataError = 1;
                } else {
		    j = (int)((buffer[pnt] >> 17) & 0x3F); //(16 channels)

                    myrange = buffer[pnt]>>16 & 0x1;
                    number_of_channel = buffer[pnt]>>17 & 0xF;
                    charge[myrange][number_of_channel] = (int) (buffer[pnt] & 0xFFF);

		    //ADCdata[j] = buffer[pnt] & 0xFFF;
                    if (myconfiguration.PayloadHexPrint) std::cout << " buffer["<< pnt << "] = " << std::hex << buffer[pnt] << std::dec << " CH = "<< number_of_channel << " range = "<< myrange << " charge = " << charge[myrange][number_of_channel] << std::endl;

                    if (chindex == (nch-1)) {
                        DataType = DATATYPE_EOB;
                        if (myconfiguration.PayloadHexPrint) std::cout << " ---------- " << std::endl;
		    }
                    chindex++;                    
                }
                break;

            // EOB
            case DATATYPE_EOB :
                if((buffer[pnt] & DATATYPE_MASK) != DATATYPE_EOB) {
                    if ( myconfiguration.info) printf("EOB not found: %08X (pnt=%d)\n", buffer[pnt], pnt);
                    DataError = 1;
                }
                else {
                    if (myconfiguration.EOBHexPrint) std::cout << "EOB Rawdata =" << std::hex << buffer[pnt] << std::dec << std::endl;
                    DataType = DATATYPE_HEADER;
                    return charge;
                }
                break;
        }
        pnt++;

        if (DataError) {
            pnt = wcnt;
            write_register(BHandle, BoardAddress,0x1032, 0x4);
            write_register(BHandle, BoardAddress,0x1034, 0x4);
            DataType = DATATYPE_HEADER;
            DataError=0;
        }

/*
        //CAENVME_BLTReadCycle(BHandle, BoardAddress, (char *)buffer,256,cvA32_U_DATA,cvD32,&bcnt);
        //CAENVME_FIFOMBLTReadCycle(BHandle, BoardAddress, (char *)buffer, MAX_BLT_SIZE, cvA32_U_MBLT, &bcnt);
        for (int h=0; h<34; h++) {
            sprintf(hex[h],"%x",buffer[h]);
 
            for (ii=0;ii<8;ii++) {            
                k=(hex[h][ii]<='9' && hex[h][ii]>='0' ? hex[h][ii]-'0' : hex[h][ii]-'a'+10);            
                strcpy(bin+4*ii,h2b[k]);       
            }

            if (bin[5]=='0' && bin[6]=='1' && bin[7]=='0') {                    // This is Begin of Block
                if ( (myconfiguration.HeaderHexPrint) || (myconfiguration.HeaderBinPrint) ) std::cout << "HEADER RAW: ";
                if (myconfiguration.HeaderHexPrint) std::cout << " HEX = " << hex[h] << "    ";
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
                myevent = myevent + 1;
                myrem = myrem + 1;
                //std::cout << "AAAAA " << myevent << std::endl;
                for (l=0;l<23;l++) {                                        // PROCESSING EVENT BUFFER
                    if (bin[9+l]=='1') {
                       dec_buff[l]=cost[23-l];
                    }
                    else {
                       dec_buff[l]=0;
                    }
                }

                time_t now;
                struct tm  ts;

                time(&now);
                ts = *localtime(&now);
                strftime(buf2, sizeof(buf2), "%H%M%S", &ts);

                event_buffer = 0;
                for (l=0;l<=23;l++) {                                        // PROCESSING EVENT BUFFER
                    event_buffer = event_buffer + dec_buff[l];
                }
                //myunixtime = static_cast<long int> (now);

                // HEX and/or BIN printout
                if (myconfiguration.EOBHexPrint) std::cout << "EOB Rawdata HEX=" << hexx;
                if (myconfiguration.EOBBinPrint) std::cout << "EOB Rawdata BIN=" << bin;
                if ( (myconfiguration.EOBHexPrint) || (myconfiguration.EOBBinPrint) ) std::cout << std::endl;
                return charge;
            }

            if (bin[5]=='0' && bin[6]=='0' && bin[7]=='0' ) {	//This is valid data

                // High vs Range selector ------
                if ( (myconfiguration.ADCboard) == "965" ) {
                    if (bin[15]=='1') myrange=1;
                    if (bin[15]=='0') myrange=0;
                }
                else if ( (myconfiguration.ADCboard) == "965a" ) {
                    if (bin[14]=='1') myrange=1;
                    if (bin[14]=='0') myrange=0;
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
                        
                charge[myrange][number_of_channel]=dec[0]+dec[1]+dec[2]+dec[3]+dec[4]+dec[5]+dec[6]+dec[7]+dec[8]+dec[9]+dec[10]+dec[11];
                if (bin[18]=='1') charge[myrange][number_of_channel]=-1;	// underflow
                if (bin[19]=='1') charge[myrange][number_of_channel]=5000;    // overflow

                // HEX and/or BIN printout
                if ( (myconfiguration.PayloadHexPrint) || (myconfiguration.PayloadBinPrint) ) std::cout << "PAYLOAD RAW: ";
                if (myconfiguration.PayloadHexPrint) std::cout << "HEX = " << hex[h] << "    ";
                if (myconfiguration.PayloadBinPrint) std::cout << "BIN = " << bin  << "    ";
                if (myconfiguration.PayloadDecPrint) std::cout << "DEC: Range = " << myrange << ", Channel = " << number_of_channel << ", <adc> = " << charge[myrange][number_of_channel];
                if ( (myconfiguration.PayloadHexPrint) || (myconfiguration.PayloadBinPrint) ) std::cout << std::endl;
            }
        }
        */
    }
}
