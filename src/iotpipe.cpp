#include "iotpipe.h"
#include "iotpipe_utils.h"

using namespace IotPipe_Utils;

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
bool IotPipe::scan(String &buf)
{
        return gpio.jsonifyInputScan(buf);
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
bool IotPipe::get_output_topic(char *topicBuf, int len)
{
  if (this->outputTopic.length() > len - 1)
  {
    LOG_DEBUG_ARGS("Output topic is %d characters long.  Must allocate at least %d characters for buffer.", this->outputTopic.length(), this->outputTopic.length()+1);
    return false;
  }

  for(int i = 0; i < len; i++)
    topicBuf[i]='\0';


  strcpy(topicBuf, this->outputTopic.c_str());
  return true;
  
}

//Generates the topic to which the device subscribes to receive updates to its output ports
bool IotPipe::get_output_topic(String& topicBuf)
{
  topicBuf = String(this->outputTopic);
  return true;
}

//Generates the topic to which the device publishes when sampling input ports
bool IotPipe::get_sampling_topic(char *samplingBuf, int len)
{
  if (this->samplingTopic.length() > len - 1)
  {
    LOG_DEBUG_ARGS("Sampling topic is %d characters long.  Must allocate at least %d characters for buffer.", this->samplingTopic.length(), this->samplingTopic.length()+1);
    return false;
  }

  for(int i = 0; i < len; i++)
    samplingBuf[i]='\0';


  strcpy(samplingBuf, this->samplingTopic.c_str());
  return true;
    
}

//Generates the topic to which the device publishes when sampling input ports
bool IotPipe::get_sampling_topic(String& samplingBuf)
{
  samplingBuf = String(this->samplingTopic);
  return true;  
}

