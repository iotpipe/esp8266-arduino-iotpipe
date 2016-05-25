#include "iotpipe.h"

IotPipe::IotPipe(String deviceId)
{
        this->topic = "sampling/" + deviceId;
}

IotPipe::IotPipe(const char* deviceId)
{
        this->topic = "sampling/" + String(deviceId);
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
bool IotPipe::scan(String& buf)
{
        return gpio.jsonifyInputScan(buf);
}

//Reads a JSON payload from IoT Pipe web service that contains the desired values of output ports.
bool IotPipe::update_outputs(String topic, String msg)
{
  if( topic.equals(this->topic) )
    return gpio.gpio_update_outputs(msg);
  else
    return true;    
}

