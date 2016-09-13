#include "iotpipe_gpio.h"
#include "iotpipe_sntp.h"


using namespace IotPipe_Utils;

//we pick a different name to not be confused with gpio_init() in gpio.h
IotPipe_GPIO::IotPipe_GPIO()
{
}

IotPipe_GPIO::~IotPipe_GPIO()
{
	//do nothing
}



//Scans MQTT payload and checks if any OUTPUT port is mentioned
bool IotPipe_GPIO::gpioUpdateOutputs(String msg)
{

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(msg);

	//check json is valid  
	if (!root.success()) 
	{
		LOG_DEBUG_ARGS("JSON received is not valid");
		return false;
	}
	return true;
}


void IotPipe_GPIO::updateOutput(int pin, String newValue)
{
	newValue.toLowerCase();
	if( newValue.equals("low") | newValue.equals("high") | newValue.equals("flip") )
	{
    		LOG_DEBUG("Updating output:");
		if ( newValue.equals("flip") )
		{
			if ( node->value==0 )
				LOG_DEBUG_ARGS("\t%s: low-->high", node->portName.c_str());    
			else
				LOG_DEBUG_ARGS("\t%s: high-->low", node->portName.c_str());    
		}
		else
		{
			LOG_DEBUG_ARGS("\t%s: %s-->%s", node->portName.c_str(), node->value ? "high" : "low", newValue.c_str());  
		}
		
	}
	else
	{
		LOG_DEBUG_ARGS("Cannot update output because %s isn't a valid value.", newValue.c_str());
	}

	if( newValue.equals("low") )
	{
		node->value = 0;
		digitalWrite(node->portNumber, LOW);
	}
	else if( newValue.equals("high") )
	{
		node->value = 1;
		digitalWrite(node->portNumber, HIGH);
	}	
	else if( newValue.equals("flip") )
	{
		if(node->value==1)
		{      
			node->value=0;
			digitalWrite(node->portNumber, LOW);
		}
		else
		{
			node->value=1;
			digitalWrite(node->portNumber, HIGH);      
		}
	}
}

