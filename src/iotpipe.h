#ifndef IOTPIPE_H_
#define IOTPIPE_H_

#include "iotpipe_gpio.h"
#include "iotpipe_utils.h"

using namespace IotPipe_Utils;

class IotPipe
{
	public:
		IotPipe(String deviceId, void (*)(String)=NULL);
		IotPipe(const char* deviceId, void (*)(String)=NULL);
		~IotPipe();

		//Creates a JSON payload of a scan
		//Arguments: sensor value, sensor name, and a buffer to store the JSON
		//Returns: None
		template <typename T>
		void jsonifyResult(  T val, String name, String &buf);

		//Creates a JSON payload of mulitple measurements.  This is used when multiple measurements have the same timestamp
		//Arguments: sensor values, sensor names, number of measurements, and a buffer to store the JSON
		//Returns: None
		template <typename T>
		void jsonifyResult( T *values, String *names, int numMeasures, String &buf);

		//Adds a sensor to the list of outputs which can receive messages
		void registerSensor(int pin, String name);

		//Generates the topic to which the device subscribes to receive updates to its output ports
		//Arguments: String buffer to which topic is written
		//Return: None
		void getOutputTopic(String &buf) {buf = this->outputTopic;};

		//Generates the topic to which the device publishes when sampling input ports
		//Arguments: String buffer to which topic is written
		//Return: None 
		void getSamplingTopic(String &buf) {buf = this->samplingTopic;}

		//Generates the topic to which the device subscribes so it can send real time data 
		//Arguments: String buffer to which topic is written
		//Return: None 
		void getRealTimeTopic(String &buf) {buf = this->realTimeTopic;}

		//Called inside your MQTT Message Callback to perform IoT Pipe business.
		//Arguments: the MQTT Topic and Payload
		//Return: None
		void messageHandler(String topic, String payload);

	private:

		IotPipe_GPIO gpio;
		IotPipe_SNTP timeGetter;
		String samplingTopic;
		String outputTopic;
		String realTimeTopic;

		void (*udfMsgRcvd_cb)(String);

		void userDefinedMsg(String msg);
		void updateOutputs(String topic);
};



//Takes a function that returns a float and generates a json payload that can be read by IoT Pipe service

template <typename T>
void IotPipe::jsonifyResult( T value, String name, String &buf)
{

	long timeAtReading = millis();
	String timestamp;

	if ( timeGetter.isTimeSynced() == false )
	{
		timeGetter.syncToServer();
	}
	
	delay(10);

	if( timeGetter.isTimeSynced() == true )
	{

		timeGetter.getTimeStamp(timeAtReading, timestamp);
		delay(10);
		gpio.jsonifyInputScan(value, timestamp, name, buf); 

	}
}

//takes a variable number of measures and writes to a single timestamp
template <typename T>
void IotPipe::jsonifyResult( T *values, String* names, int numMeasures, String &buf)
{

	long timeAtReading = millis();
	String timestamp;

	if ( timeGetter.isTimeSynced() == false )
	{
		timeGetter.syncToServer();
	}
	
	delay(10);

	if( timeGetter.isTimeSynced() == true )
	{

		timeGetter.getTimeStamp(timeAtReading, timestamp);
		delay(10);
		gpio.jsonifyInputScan(values, timestamp, names, numMeasures, buf); 
	}
}

#endif


