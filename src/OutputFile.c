#include <string.h>
#include <iostream>
#include <fstream>
#include "ReadConf.h"

std::string OutputFile(ReadConf myconfiguration) {

    // Setting the RUN number and unixtime timestamp
    std::string filel, filerun;
    char buf[80];
    time_t now;
    struct tm  ts;
    time(&now);

    ts = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y%m%d-%H%M", &ts);

    // Preparing output files
    filel = myconfiguration.DataFolder + myconfiguration.FileNamePrefix + myconfiguration.FileNameSuffix + buf + ".root";
    filerun = myconfiguration.DataFolder + "running.txt";
      
    std::ofstream myfile;
    myfile.open (filerun);
    myfile << myconfiguration.FileNamePrefix << myconfiguration.FileNameSuffix << buf << ".root";
    myfile.close();

    if (myconfiguration.PrintDataFile) std::cout << "File being written: " << filel << "\n" << std::endl;
    return filel;
}
