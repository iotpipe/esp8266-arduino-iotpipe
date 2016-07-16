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

//Adds a GPIO to a list of input ports
bool IotPipe::addDigitalInputPort(int portNum, String portName)
{
        bool success = gpio.setPortAsDigitalInput(portNum, portName);
        return success;
}

//Adds an analog GPIO to list of input ports
bool IotPipe::addAnalogInputPort(String portName)
{
        bool success = gpio.setPortAsAnalogInput(portName);
        return success;
}

//Adds a GPIO to a list of output ports
bool IotPipe::addDigitalOutputPort(int portNum, String portName)
{
        bool success = gpio.setPortAsDigitalOutput(portNum, portName);
        return success;
}

//Scans all input ports and creates a JSON payload of thier values which can be read by IoT Pipe web service
String IotPipe::scan()
{
	String buf;
	bool success = gpio.jsonifyInputScan(buf); 
	if(success==true)
		return buf;
	else
		return String("");
}

//Reads a JSON payload from IoT Pipe web service that contains the desired values of output ports.
bool IotPipe::update_outputs(String topic, String msg)
{
  if( topic.equals(this->outputTopic) )
    return gpio.gpio_update_outputs(msg);
  else
    return true;    
}



//Generates the topic to which the device subscribes to receive updates to its output ports
String IotPipe::get_output_topic()
{
  return String(this->outputTopic);
}


//Generates the topic to which the device publishes when sampling input ports
String IotPipe::get_sampling_topic()
{
  return String(this->samplingTopic);

}

