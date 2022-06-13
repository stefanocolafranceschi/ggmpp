#include "ReadConf.h"
int ReadConf::LoadConfiguration() {

    INIReader reader(InputFile);

    if (reader.ParseError() != 0) {
        std::cout << "Can't load " << InputFile <<" \n";
        return 1;
    }
 
    // - Monitoring Variables ----
    email = reader.Get("monitor", "email", "stefano.colafranceschi@emu.edu");

    temp_bin = reader.GetInteger("monitor", "temp_bin",200);
    temp_min = reader.GetInteger("monitor", "temp_min",15);
    temp_max = reader.GetInteger("monitor", "temp_max",25);

    p_bin = reader.GetInteger("monitor", "p_bin",200);
    p_min = reader.GetInteger("monitor", "p_min",920);
    p_max = reader.GetInteger("monitor", "p_max",980);

    rh_bin = reader.GetInteger("monitor", "rh_bin",200);
    rh_min = reader.GetInteger("monitor", "rh_min",-5);
    rh_max = reader.GetInteger("monitor", "rh_max",65);

    hv_bin = reader.GetInteger("monitor", "hv_bin",200);
    hv_min = reader.GetInteger("monitor", "hv_min",0);
    hv_max = reader.GetInteger("monitor", "hv_max",10200);

    histo_bin = reader.GetInteger("monitor", "histo_bin",200);
    histo_min = reader.GetInteger("monitor", "histo_min",0);
    histo_max = reader.GetInteger("monitor", "histo_max",10000);

    eff_bin = reader.GetInteger("monitor", "eff_bin",200);
    eff_min = reader.GetFloat("monitor", "eff_min",0);
    eff_max = reader.GetFloat("monitor", "eff_max",100);

    chargeMin = reader.GetFloat("monitor", "chargeMin",0.1);
    chargeMax = reader.GetFloat("monitor", "chargeMax",5);

    charge_bin = reader.GetInteger("monitor", "charge_bin",200);
    charge_min = reader.GetFloat("monitor", "charge_min",0);
    charge_max = reader.GetFloat("monitor", "charge_max",250);

    avalanche_bin = reader.GetInteger("monitor", "avalanche_bin",200);
    avalanche_min = reader.GetInteger("monitor", "avalanche_min",0);
    avalanche_max = reader.GetInteger("monitor", "avalanche_max",1000);

    streamer_bin = reader.GetInteger("monitor", "streamer_bin",200);
    streamer_min = reader.GetInteger("monitor", "streamer_min",0);
    streamer_max = reader.GetInteger("monitor", "streamer_max",1000);

    label_font = reader.GetFloat("monitor", "label_font",0.025);
    axis_offset = reader.GetFloat("monitor", "axis_offset",0.9);
    time_bin = reader.GetFloat("monitor", "time_bin",5);
    label_size = reader.GetFloat("monitor", "label_size",0.03);

    wp_min = reader.GetFloat("monitor", "wp_min",0);
    wp_max = reader.GetFloat("monitor", "wp_max",1.8);
  
    wp_bin = reader.GetInteger("monitor", "wp_bin",200);
    unixtime_bin = reader.GetInteger("monitor", "unixtime_bin", 100);
    timeperiod = reader.GetInteger("monitor", "timeperiod", 604800);
//-----------------------------

    SendMail = reader.GetBoolean("daq", "SendMail", false);
    SendROOT = reader.GetBoolean("daq", "SendROOT", true);
    arduinoEnable = reader.GetBoolean("daq", "arduinoEnable", true);
    arduinoOffset = reader.GetInteger("daq", "arduinoOffset", 0);
    Recipient = reader.Get("daq", "Recipient", "stefano.colafranceschi@emu.edu");
    VMEcontroller = reader.Get("daq", "VMEcontroller", "V2718");
    info = reader.GetBoolean("verbose", "Info", true);
    FileNameXDaqHV = reader.GetBoolean("daq", "FileNameXDaqHV", true);
    ConfigurationPrint = reader.GetBoolean("verbose", "ConfigurationPrint", true);
    events = reader.GetInteger("daq", "Events", 100);
    Board = reader.GetInteger("daq", "Board", 0);
    Link = reader.GetInteger("daq", "Link", 0);
    sBoardAddress = reader.Get("daq", "BoardAddress", "0");
    ADCboard = reader.Get("adc", "ADCBoard", "965");
    ExpectedWorkingChannels = reader.GetInteger("adc", "ExpectedWorkingChannels", 16);

    ReferenceGap = reader.GetInteger("adc", "ReferenceGap", 3);
    gap0 = reader.GetInteger("adc", "gap0", 0);
    gap1 = reader.GetInteger("adc", "gap1", 1);
    gap2 = reader.GetInteger("adc", "gap2", 2);
    gap3 = reader.GetInteger("adc", "gap3", 3);
    gap4 = reader.GetInteger("adc", "gap4", 4);
    gap5 = reader.GetInteger("adc", "gap5", 5);
    gap6 = reader.GetInteger("adc", "gap6", 6);
    gap7 = reader.GetInteger("adc", "gap7", 7);
    gap8 = reader.GetInteger("adc", "gap8", 8);
    gap9 = reader.GetInteger("adc", "gap9", 9);
    gap10 = reader.GetInteger("adc", "gap10", 10);
    gap11 = reader.GetInteger("adc", "gap11", 11);
    gap12 = reader.GetInteger("adc", "gap12", 12);
    gap13 = reader.GetInteger("adc", "gap13", 13);
    gap14 = reader.GetInteger("adc", "gap14", 14);
    gap15 = reader.GetInteger("adc", "gap15", 15);

    IPED = reader.GetInteger("adc", "IPED", 180);
    NotFound = reader.GetInteger("adc", "NotFound", 666);
    multiplereading = reader.GetBoolean("adc", "Multiplereading", false);
    BLTsize = reader.GetInteger("adc", "BLTsize", 256);
    NumberOfRuns = reader.GetInteger("daq", "NumberOfRuns", 1);
    DataFolder = reader.Get("daq", "DataFolder", "/tmp");
    ChargeADC = reader.Get("adc", "ChargeADC", "Total");
    FileNamePrefix = reader.Get("daq", "FileNamePrefix", "undefined");
    FileNameSuffix = reader.Get("daq", "FileNameSuffix", "");
    hvmode = reader.Get("daq", "HvMode", "config");
    rootfile = reader.GetBoolean("daq", "RootFile", true);
    DaqEnable = reader.GetBoolean("daq", "Enable", true);
    DaqMode = reader.Get("daq", "DaqMode", "Physics");

    RawPrint = reader.GetBoolean("verbose", "RawPrint", false);
    PayloadHexPrint = reader.GetBoolean("verbose", "PayloadHexPrint", false);
    EOBHexPrint = reader.GetBoolean("verbose", "EOBHexPrint", false);
    HeaderHexPrint = reader.GetBoolean("verbose", "HeaderHexPrint", false);
if (RawPrint) {
PayloadHexPrint = true;
EOBHexPrint = true;
HeaderHexPrint = true;
}
else {
PayloadHexPrint	= false;
EOBHexPrint = false;
HeaderHexPrint = false;
}
    eventmonitor = reader.GetInteger("verbose", "EventMonitor", 100);  
  
    PrintHv = reader.GetBoolean("verbose", "PrintHv", true);
    PrintConfig = reader.GetBoolean("verbose", "PrintConfig", true);
    PrintDataFile = reader.GetBoolean("verbose", "PrintDataFile", true);

    QuickHisto = reader.Get("analysis", "QuickHisto", "low");
    RemovePed = reader.GetBoolean("analysis", "RemovePed", false);
    Fit = reader.GetBoolean("analysis", "Fit", false);
    FitLandauMin = reader.GetInteger("analysis", "FitLandauMin", 500);
    SpyChannel = reader.GetInteger("analysis","SpyChannel",15);
    FitLandauMax = reader.GetInteger("analysis", "FitLandauMax", 1500);
    FitGausMin = reader.GetInteger("analysis", "FitGausMin", 1500);
    FitGausMax = reader.GetInteger("analysis", "FitGausMax", 4000);
    FitGausBins = reader.GetInteger("analysis", "FitGausMax", 200);
    FitLandauBins = reader.GetInteger("analysis", "FitLandauMax", 200);
    StreamerThreshold = reader.GetInteger("analysis", "StreamerThreshold", 100);
    AvalancheThreshold = reader.GetInteger("analysis", "AvalancheThreshold", 50);
    HistoFolder = reader.Get("analysis", "HistoFolder", "/nfshome0/tetto/data/temp");
    spychannel = reader.GetInteger("analysis", "SpyChannel", 666);
    FitRange = reader.GetInteger("analysis", "FitRange", 1);
    HistoBinningL = reader.GetInteger("analysis", "HistoBinningL", 10);
    HistoBinningH = reader.GetInteger("analysis", "HistoBinningH", 10);
    HistoMinH = reader.GetInteger("analysis", "HistoMinH", 0);
    HistoMaxH = reader.GetInteger("analysis", "HistoMaxH", 4095);
    HistoMinL = reader.GetInteger("analysis", "HistoMinL", 0);
    HistoMaxL = reader.GetInteger("analysis", "HistoMaxL", 4095);
    PedAvg[0] = reader.GetInteger("analysis", "PedAvg0", 0);
    PedSigma[0] = reader.GetInteger("analysis", "PedSigma0", 0);
    PedAvg[1] = reader.GetInteger("analysis", "PedAvg1", 0);
    PedSigma[1] = reader.GetInteger("analysis", "PedSigma1", 0);
    PedAvg[2] = reader.GetInteger("analysis", "PedAvg2", 0);
    PedSigma[2] = reader.GetInteger("analysis", "PedSigma2", 0);
    PedAvg[3] = reader.GetInteger("analysis", "PedAvg3", 0);
    PedSigma[3] = reader.GetInteger("analysis", "PedSigma3", 0);
    PedAvg[4] = reader.GetInteger("analysis", "PedAvg4", 0);
    PedSigma[4] = reader.GetInteger("analysis", "PedSigma4", 0);
    PedAvg[5] = reader.GetInteger("analysis", "PedAvg5", 0);
    PedSigma[5] = reader.GetInteger("analysis", "PedSigma5", 0);
    PedAvg[6] = reader.GetInteger("analysis", "PedAvg6", 0);
    PedSigma[6] = reader.GetInteger("analysis", "PedSigma6", 0);
    PedAvg[7] = reader.GetInteger("analysis", "PedAvg7", 0);
    PedSigma[7] = reader.GetInteger("analysis", "PedSigma7", 0);
    PedAvg[8] = reader.GetInteger("analysis", "PedAvg8", 0);
    PedSigma[8] = reader.GetInteger("analysis", "PedSigma8", 0);
    PedAvg[9] = reader.GetInteger("analysis", "PedAvg9", 0);
    PedSigma[9] = reader.GetInteger("analysis", "PedSigma9", 0);
    PedAvg[10] = reader.GetInteger("analysis", "PedAvg10", 0);
    PedSigma[10] = reader.GetInteger("analysis", "PedSigma10", 0);
    PedAvg[11] = reader.GetInteger("analysis", "PedAvg11", 0);
    PedSigma[11] = reader.GetInteger("analysis", "PedSigma11", 0);
    PedAvg[12] = reader.GetInteger("analysis", "PedAvg12", 0);
    PedSigma[12] = reader.GetInteger("analysis", "PedSigma12", 0);
    PedAvg[13] = reader.GetInteger("analysis", "PedAvg13", 0);
    PedSigma[13] = reader.GetInteger("analysis", "PedSigma13", 0);
    PedAvg[14] = reader.GetInteger("analysis", "PedAvg14", 0);
    PedSigma[14] = reader.GetInteger("analysis", "PedSigma14", 0);
    PedAvg[15] = reader.GetInteger("analysis", "PedAvg15", 0);
    PedSigma[15] = reader.GetInteger("analysis", "PedSigma15", 0);

    rpcmachine = reader.Get("XDAQ", "DCSmachine", "cms_rpc_dcs_1");
    xdaqcommand = reader.Get("XDAQ", "XDAQ_GetScript", "/opt/xdaq/bin/dpGet.sh");
    XDaqSendCommand = reader.Get("XDAQ", "XDAQ_SetScript", "/opt/xdaq/bin/dpSet.sh");
    SendToPVSS = reader.GetBoolean("XDAQ", "SendToPVSS",false);
    psxmachine = reader.Get("XDAQ", "PSXmachine", "http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30");

    PulserEnable = reader.GetBoolean("pulser", "Enable", false);
    Pulser = reader.GetInteger("pulser", "Pulser", 0);
    Period = reader.GetInteger("pulser", "Period", 2);
    Width = reader.GetInteger("pulser", "Width", 1);
    NumberOfPulses = reader.GetInteger("pulser", "NumberOfPulses", 0);
    Start = reader.GetInteger("pulser", "Start", 0);
    TimeUnit = reader.GetInteger("pulser", "TimeUnit", 0);
    Output = reader.GetInteger("pulser", "Output", 0);
    Polarity = reader.GetInteger("pulser", "Polarity", 0);

    hv[0] = reader.GetInteger("hv", "channel0", 0);        
    hv[1] = reader.GetInteger("hv", "channel1", 0);
    hv[2] = reader.GetInteger("hv", "channel2", 0);
    hv[3] = reader.GetInteger("hv", "channel3", 0);
    hv[4] = reader.GetInteger("hv", "channel4", 0);
    hv[5] = reader.GetInteger("hv", "channel5", 0);
    hv[6] = reader.GetInteger("hv", "channel6", 0);
    hv[7] = reader.GetInteger("hv", "channel7", 0);
    hv[8] = reader.GetInteger("hv", "channel8", 0);
    hv[9] = reader.GetInteger("hv", "channel9", 0);
    hv[10] = reader.GetInteger("hv", "channel10", 0);
    hv[11] = reader.GetInteger("hv", "channel11", 0);
}

