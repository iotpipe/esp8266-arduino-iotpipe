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
			delay(1000);
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
	unsigned long time;              //Store our current time in seconds
	unsigned long offSet;            //This computes the # of seconds between the reading and when the time was synced
	int fractionalOffsetPart = 0;    //Our offset discards everything below 1 second, so we store the fraction part of the second as a number between 0 and 999, where 636 would be 0.636 seconds for example.

	offSet = timeOfReadingInMillis - this->timeWhenSyncedInMillis;

	fractionalOffsetPart = offSet % 1000; //This is because the last 3 digits of offSet are discared on the next line when we divide by 1000 to convert from milliseconds to seconds
	offSet = offSet / 1000.;

	time = this->absTimeInSeconds + offSet;

	timestamp = String(time);

	//We pad the offset with zeroes in case it is only 1 or 2 digits.
	String fractionalOffsetString = String(fractionalOffsetPart);

	padStringWithZeros(fractionalOffsetString,3);

	timestamp = timestamp + fractionalOffsetString;

	Serial.print("timeWhenSyncedInMillis:   ");
	Serial.println(this->timeWhenSyncedInMillis);

	Serial.print("timeOfReadingInMillis:   ");
	Serial.println(timeOfReadingInMillis);

	Serial.print("absTimeInSeconds:   ");
	Serial.println(this->absTimeInSeconds);
	
	Serial.print("Offset:   ");
	Serial.println(offSet);

	Serial.print("fractionOffsetString:   ");
	Serial.println(fractionalOffsetString);

}



