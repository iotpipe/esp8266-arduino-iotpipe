#ifndef IOTPIPE_GPIO_H_
#define IOTPIPE_GPIO_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include "iotpipe_sntp.h"
#include "iotpipe_utils.h"


const int max_portname_length = 16;
const int max_jsmn_tokens = 128;

//Max number of GPIOs on device.  Can be more but not less.
const int max_sensors = 32;




const int max_json_payload_length = 256;
class IotPipe_GPIO
{
	public:
		IotPipe_GPIO();
		~IotPipe_GPIO();

		//updates value field in each node of gpio_head
		template <typename T> bool jsonifyInputScan(T val, String timestamp, String name, String& buf);
		void gpioUpdateOutputs(String msg);
		void registerSensor(int pin, String sensorName);

		int findSensor(String name);

	private:
		//Checks if port # is a valid GPIO for the ESP8266
		//sets output pins high or low
		void updateOutput(int pin, String newValue);    

		//Array of pin,name values for registered sensors
		String registeredSensors[max_sensors];
};


template <typename T> 
bool IotPipe_GPIO::jsonifyInputScan(T val, String timestamp, String name, String& buf)
{

	Serial.println("in gpio");
 
	StaticJsonBuffer<max_json_payload_length> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

	root.set<String>("timestamp", timestamp);
	root.set<T>(name,val);

	int len = root.measureLength();


 	//If json response is too large
	if(len > max_json_payload_length)
	{
		return false;
	}

	Serial.println("still in gpio");

	root.printTo(buf);

	Serial.println("printing to buffer");
	return true;
}


#endif
