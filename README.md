# ggmpp
GGM++ system

  The Gas Gain Monitoring (GGM) system is a small cosmic ray telescope that relies on Resistive Plate Counters to ensure a stable operation of the Compact Muon Solenoid (CMS) RPC sub-system. The GGM was designed to deliver charge readout data whose stability is a function of the quality of the gas mixture, common between the GGM and the CMS RPC sub-system. After nearly a decade of operations, the GGM DAQ and analysis routine have been upgraded to increase performance, reduce downtime, and improve long-term maintenance. The new DAQ features a standalone C++ software, modular and configurable at runtime; the analysis package is based on ROOT6 while the data quality monitoring is achieved via a Python-based local webserver. The software system is interfaced to a new PCI-Express optical card that reads a VME Controller and a QDC connected to the cosmic ray telescope. Technical details about the upgraded GGM and the developed general-purpose DAQ are hereby described.

Introduction
During 2021 the Gas Gain Monitoring (GGM) system has seen a complete software and hardware redesign. The new system, GGM++ replaces the old one improving performance, stability and general reliability.
The GGM++ is a cosmic ray telescope, based on small 12 RPC detectors whose working point is continuously monitored online. The monitoring is achieved by reading the charge induced by cosmic muons on copper readout pads\cite{Colafranceschi:2010zz}. The muon signal is amplified by RPCs flown with the same gas mixture that is supplying the CMS RPC Muon system. By doing so, any shift in working point measured by the GGM++ can be correlated to a wrong or contamined gas mixture, triggering an alarm to the entire Muon system.

Hardware setup and devices
Out of the 12 RPC, 4 form the trigger, in a majority system 3/4. The remaining 8 RPCs are organized in 3 sets. One set of (two) RPC detectors flown with fresh gas mixture and two sets of RPC detectors flown with the CMS RPC closed loop gas mixture\cite{Benussi:2010yx}, one set is using the recirculated gas before purifiers and the other set the gas mixture after the purifiers\cite{Benussi:2008vs}. 
The 8 RPCs are supplied with a CAEN High Voltage system, read by a CAEN V965 charge sensitive ADC, and monitored in terms of gas pressure (in/out), gas temperature (in/out) and ambient conditions. 
The GGM++ features a new Arduino-based system that interfaces with pressure and temperature sensors. Each RPC gap is equipped with two temperature/pressure sensors at the gas inlet/outlet. Two Arduino MEGA collect all sensors data, transferred via serial line to a new DAQ computer. 

In the GGM++, the DAQ computer got upgraded to a quad-core Xeon with a PCI-express fiber optics card, connected to the VME crate (where the ADC is installed). The operating system adopted by CMS is CentOS~7.6 and the machine is protected by the private CMS network, reachable from CERN online cluster (lxplus).

Software infrastructure
The GGM++ software is mounted on NFS (and so the collected data) and is fully compiled with C++. In addition, the software makes use of several libraries among which ROOT6 and XDAQ for prompt analysis and data-retrieval/storage from/to the CMS Online database. Arduino is also used to interrogate the hardware sensors the RPC are equipped with. A dedicated Linux built-in process that belong to the family of "systemd daemon" powers up the GGM++ at boot. The systemd process can be controlled with the built-in Linux systemctl as the GGM++ systemd daemon provides common commands (such as start, restart and stop) as shown below:

$ systemctl -l status ggm.service
  $ systemctl -l stop ggm.service
  $ systemctl -l start ggm.service

Alternatively (to systemd) and/or for debugging reasons, the GGM++ can run in standalone mode so that the user can program the system to print a number of verbose printouts to monitor the status of the data-taking. Another use case for the standalone mode is the GGM++ High Voltage Scan. A dedicated bash script runs the standalone GGM++ after increasing the HV on the RPC detectors in programmable steps. The HV is being written by the DAQ into WinCC data-points, that eventually set the CAEN Hardware Mainframe to the desired HV value. 
In standalone mode the GGM++ executable runs simply by:

  $ ./GGMpp

The system accepts two parameters as displayed by the helper function:
  $ ./GGMpp -h

The first option (-a) instruct the GGM++ to re-analyze an already existing run (rootfile) and possibly to regenerate plots and new results. The second option (-c) specifies a configuration file to be used (without option the standard config file might be used).

$ ./GGMpp -a
  $ ./GGMpp -c

A major feature that GGM++ introduces is the above mentioned configuration file (ASCII) that is fully customizable at runtime to provide a high level of customizability and programmability. The config file is programmed to get a very large number of input variables to enable/disable and/or adjust specific feature of the system. Appendix~\ref{myconffile} shows a sample config file of the GGM++.

The first part of the file is about [daq] related fields, such as enable/disable the QCD and/or the Arduino and set the working mode: Physics (real data from detectors), Montecarlo (simulated data for debugging, tests and trials). The system can send emails, to a specified address, attaching plots and/or ROOT files as indicated in the configuration file. The QDC address, link, VME controller address and number of events control how the acquisition system will perform. The Filename prefix, suffix, data folder and location deal with how and where output files will be saved. Lastly the HVmode can be set to off (do not care about HV), xdaq (use xdaq to send a SOAP message to WinCC and retrieve the HV), config (read statically the HV in the config file).

The [adc] section programs the QCD in terms of the board version (8 vs 16 channels), number of expected working channels and comparator current adjustable register.

The [verbose] section allows to print out various helper functions and hex/raw/binary data to visualize event by event the structure of the header, payload and trailer.

The [pulser] section operates on the CAEN VME Controller that has the built-in functionalities of programming output signals with pulses. Such pulser can be enabled/disabled and set to start with a specified polarity, period, width.

The [XDAQ] section declares variables needed by the GGM++ to internally utilize XDAQ scripts to read/write into WinCC datapoints. XDAQ_Get and XDAQ_Set scripts locations store those scripts while DCS and PSX machine link to computers that handle the CMS control system and the XDAQ exchange data respectively.

The [analysis] section provides a way to perform automatic analysis with a great level of adjusting variables and style. The first setting "QuickHisto" sets which plots are required to be generated; the ADC has two working ranges (low and high), so the user can select low/high or both or off. The "spychannel" number refers to a specific QDC input saturated with a large NIM signal. This saturation does not happen in coincidence with the trigger but after 9us, effectively forming a second gate\cite{Benussi:2008fp}. The spychannel setting is effective only if the "RemovePed" setting is enabled, this setting allows reading the second gate and identify the pedestal of all channels. The remaining settings set the min/max/binning of all the histograms and control the fit (enable/disable) range and binning. The pedestal average and sigma are statically written in the config file to deal with situations where the second gate is not enabled and the user intents to fit the pedestal.

The [hv] section is used to statically save the HV values, written in the config file, into the DAQ output ROOT file. This feature is used when the previous setting "HvMode" is set to config (rather than set to xdaq).

The last section [Arduinos] contains calibrations for each sensors attached to each of the 16 ports of the two Arduino used (denoted with 1\_ and 2\_). The \_cal value multiplies the raw reading (mV) while the offset value adds an offset.

Conclusions
The GGM++ development is finalized, and in production; the system is taking-data and delivering its objective. The described system is going to play a central role during the next phase of CMS data-taking, the automatic GGM++ features will ease the usability and allow the shifter to act quickly in case of problems.

