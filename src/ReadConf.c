#include "ReadConf.h"
int ReadConf::LoadConfiguration() {

    INIReader reader(InputFile);

    if (reader.ParseError() != 0) {
        std::cout << "Can't load " << InputFile <<" \n";
        return 1;
    }
    SendMail = reader.GetBoolean("daq", "SendMail", false);
    SendROOT = reader.GetBoolean("daq", "SendROOT", true);
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
    NotFound = reader.GetInteger("adc", "NotFound", 666);
    multiplereading = reader.GetBoolean("adc", "Multiplereading", false);
    BLTsize = reader.GetInteger("adc", "BLTsize", 256);
    NumberOfRuns = reader.GetInteger("daq", "NumberOfRuns", 1);
    DataFolder = reader.Get("daq", "DataFolder", "/tmp");
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

    QuickHisto = reader.GetBoolean("analysis", "QuickHisto", false);
    Fit = reader.GetBoolean("analysis", "Fit", false);
    FitLandauMin = reader.GetInteger("analysis", "FitLandauMin", 500);
    FitLandauMax = reader.GetInteger("analysis", "FitLandauMax", 1500);
    FitGausMin = reader.GetInteger("analysis", "FitGausMin", 1500);
    FitGausMax = reader.GetInteger("analysis", "FitGausMax", 4000);
    HistoFolder = reader.Get("analysis", "HistoFolder", "/nfshome0/tetto/data/temp");
    spychannel = reader.GetInteger("analysis", "SpyChannel", 666);
    FitRange = reader.GetInteger("analysis", "FitRange", 1);
    HistoBinningL = reader.GetInteger("analysis", "HistoBinningL", 10);
    HistoBinningH = reader.GetInteger("analysis", "HistoBinningH", 10);
    HistoMinH = reader.GetInteger("analysis", "HistoMinH", 0);
    HistoMaxH = reader.GetInteger("analysis", "HistoMaxH", 4095);
    HistoMinL = reader.GetInteger("analysis", "HistoMinL", 0);
    HistoMaxL = reader.GetInteger("analysis", "HistoMaxL", 4095);

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
