#ifndef IOTPIPE_GPIO_H_
#define IOTPIPE_GPIO_H_

const int max_portname_length = 16;
const int max_jsmn_tokens = 128;

//Node for the linked list that stores all of the GPIOs set as inputs
typedef struct input_node
{
	int portNumber;
	char portName[max_portname_length];
	int gpio_type;
	int value;
	bool active;
	input_node(int pn=-1) : portNumber(pn), gpio_type(-1), value(-1), active(false) {}

} gpio_node_t;

typedef enum {DIGITAL_INPUT, ANALOG_INPUT, DIGITAL_OUTPUT} gpio_mode;


const int gpio_mask[] = {1,0,1,0,1,1,0,0,0,0,0,0,1,1,1,1,0,1}; //valid gpios are 0,2,4,5,12,13,14,15.  Final spot is for ADC.

class IotPipe_GPIO
{
	public:
		IotPipe_GPIO();
		~IotPipe_GPIO();

    void print();
    void print_input_values();

    bool setPortAsDigitalInput(int portNum, char *portName);		
		bool setPortAsAnalogInput(char *portName);
		bool setPortAsDigitalOutput(int portNum, char *portName);
        
		//updates value field in each node of gpio_head
		bool jsonifyInputScan(char *buf, int bufLength);
		
		//bool gpio_update_outputs(char *jsonString);
	private:
		bool isPortNameValid(char *portName, int type);  
		bool isValidGPIO(int portNum);  
		gpio_node_t gpios[18]; //17 (0 to 16) gpios on ESP8266 + 1 ADC
    //GPIOs of the same type cannot have the same name
    
		void addNode(int pin, char *portName, int type);
		
		/*
		//Checks if port # is a valid GPIO for the ESP8266
		//sets output pins high or low
		int jsoneq(const char *json, jsmntok_t *tok, const char *s);
		void updateOutput(gpio_node_t *node, char *newValue);
		*/
};

#endif
