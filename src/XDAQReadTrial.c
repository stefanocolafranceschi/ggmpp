#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sstream> 

using namespace std;
// compile with g++ -std=c++11 XDAQReadTrial.c
int main() {
  
    FILE *p;
    char ch;
    double hv[8];
    string temporary="";
    string rpcmachine = "cms_rpc_dcs_1";
    string xdaqcommand = "/opt/xdaq/bin/dpGet.sh";
    string psxmachine = "http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30";

    for (int i = 1; i <= 8; i++) {
        string command = xdaqcommand + " " + rpcmachine + ":rpc_ggm_ch0" + std::to_string(i) + ".hv:_online.._value " + psxmachine + " | grep -oP '(?<=<psx:dp name=\"" + rpcmachine + ":rpc_ggm_ch0" + std::to_string(i) + ".hv:_online.._value\">)[^<]*'";
        //std::cout << command;
        temporary="";
        p = popen(command.c_str(),"r");           
        while( (ch=fgetc(p)) != EOF)  
            //putchar(ch);
            temporary = temporary + ch;
        pclose(p);
        //std::cout << temporary << std::endl;

        hv[i-1] = std::stod(temporary);
        std::cout << "Value of hv[" << i << "] = " << hv[i-1] << std::endl;
    }
}
