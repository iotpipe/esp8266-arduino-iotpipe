#ifndef _IOTPIPE_SNTP_H_
#define _IOTPIPE_SNTP_H_

#include "Arduino.h"
#include "iotpipe_utils.h"

using namespace IotPipe_Utils;


extern "C" {
#include "sntp.h"

#define server1 "us.pool.ntp.org"
#define server2 "europe.pool.ntp.org"
#define server3 "asia.pool.ntp.org"
}


#define time_between_syncs_in_millis 1*60*60*1000
#define max_attempts_to_connect 5

class IotPipe_SNTP
{

	public:
		IotPipe_SNTP();
		~IotPipe_SNTP();

		void getTimeStamp(unsigned long timeOfReadingInMillis, String &timestamp);
		void syncToServer();
		bool isTimeSynced();
	private:
		unsigned long timeWhenSyncedInMillis;
		unsigned long absTimeInSeconds;
};


#endif
