#ifndef IOTPIPE_H_
#define IOTPIPE_H_

#include "iotpipe_gpio.h"
#include "iotpipe_utils.h"

using namespace IotPipe_Utils;

class IotPipe
{
	public:

		IotPipe(String deviceId);
		IotPipe(const char* deviceId);
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

		//Reads a JSON payload from IoT Pipe web service that contains the desired values of output ports. 
		//Arguments: The JSON payload and topic sent from IoT Pipe web service.
		//Return: None 
		void updateOutputs(String topic, String msg);


		//Adds a sensor to the list of outputs which can receive messages
		void registerSensor(int pin, String name);

		//Generates the topic to which the device subscribes to receive updates to its output ports
		//Arguments: String buffer to which output topic is written
		//Return: None
		void getOutputTopic(String &buf) {buf = this->outputTopic;};


		//Generates the topic to which the device publishes when sampling input ports
		//Arguments: String buffer to which sampling topic is written
		//Return: None 
		void getSamplingTopic(String &buf) {buf = this->samplingTopic;}

	private:
		IotPipe_GPIO gpio;
		IotPipe_SNTP timeGetter;
		String samplingTopic;
		String outputTopic;
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


