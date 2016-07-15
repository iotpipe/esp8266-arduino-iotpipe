#ifndef _IOTPIPE_SNTP_H_
#define _IOTPIPE_SNTP_H_

#include "Arduino.h"

extern "C" {
#include "sntp.h"

#define server1 "us.pool.ntp.org"
#define server2 "europe.pool.ntp.org"
#define server3 "asia.pool.ntp.org"
}

class IotPipe_SNTP
{

  public:
    IotPipe_SNTP();
    ~IotPipe_SNTP();

    int getEpochTimeInSeconds();
  private:
};


#endif
