#include <string.h>
#include <iostream>
#include <fstream>
#include <TTree.h>
#include <TBranch.h>
#include "DataStructure.h"

TTree* GGMDataStructure() {
    TTree *GGMraw = new TTree("GGMraw","GGMraw");

    GGMraw->Branch("event",&event,"event/I");
    GGMraw->Branch("range",&range,"range/I");
    GGMraw->Branch("timestamp",&unixtime,"unixtime/L");
    GGMraw->Branch("channel0",&charge0,"charge0/I");
    GGMraw->Branch("channel1",&charge1,"charge1/I");
    GGMraw->Branch("channel2",&charge2,"charge2/I");
    GGMraw->Branch("channel3",&charge3,"charge3/I");
    GGMraw->Branch("channel4",&charge4,"charge4/I");
    GGMraw->Branch("channel5",&charge5,"charge5/I");
    GGMraw->Branch("channel6",&charge6,"charge6/I");
    GGMraw->Branch("channel7",&charge7,"charge7/I");
    GGMraw->Branch("channel8",&charge8,"charge8/I");
    GGMraw->Branch("channel9",&charge9,"charge9/I");
    GGMraw->Branch("channel10",&charge10,"charge10/I");
    GGMraw->Branch("channel11",&charge11,"charge11/I");
    GGMraw->Branch("channel12",&charge12,"charge12/I");
    GGMraw->Branch("channel13",&charge13,"charge13/I");
    GGMraw->Branch("channel14",&charge14,"charge14/I");
    GGMraw->Branch("channel15",&charge15,"charge15/I");

    GGMraw->Branch("channel0hv",&charge0hv,"charge0hv/D");
    GGMraw->Branch("channel1hv",&charge1hv,"charge1hv/D");
    GGMraw->Branch("channel2hv",&charge2hv,"charge2hv/D");
    GGMraw->Branch("channel3hv",&charge3hv,"charge3hv/D");
    GGMraw->Branch("channel4hv",&charge4hv,"charge4hv/D");
    GGMraw->Branch("channel5hv",&charge5hv,"charge5hv/D");
    GGMraw->Branch("channel6hv",&charge6hv,"charge6hv/D");
    GGMraw->Branch("channel7hv",&charge7hv,"charge7hv/D");
    GGMraw->Branch("channel8hv",&charge8hv,"charge8hv/D");
    GGMraw->Branch("channel9hv",&charge9hv,"charge9hv/D");
    GGMraw->Branch("channel10hv",&charge10hv,"charge10hv/D");
    GGMraw->Branch("channel11hv",&charge11hv,"charge11hv/D");
    GGMraw->Branch("channel12hv",&charge12hv,"charge12hv/D");
    GGMraw->Branch("channel13hv",&charge13hv,"charge13hv/D");
    GGMraw->Branch("channel14hv",&charge14hv,"charge14hv/D");
    GGMraw->Branch("channel15hv",&charge15hv,"charge15hv/D");
    return GGMraw;
}