void ReadConf::PrintConfiguration() {
        std::cout << "\n\n--------------------------------------------------------------\nConfig loaded from " << InputFile
        << ", Printing Info = " << info
        << ":\n\n Events = " << events
        << ", VME Board = " << VMEBoard
        << ", Board Address = " << std::hex << std::stoull(sBoardAddress,NULL,16) << std::dec
        << ", Board ID = " << Board
        << ", Link ID = " << Link
        << ", Multiple Reading = " << multiplereading
        << ", BLT Size = " << BLTsize
        << ",\n\n HV reading mode = " << hvmode
        << ", DCS machine = " << rpcmachine
        << ",\n XDAQ GetScript = " << xdaqcommand
        << ",\n PSX machine = " << psxmachine
        << ",\n\n Filename Prefix = " << FileNamePrefix
        << ", Filename Suffix = " << FileNameSuffix
        << ", PayloadHexPrint = " << PayloadHexPrint
        << ", HeaderHexPrint = " << HeaderHexPrint
        << ", EOBHexPrint = " << EOBHexPrint
        << ", Print HV = " << PrintHv
        << ", Print Data File = " << PrintDataFile
        << ", Rootfile = " << rootfile
        << ", Event Monitor = " << eventmonitor
        << ", QuickHisto = " << QuickHisto
        << ", Spy Channel = " << spychannel << "\n--------------------------------------------------------------\n\n";
}
