lines=$(find /ggmdata/DATA/*.root -type f -mmin -20 | wc -l)

if [ $lines -eq 0 ]; then
  /opt/xdaq/bin/dpSet.sh cms_rpc_dcs_1:rpc_ggm_trg0.rate:_original.._value 0  http://kvm-s3562-1-ip151-97.cms:9924/urn:xdaq-application:lid=30
  mail -s "GGM rate is zero" cmsfrascatiggm@googlegroups.com < /dev/null
fi
