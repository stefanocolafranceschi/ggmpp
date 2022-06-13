#!/bin/bash
# USAGE: nohup ./ipedscan.sh &
voltage = 10000
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
    sleep 150

/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan174.ini
/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan170.ini
/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan164.ini
/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan158.ini
#/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan156.ini
/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan154.ini
#/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan152.ini
/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan150.ini
#/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan148.ini
/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan146.ini
#/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan144.ini
/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan142.ini
#/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan140.ini
#/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan138.ini
#/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan136.ini
#/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan134.ini
#/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan132.ini
#/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan130.ini
#/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan128.ini
#/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan126.ini
#/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan124.ini
#/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan122.ini
#/nfshome0/tetto/ggm++/GGMpp -c /nfshome0/tetto/ggm++/ipedscan/ipedscan120.ini
