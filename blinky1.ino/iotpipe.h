#ifndef IOTPIPE_H_
#define IOTPIPE_H_

#include "iotpipe_gpio.h"
#include "Arduino.h"

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
    bool scan(String& buf); 
 
 
    //Reads a JSON payload from IoT Pipe web service that contains the desired values of output ports. 
    //Arguments: The JSON payload and topic sent from IoT Pipe web service.
    //Return: True/False to designate a success or failure 
    bool update_outputs(String topic, String msg);

  private:
    IotPipe_GPIO gpio;
    String topic;
};


#endif


