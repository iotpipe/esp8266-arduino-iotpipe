#include "iotpipe_gpio.h"
#include "iotpipe_utils.h"
#include "iotpipe_sntp.h"

#include <ArduinoJson.h>

using namespace IotPipe_Utils;

//we pick a different name to not be confused with gpio_init() in gpio.h
IotPipe_GPIO::IotPipe_GPIO()
{
	//ESP8266 has a list of valid GPIOS contained in gpio_definitions.h
	//we store a gpio_node_t struct at the index corresponding to each valid GPIO and a NULL otherwise.

	for(int i = 0; i < 17; i++)
	{
		if ( isValidGPIO(i) )
		{
			gpios[i] = gpio_node_t(i);
		}
		else
		{
      gpios[i]=gpio_node_t();
		}
	}	
	//now do the ADC
  gpios[17] = gpio_node_t(17);
  gpios[17].gpio_type=ANALOG_INPUT;
}

IotPipe_GPIO::~IotPipe_GPIO()
{
	//do nothing
}



//Checks if port # is a valid GPIO for the ESP8266
bool IotPipe_GPIO::isValidGPIO(int portNum)
{
	return gpio_mask[portNum];
}

//GPIOs of the same type cannot have the same name
bool IotPipe_GPIO::isPortNameValid(String portName, int type)
{
	//if portname is empty then user is asking for one to be auto-assigned.
	if( portName.equals("")==0)
		return true;


  for(int i = 0; i < 18; i++)
  {
    if ( !isValidGPIO(i) )
    {
      continue;
    }
    if ( gpios[i].portName.equals(portName) & gpios[i].gpio_type == type )
    {
      return false;
    }      
  }
	return true;
}


bool IotPipe_GPIO::setPortAsDigitalInput(int portNum, String portName)
{
	
	if( !isValidGPIO(portNum) )
	{
		LOG_DEBUG_ARGS("GPIO%d is not valid.",portNum);
		return false;
	}
	
	if ( !isPortNameValid(portName,DIGITAL_INPUT) )	
	{
		LOG_DEBUG_ARGS("Failed to set GPIO%d as input.  Portname of (%s) is already assigned to a port of type %d", portNum, portName.c_str(), DIGITAL_INPUT);
		return false;
	}	
	
	addNode(portNum, portName, DIGITAL_INPUT);
 
	pinMode(portNum, INPUT);           // set pin to input
	digitalWrite(portNum, HIGH);       // turn on pullup resistors
	
	return true;
}

bool IotPipe_GPIO::setPortAsAnalogInput(String portName)
{
	
	if ( !isPortNameValid(portName,ANALOG_INPUT) )	
	{
		LOG_DEBUG_ARGS("Failed to set ADC as input.  Portname of (%s) is already assigned to a port of type %d",  portName.c_str(), DIGITAL_INPUT);
		return false;
	}	
	
	
	addNode(17,portName,ANALOG_INPUT);
	
	return true;
}

bool IotPipe_GPIO::setPortAsDigitalOutput(int portNum, String portName)
{
	if( !isValidGPIO(portNum) )
	{
		LOG_DEBUG_ARGS("GPIO%d is not valid.",portNum);
		return false;
	}

	if ( !isPortNameValid(portName, DIGITAL_OUTPUT) )	
	{
		LOG_DEBUG_ARGS("%s is already assigned to a port of type %d", portName.c_str(), DIGITAL_OUTPUT);
		return false;
	}

	addNode(portNum, portName, DIGITAL_OUTPUT);
	
	pinMode(portNum,OUTPUT);
	digitalWrite(portNum,HIGH);

	return true;


}

void IotPipe_GPIO::addNode(int portNumber, String portName, int type)
{

  if( portName.equals("") )
  {
    String autoName;
    if(type==DIGITAL_INPUT | type==DIGITAL_OUTPUT)  
      autoName = "GPIO" + String(portNumber);
    else if(type==ANALOG_INPUT)
      autoName = "ADC";

    gpios[portNumber].portName = autoName;
  }
  else
  {
    gpios[portNumber].portName = portName;
  }
  gpios[portNumber].gpio_type = type;
  gpios[portNumber].active = true;
}

