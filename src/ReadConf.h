#include <string.h>
#include <iostream>
#include "INIReader.h"
#include "CAENVMElib.h"

class ReadConf {

public:
    ReadConf() {
        CVBoardTypes VMEBoard;
    }

    int LoadConfiguration();
    void PrintConfiguration();

    std::string HistoFolder, Recipient, VMEcontroller, InputFile, DaqMode, hvmode, rpcmachine, XDaqSendCommand, xdaqcommand, psxmachine, sBoardAddress, DataFolder, FileNamePrefix, FileNameSuffix, ADCboard;
    bool FileNameXDaqHV, SendToPVSS, Fit, SendMail, SendROOT, ConfigurationPrint, RawPrint, PayloadHexPrint, HeaderHexPrint, EOBHexPrint, rootfile, PrintDataFile, info, multiplereading, PrintHv, PrintConfig, QuickHisto;
    int Polarity, NotFound, eventmonitor, Board, Link, spychannel, events, NumberOfRuns, BLTsize, HistoBinningL, HistoBinningH, HistoMinL, HistoMaxL, HistoMinH, HistoMaxH, ExpectedWorkingChannels, DaqEnable, PulserEnable;
    int hv[16], TimeUnit, Start, Pulser, Period, Width, Output, NumberOfPulses, FitRange;
    CVBoardTypes VMEBoard;
    int FitLandauMin, FitLandauMax, FitGausMin, FitGausMax;

private:
  int member;
};
