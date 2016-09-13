#include "iotpipe.h"

IotPipe::IotPipe(String deviceId)
{
        this->samplingTopic = "sampling/" + deviceId;
        this->outputTopic = "outputport/" + deviceId;
	this->port = 1883;
	this->server = "tcp://broker.iotpipe.io";
}

IotPipe::IotPipe(const char* deviceId)
{
        this->samplingTopic = "sampling/" + String(deviceId);
        this->outputTopic = "outputport/" + String(deviceId);
	this->port = 1883;
	this->server = "tcp://broker.iotpipe.io";
}

IotPipe::~IotPipe()
{
        //do nothing
}

//Adds a sensor to the list of outputs which can receive messages
void IoTPipe::registerPin(int pin, String name)
{

}

//Takes a function that returns a float and generates a json payload that can be read by IoT Pipe service
void IotPipe::jsonifyResult( float (*f)(), String name, String buf)
{
	float value = (*f)();
	gpio.jsonifyScan(value, name, buf); 
	return buf
}

//Takes a function that returns a float and generates a json payload that can be read by IoT Pipe service
void IotPipe::jsonifyResult( int (*f)(), String name, String buf)
{
	int value = (*f)();
	gpio.jsonifyScan(value, name, buf); 
	return buf
}


//Reads a JSON payload from IoT Pipe web service that contains the desired values of output ports.
bool IotPipe::updateOutputs(String topic, String msg)
{
  if( topic.equals(this->outputTopic) )
    return gpio.gpio_update_outputs(msg);
  else
    return true;    
}


//Generates the topic to which the device subscribes to receive updates to its output ports
void IotPipe::getOutputTopic(String buf)
{
	buf = this->outputTopic;
}


//Generates the topic to which the device publishes when sampling input ports
void IotPipe::getSamplingTopic(String buf)
{
	buf = this->samplingTopic;
}
