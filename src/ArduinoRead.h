#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include <unistd.h>

    char readBuffer[1024];
    std::string stringIn, s;
    std::string values[16];
    std::string mode;
    int arduino1 [16];
    int arduino2 [16];
    FILE* serPort;
