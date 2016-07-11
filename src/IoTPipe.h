#ifndef IOTPIPE_H_
#define IOTPIPE_H_

#include "iotpipe_gpio.h"
#include "Arduino.h"
#include "iotpipe_utils."

using namespace IoTPipe_Utils;

class IotPipe
{
  public:
    IotPipe(String deviceId);
    IotPipe(const char* deviceId);
    ~IotPipe();

 
    //Adds a GPIO to a list of input ports 
    //Arguments:  GPIO # and desired name of the port 
    //Return: True/False to designate a success or failure 
    bool addDigitalInputPort(int portNum, String portName);

    //Adds the ADC to a list of input ports 
    //Arguments:  Desired name of the ADC 
    //Return: True/False to designate a success or failure 
    bool addAnalogInputPort(String portName);
 
    //Adds a GPIO to a list of output ports 
    //Arguments:  GPIO # and desired name of the port 
    //Return: True/False to designate a success or failure 
    bool addDigitalOutputPort(int portNum, String portName); 
 
 
    //Scans all input ports and creates a JSON payload of thier values which can be read by IoT Pipe web service 
    //Arguments:  The buffer to which the JSON payload will be written as well as the maximum length of the buffer 
    //Return: True/False to designate a success or failure 
    bool scan(String &buf); 
 
 
    //Reads a JSON payload from IoT Pipe web service that contains the desired values of output ports. 
    //Arguments: The JSON payload and topic sent from IoT Pipe web service.
    //Return: True/False to designate a success or failure 
    bool update_outputs(String topic, String msg);


    //Generates the topic to which the device subscribes to receive updates to its output ports
    //Arguments: Pass in an already allocated c-style char array that is at least 32 bytes (characters) in length
    //Return: True/False to designate success or failure
    bool get_output_topic(char *topicBuf, int len);

    //Generates the topic to which the device subscribes to receive updates to its output ports
    //Arguments: Pass in a C++ style string
    //Return: True/False to designate success or failure
    bool get_output_topic(String& topicBuf);

    //Generates the topic to which the device publishes when sampling input ports
    //Arguments: Pass in an already allocated c-style char array that is at least 32 bytes (characters) in length
    //Return: True/False to designate success or failure
    bool get_sampling_topic(char *topicBuf, int len);

    //Generates the topic to which the device publishes when sampling input ports
    //Arguments: Pass in a C++ style string
    //Return: True/False to designate success or failure
    bool get_sampling_topic(String& topicBuf);

    //Arguments: None
    //Returns: The MQTT Port as an integer
    int get_port() {return this->port;}

    //Arguments: None
    //Returns: The MQTT Server as a String
    String get_server() {return this->server.c_string();}

  private:
    IotPipe_GPIO gpio;
    String samplingTopic;
    String outputTopic;
    String server;
    int port;
};


#endif


