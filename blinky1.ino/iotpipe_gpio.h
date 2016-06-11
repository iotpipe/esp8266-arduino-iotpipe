#ifndef IOTPIPE_GPIO_H_
#define IOTPIPE_GPIO_H_

#include <Arduino.h>
#include "iotpipe_sntp.h"

const int max_portname_length = 16;
const int max_jsmn_tokens = 128;

//Node for the linked list that stores all of the GPIOs set as inputs
typedef struct input_node
{
	int portNumber;
	String portName;
	int gpio_type;
	int value;
	bool active;
	input_node(int pn=-1) : portNumber(pn), gpio_type(-1), value(-1), active(false) {}

} gpio_node_t;

typedef enum {DIGITAL_INPUT, ANALOG_INPUT, DIGITAL_OUTPUT} gpio_mode;


const int gpio_mask[] = {1,0,1,0,1,1,0,0,0,0,0,0,1,1,1,1,0,1}; //valid gpios are 0,2,4,5,12,13,14,15.  Final spot is for ADC.
const int max_json_payload_length = 256;
class IotPipe_GPIO
{
	public:
		IotPipe_GPIO();
		~IotPipe_GPIO();

    void print();
    void print_input_values();

    bool setPortAsDigitalInput(int portNum, String portName);		
		bool setPortAsAnalogInput(String portName);
		bool setPortAsDigitalOutput(int portNum, String portName);
        
		//updates value field in each node of gpio_head
		bool jsonifyInputScan(String& buf);
		
		bool gpio_update_outputs(String msg);
	private:
		bool isPortNameValid(String portName, int type);  
		bool isValidGPIO(int portNum);  
		gpio_node_t gpios[18]; //17 (0 to 16) gpios on ESP8266 + 1 ADC
    
    
		void addNode(int pin, String portName, int type);
		
		//Checks if port # is a valid GPIO for the ESP8266
		//sets output pins high or low
		void updateOutput(gpio_node_t *node, String newValue);    
};

#endif
