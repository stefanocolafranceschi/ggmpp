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

    std::string email, ChargeADC, HistoFolder, Recipient, VMEcontroller, InputFile, DaqMode, hvmode, rpcmachine, XDaqSendCommand, xdaqcommand, psxmachine, sBoardAddress, DataFolder, FileNamePrefix, FileNameSuffix, ADCboard, QuickHisto;
    bool RemovePed, FileNameXDaqHV, SendToPVSS, Fit, SendMail, arduinoEnable, SendROOT, ConfigurationPrint, RawPrint, PayloadHexPrint, HeaderHexPrint, EOBHexPrint, rootfile, PrintDataFile, info, multiplereading, PrintHv, PrintConfig;
    int FitGausBins,FitLandauBins,AvalancheThreshold, StreamerThreshold, Polarity, NotFound, eventmonitor, Board, Link, spychannel, events, NumberOfRuns, BLTsize, HistoBinningL, HistoBinningH, HistoMinL, HistoMaxL, HistoMinH, HistoMaxH, ExpectedWorkingChannels, DaqEnable, PulserEnable;
    int hv[16], TimeUnit, Start, Pulser, Period, Width, Output, NumberOfPulses, FitRange;
    int PedAvg[16], PedSigma[16], arduinoOffset;
    CVBoardTypes VMEBoard;
    int FitLandauMin, FitLandauMax, FitGausMin, FitGausMax, IPED, SpyChannel;
    int gap0, gap1, gap2, gap3, gap4, gap5, gap6, gap7, gap8, gap9, gap10, gap11, gap12, gap13, gap14, gap15, ReferenceGap;
    int temp_bin, temp_min, temp_max, p_bin, p_min, p_max, rh_bin, rh_min, rh_max, hv_bin, hv_min, hv_max;
    int histo_bin, histo_min, histo_max, effi_bin, charge_bin, wp_bin, unixtime_bin, timeperiod, avalanche_min, avalanche_bin, avalanche_max, streamer_bin, streamer_min, streamer_max, eff_bin;
    float eff_min, eff_max, chargeMin, chargeMax, charge_min, charge_max, label_font, axis_offset, time_bin, label_size, wp_min, wp_max;

private:
  int member;
};
