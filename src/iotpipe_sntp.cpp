#include "iotpipe_sntp.h"

IotPipe_SNTP::IotPipe_SNTP()
{
	sntp_setservername(0,server1);
	sntp_setservername(1,server2);
	sntp_setservername(2,server3);  

	sntp_init(); 
	

	this->absTimeInSeconds=0;
	this->timeWhenSyncedInMillis=0;

}

void IotPipe_SNTP::syncToServer()
{
	if(!isTimeSynced())
	{	
		this->absTimeInSeconds = sntp_get_current_timestamp();
		unsigned long tempTime = millis();
		LOG_DEBUG("Connecting to NTP Server");

		int counter=0;
		while(this->absTimeInSeconds==0 & counter < max_attempts_to_connect)
		{
			delay(500);
			this->absTimeInSeconds = sntp_get_current_timestamp();
			tempTime = millis();
			LOG_DEBUG(".");	
			counter++;
		}
		if(this->absTimeInSeconds!=0)
		{
			LOG_DEBUG("Connected to server.");
			this->timeWhenSyncedInMillis = tempTime;
		}
	}
}

bool IotPipe_SNTP::isTimeSynced()
{
	return ! ( timeWhenSyncedInMillis == 0 | millis() - timeWhenSyncedInMillis > time_between_syncs_in_millis );
}

IotPipe_SNTP::~IotPipe_SNTP()
{
	  //do nothing
}


void IotPipe_SNTP::getTimeStamp(unsigned long timeOfReadingInMillis, String &timestamp)
{
	Serial.println("timestamp getting");
	unsigned long time;              //Store our current time in seconds
	unsigned long offSet;            //This computes the # of seconds between the reading and when the time was synced
	int offSetFractionalPart = 0;    //Our offset discards everything below 1 second, so we store the fraction part of the second as a number between 0 and 999, where 636 would be 0.636 seconds for example.

	offSet = this->timeWhenSyncedInMillis - timeOfReadingInMillis; 
	offSetFractionalPart = offSet % 1000; //This is because the last 3 digits of offSet are discared on the next line when we divide by 1000 to convert from milliseconds to seconds
	offSet = offSet / 1000.;

	time = this->absTimeInSeconds - offSet;
	Serial.println("time time");
	timestamp = String(time);
	timestamp = timestamp + String(offSetFractionalPart);
	Serial.println("timestamp timestamp");
	Serial.println(timestamp);
}
