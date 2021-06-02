#define _GLIBCXX_USE_C99 1
#include "ReadConf.h"
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <iostream>
#include <fstream>

#include "INIReader.h"

void PrintHistograms(ReadConf myconfiguration, std::string filel);

int main(int argc, char** argv) {

    if (argc==1) {
        ReadConf myconfiguration;
        myconfiguration.InputFile = configFile;
        std::string arg1(argv[1]);
        PrintHistograms(myconfiguration, arg1);
    }
    else {
        std::cout << "Provide which file to be analyzed" << std::endl;
    }
}