void IotPipe_GPIO::print()
{
  for(int i = 0; i < 18; i++)
  {
    if( !isValidGPIO(i) )
      continue;

    if(gpios[i].active==false)
      continue;

    if(gpios[i].gpio_type==DIGITAL_INPUT)
      LOG_DEBUG_ARGS("INPUT Port %d: %s", i, gpios[i].portName.c_str());
    else if(gpios[i].gpio_type==DIGITAL_OUTPUT)
      LOG_DEBUG_ARGS("OUTPUT Port %d: %s", i, gpios[i].portName.c_str());
    else if(gpios[i].gpio_type==ANALOG_INPUT)
      LOG_DEBUG_ARGS("ADC Port %d: %s", i, gpios[i].portName.c_str());      
  } 
}

void IotPipe_GPIO::print_input_values()
{
  for(int i = 0; i < 18; i++)
  {
    if( !isValidGPIO(i) )
      continue;

    if(gpios[i].active==false)
      continue;

    if(gpios[i].gpio_type==DIGITAL_INPUT)
      LOG_DEBUG_ARGS("INPUT Port %d: %s -- %d", i, gpios[i].portName.c_str(), gpios[i].value);

    else if(gpios[i].gpio_type==ANALOG_INPUT)
      LOG_DEBUG_ARGS("ADC Port %d: %s -- %d", i, gpios[i].portName.c_str(), gpios[i].value);      
      
  }  
}


bool IotPipe_GPIO::jsonifyInputScan(String& buf)
{

  for(int i = 0; i < 18; i++)
  {

    if( !isValidGPIO(i) )
      continue;

    if (gpios[i].gpio_type==DIGITAL_INPUT)
      gpios[i].value = digitalRead(i);
    else if(gpios[i].gpio_type==ANALOG_INPUT)
      gpios[i].value = analogRead(i);
    
  }
  
  StaticJsonBuffer<max_json_payload_length> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  for(int i = 0; i < 18; i++)
  {
    if( !isValidGPIO(i) )
      continue;

    if(gpios[i].active == false)
      continue;
    if(gpios[i].gpio_type==ANALOG_INPUT | gpios[i].gpio_type==DIGITAL_INPUT)            
      root[gpios[i].portName] = gpios[i].value;    
  }
  IotPipe_SNTP timeGetter;



  if(timeGetter.getEpochTimeInSeconds() == 0)
  {
    LOG_DEBUG_ARGS("Haven't yet connected to NTP Server.");
    return false;
  }
  LOG_DEBUG_ARGS("time in seconds: %d", timeGetter.getEpochTimeInSeconds());

  //We cast time to a string so we can append three 0's.  This is because we can't handle longs on the arduino.
  String timeBuf = String(timeGetter.getEpochTimeInSeconds());
  timeBuf = timeBuf + "000";

  root.set<String>("timestamp", timeBuf);

  int len = root.measureLength();


  
  if(len > max_json_payload_length)
  {
    LOG_DEBUG("Error.  The JSON payload cannot fit into the allocated json buffer.");
    return false;
  }
  root.printTo(buf);
  return true; 
}


//Scans MQTT payload and checks if any OUTPUT port is mentioned
bool IotPipe_GPIO::gpio_update_outputs(String msg)
{

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(msg);

	//check json is valid  
	if (!root.success()) 
	{
		LOG_DEBUG_ARGS("JSON received is not valid");
		return false;
	}


	//Iterate through all GPIOs.  For each GPIO see if it is included in json string
	for(int i = 0; i < 18; i++)
	{
		if(gpios[i].gpio_type==DIGITAL_OUTPUT)
		{
			//start at i = 1, since i = 0 is the JSMN_OBJECT token (aka the root)
			for (JsonObject::iterator it=root.begin(); it!=root.end(); ++it)
		 	{
		  		if( gpios[i].portName.equals(it->key) )
		  		{
					const char* c_val = root[it->key];
					String value( c_val ); //Service always returns a string as the json value.
					updateOutput(&gpios[i],value);
					LOG_DEBUG_ARGS("Output %s to be updated", gpios[i].portName.c_str());
					break;
				}
		 	}
	  	}
	}	
	return true;
}


void IotPipe_GPIO::updateOutput(gpio_node_t *node, String newValue)
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

