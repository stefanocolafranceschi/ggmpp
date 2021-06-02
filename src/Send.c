#include <string.h>
#include <iostream>
#include <fstream>
#include "CAENVMElib.h"
#include "TF1.h"
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
#include "ReadConf.h"

void GGMmail(ReadConf myconfiguration, std::string filel) {
            int MaxCh;
            std::string Attachments, imagesave;

            std::string subject = "GGM automatic feedback system";
            std::string text = "GGM run " + filel + " succesfully ended, attached the configuration file and plots";

            if ( (myconfiguration.ADCboard) == "965" ) MaxCh = 16;
            if ( (myconfiguration.ADCboard) == "965a") MaxCh = 8;

            if (myconfiguration.SendROOT) {
                //if (filel.find(".raw") != std::string::npos) 
                Attachments = Attachments + " -a " + filel;
            }

            if (filel.find(".raw") != std::string::npos) {
                imagesave = filel.replace(filel.find(".raw"), sizeof(".raw") - 1, "_CH");
            }
            else {
                imagesave = filel.replace(filel.find(".root"), sizeof(".root") - 1, "_CH");
            }
 
            for (int i = 0; i < MaxCh; i++) {
                std::stringstream convert;   // stream used for the conversion
                convert << i;
                Attachments = Attachments + " -a " + imagesave + convert.str() + "l.pdf ";
                //Attachments = Attachments + " -a " + imagesave + i + "l.pdf ";
            }
            for (int i = 0; i < MaxCh; i++) {
                std::stringstream convert;   // stream used for the conversion
                convert << i;
                Attachments = Attachments + " -a " + imagesave + convert.str() + "h.pdf ";
                //Attachments = Attachments + " -a " + imagesave + i + "h.pdf ";
            }

            std::string command = "echo \"" + text + "\" | mail -s \"" + subject + "\"" + Attachments + myconfiguration.Recipient;
            //std::cout << command << std::endl;
            system(command.c_str());
}
