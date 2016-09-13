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
		//Arguments: Function that returns a sensor value as a float, sensor name, and a buffer to store the JSON
		//Returns: None
		void jsonifyResult( int (*f)(), String name, String buf);

		//Creates a JSON payload of a scan
		//Arguments: Function that returns a sensor value as an integer, sensor name, and a buffer to store the JSON
		//Returns: None
		void jsonifyResult( float (*f)(), String name, String buf);



		//Reads a JSON payload from IoT Pipe web service that contains the desired values of output ports. 
		//Arguments: The JSON payload and topic sent from IoT Pipe web service.
		//Return: True/False to designate a success or failure 
		bool updateOutputs(String topic, String msg);


		//Generates the topic to which the device subscribes to receive updates to its output ports
		//Arguments: String buffer to which output topic is written
		//Return: None
		void getOutputTopic(String buf) {buf = this->outputTopic;};


		//Generates the topic to which the device publishes when sampling input ports
		//Arguments: String buffer to which sampling topic is written
		//Return: None 
		void getSamplingTopic(String buf) {buf = this->samplingTopic;}

		//Gets the MQTT Port
		//Arguments: None
		//Returns: The MQTT Port as an integer
		int getPort() {return this->port;}

		//Gets the domain of the MQTT server
		//Arguments: String buffer to which the server domain is written
		//Returns: None
		void getServer(String buf) {buf = this->server;}

	private:
		IotPipe_GPIO gpio;
		String samplingTopic;
		String outputTopic;
		String server;
		int port;
};


#endif


