#include <string.h>
#include <iostream>
#include <fstream>
#include "ReadConf.h"
#include <TRandom.h>

int** PayloadSimulator(ReadConf myconfiguration, TRandom *ran) {

    int** charge = 0;
    charge = new int*[2];
    charge[0] = new int[16];
    charge[1] = new int[16];
    int x, bias;
    double offset = 0; // -0.9;
    for (int m=0;m<16;m++) {
       
        bias = int( offset + round( ran->Uniform(0,1) ) );
        //std::cout << bias << std::endl;
        if ( bias == 0 ) {
            x = ran->Landau(900,50);
            //std::cout << "Landau: " << x << std::endl;
        }
        else {
            x = ran->Gaus(2000,250);
            //std::cout << "Gauss: " << x << std::endl;
        }
        charge[0][m] = int( x/8 );
        charge[1][m] = int( x );
    }
    return charge;
}
