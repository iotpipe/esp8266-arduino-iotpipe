#include "iotpipe.h"

IotPipe::IotPipe(String deviceId)
{
        this->samplingTopic = "sampling/" + deviceId;
        this->outputTopic = "outputport/" + deviceId;

	

}

IotPipe::IotPipe(const char* deviceId)
{
        this->samplingTopic = "sampling/" + String(deviceId);
        this->outputTopic = "outputport/" + String(deviceId);
}

IotPipe::~IotPipe()
{
        //do nothing
}

//Adds a sensor to the list of outputs which can receive messages
void IotPipe::registerSensor(int pin, String name)
{
	gpio.registerSensor(pin,name);
}

//Reads a JSON payload from IoT Pipe web service that contains the desired values of output ports.
void IotPipe::updateOutputs(String topic, String msg)
{
	if( topic.equals(this->outputTopic) )
		gpio.gpioUpdateOutputs(msg);
}
