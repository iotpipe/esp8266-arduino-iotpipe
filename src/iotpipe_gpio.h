#ifndef IOTPIPE_GPIO_H_
#define IOTPIPE_GPIO_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include "iotpipe_sntp.h"
#include "iotpipe_utils.h"


const int max_portname_length = 16;
const int max_jsmn_tokens = 128;
const int max_sensors = 64;


typedef struct
{
	int pin;
	String name;
} sensor_t;





const int max_json_payload_length = 256;
class IotPipe_GPIO
{
	public:
		IotPipe_GPIO();
		~IotPipe_GPIO();

		//updates value field in each node of gpio_head
		template <typename T> bool jsonifyInputScan(T val, String name, String buf);
		bool gpioUpdateOutputs(String msg);
	private:
		//Checks if port # is a valid GPIO for the ESP8266
		//sets output pins high or low
		void updateOutput(int pin, String newValue);    

		//Array of pin,name values for registered sensors
		sensor_t registeredSensors[max_sensors];
};


template <typename T> 
bool jsonifyInputScan(T val, String name, String buf)
{
 
	StaticJsonBuffer<max_json_payload_length> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

	IotPipe_SNTP timeGetter;


	//If we don't yet have a connection to the time server
	if(timeGetter.getEpochTimeInSeconds() == 0)
  	{
		buf="";
    		return false;
  	}


  	//We cast time to a string so we can append three 0's.  This is because we can't handle longs on the arduino.
	String timeBuf = String(timeGetter.getEpochTimeInSeconds());
	timeBuf = timeBuf + "000";

	root.set<String>("timestamp", timeBuf);

	root.set<T>(name,val);

	int len = root.measureLength();


 	//If json response is too large
	if(len > max_json_payload_length)
	{
		return false;
	}
	root.printTo(buf);
	return true; 
}






#endif
