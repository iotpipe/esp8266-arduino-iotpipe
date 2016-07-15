#include "iotpipe_sntp.h"

IotPipe_SNTP::IotPipe_SNTP()
{
  sntp_setservername(0,server1);
  sntp_setservername(1,server2);
  sntp_setservername(2,server3);  
  sntp_init(); 
}

IotPipe_SNTP::~IotPipe_SNTP()
{
  //do nothing
}

int IotPipe_SNTP::getEpochTimeInSeconds()
{
  return sntp_get_current_timestamp();
}

