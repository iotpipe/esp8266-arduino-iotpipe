#ifndef IOTPIPE_H_
#define IOTPIPE_H_

#include "iotpipe_gpio.h"
#include "Arduino.h"
#include "iotpipe_utils.h"

using namespace IotPipe_Utils;

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
    //Arguments: None 
    //Return: JSON payload of scanned results.  Empty string if there was a failure
    String scan(); 
 
 
    //Reads a JSON payload from IoT Pipe web service that contains the desired values of output ports. 
    //Arguments: The JSON payload and topic sent from IoT Pipe web service.
    //Return: True/False to designate a success or failure 
    bool update_outputs(String topic, String msg);


    //Generates the topic to which the device subscribes to receive updates to its output ports
    //Arguments: None
    //Return: String with the output topic
    String get_output_topic();


    //Generates the topic to which the device publishes when sampling input ports
    //Arguments: None 
    //Return: String with the sampling topic
    String get_sampling_topic();

    //Arguments: None
    //Returns: The MQTT Port as an integer
    int get_port() {return this->port;}

    //Arguments: None
    //Returns: The MQTT Server as a String
    String get_server() {return this->server;}

  private:
    IotPipe_GPIO gpio;
    String samplingTopic;
    String outputTopic;
    String server;
    int port;
};


#endif


