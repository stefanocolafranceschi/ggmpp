#!/bin/bash
# USAGE: nohup ./hvscan.sh &
sudo systemctl -l stop ggm.service
rm /ggmdata/DATA/filelist.txt
#for ((voltage=8500;voltage<=10000;voltage=voltage+100)); do
TimeStamp=$(date +%F)
for voltage in 8500 8600 8700 8800 8900 9000 9100 9200 9300 9400 9500 9600 9700 9800 9900 10000 10100
do
    #/opt/xdaq/bin/dpSet.sh cms_rpc_dcs_1:CAEN/GGM/board00/channel002.settings.v0 $voltage http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30
    echo Setting $voltage
    /opt/xdaq/bin/dpSet.sh cms_rpc_dcs_1:rpc_ggm_ch01.hv:_original.._value $voltage http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30
    sleep 1
    /opt/xdaq/bin/dpSet.sh cms_rpc_dcs_1:rpc_ggm_ch02.hv:_original.._value $voltage http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30
    sleep 1
    /opt/xdaq/bin/dpSet.sh cms_rpc_dcs_1:rpc_ggm_ch03.hv:_original.._value $voltage http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30
    sleep 1
    /opt/xdaq/bin/dpSet.sh cms_rpc_dcs_1:rpc_ggm_ch04.hv:_original.._value $voltage http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30
    sleep 1
    /opt/xdaq/bin/dpSet.sh cms_rpc_dcs_1:rpc_ggm_ch05.hv:_original.._value $voltage http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30
    sleep 1
    /opt/xdaq/bin/dpSet.sh cms_rpc_dcs_1:rpc_ggm_ch06.hv:_original.._value $voltage http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30
    sleep 1
    /opt/xdaq/bin/dpSet.sh cms_rpc_dcs_1:rpc_ggm_ch07.hv:_original.._value $voltage http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30
    sleep 1
    /opt/xdaq/bin/dpSet.sh cms_rpc_dcs_1:rpc_ggm_ch08.hv:_original.._value $voltage http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30
    sleep 600
    /nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/hvscan/hvscan.ini
    echo -n /ggmdata/DATA/ >> /ggmdata/DATA/FileListHVscan_$TimeStamp.txt
    cat /ggmdata/DATA/running.txt >> /ggmdata/DATA/FileListHVscan_$TimeStamp.txt
    echo >> /ggmdata/DATA/FileListHVscan_$TimeStamp.txt
    cp /ggmdata/DATA/FileListHVscan_$TimeStamp.txt /ggmdata/DATA/filelist.txt
    /nfshome0/tetto/ggm++/src/PlotEffi
    //root -b -q /nfshome0/tetto/ggm++/hvscan/PlotEffi.C
    mail -a /ggmdata/DATA/filelist.txt -a /ggmdata/DATA/AllEfficiencies.pdf -a /ggmdata/DATA/Gap2effi.pdf -a /ggmdata/DATA/Gap2charge.pdf -a /ggmdata/DATA/Gap3effi.pdf -a /ggmdata/DATA/Gap3charge.pdf -a /ggmdata/DATA/Gap4effi.pdf -a /ggmdata/DATA/Gap4charge.pdf -a /ggmdata/DATA/Gap5effi.pdf -a /ggmdata/DATA/Gap5charge.pdf -a /ggmdata/DATA/Gap6effi.pdf -a /ggmdata/DATA/Gap6charge.pdf -a /ggmdata/DATA/Gap7effi.pdf -a /ggmdata/DATA/Gap7charge.pdf -a /ggmdata/DATA/Gap8effi.pdf -a /ggmdata/DATA/Gap8charge.pdf -a /ggmdata/DATA/Gap9effi.pdf -a /ggmdata/DATA/Gap9charge.pdf -s \"HV_SCAN\" cmsfrascatiggm@googlegroups.com < /dev/null
done
sudo systemctl -l start ggm.service

voltage=9800
    /opt/xdaq/bin/dpSet.sh cms_rpc_dcs_1:rpc_ggm_ch01.hv:_original.._value $voltage http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30
    sleep 1
    /opt/xdaq/bin/dpSet.sh cms_rpc_dcs_1:rpc_ggm_ch02.hv:_original.._value $voltage http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30
    sleep 1
    /opt/xdaq/bin/dpSet.sh cms_rpc_dcs_1:rpc_ggm_ch03.hv:_original.._value $voltage http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30
    sleep 1
    /opt/xdaq/bin/dpSet.sh cms_rpc_dcs_1:rpc_ggm_ch04.hv:_original.._value $voltage http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30
    sleep 1
    /opt/xdaq/bin/dpSet.sh cms_rpc_dcs_1:rpc_ggm_ch05.hv:_original.._value $voltage http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30
    sleep 1
    /opt/xdaq/bin/dpSet.sh cms_rpc_dcs_1:rpc_ggm_ch06.hv:_original.._value $voltage http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30
    sleep 1
    /opt/xdaq/bin/dpSet.sh cms_rpc_dcs_1:rpc_ggm_ch07.hv:_original.._value $voltage http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30
    sleep 1
    /opt/xdaq/bin/dpSet.sh cms_rpc_dcs_1:rpc_ggm_ch08.hv:_original.._value $voltage http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30
