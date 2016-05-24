#include "iotpipe_gpio.h"
#include "iotpipe_utils.h"
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
bool IotPipe_GPIO::isPortNameValid(char *portName, int type)
{
	//if portname is empty then user is asking for one to be auto-assigned.
	if( strcmp(portName,"")==0)
		return true;


  for(int i = 0; i < 18; i++)
  {
    if ( !isValidGPIO(i) )
    {
      continue;
    }
    if ( strcmp(gpios[i].portName, portName) == 0 & gpios[i].gpio_type == type )
    {
      return false;
    }      
  }
	return true;
}


bool IotPipe_GPIO::setPortAsDigitalInput(int portNum, char *portName)
{
	
	if( !isValidGPIO(portNum) )
	{
		LOG_DEBUG_ARGS("GPIO%d is not valid.",portNum);
		return false;
	}
	
	if ( !isPortNameValid(portName,DIGITAL_INPUT) )	
	{
		LOG_DEBUG_ARGS("Failed to set GPIO%d as input.  Portname of (%s) is already assigned to a port of type %d", portNum, portName, DIGITAL_INPUT);
		return false;
	}	
	
	addNode(portNum, portName, DIGITAL_INPUT);
 
	pinMode(portNum, INPUT);           // set pin to input
  digitalWrite(portNum, HIGH);       // turn on pullup resistors
	
	return true;
}

bool IotPipe_GPIO::setPortAsAnalogInput(char *portName)
{
	
	if ( !isPortNameValid(portName,ANALOG_INPUT) )	
	{
		LOG_DEBUG_ARGS("Failed to set ADC as input.  Portname of (%s) is already assigned to a port of type %d",  portName, DIGITAL_INPUT);
		return false;
	}	
	
	
	addNode(17,portName,ANALOG_INPUT);
	
	return true;
}

bool IotPipe_GPIO::setPortAsDigitalOutput(int portNum, char *portName)
{
	if( !isValidGPIO(portNum) )
	{
		LOG_DEBUG_ARGS("GPIO%d is not valid.",portNum);
		return false;
	}

	if ( !isPortNameValid(portName, DIGITAL_OUTPUT) )	
	{
		LOG_DEBUG_ARGS("%s is already assigned to a port of type %d", portName, DIGITAL_OUTPUT);
		return false;
	}

	addNode(portNum, portName, DIGITAL_OUTPUT);
	
	pinMode(portNum,OUTPUT);
  digitalWrite(portNum,HIGH);

	return true;


}

void IotPipe_GPIO::addNode(int portNumber, char *portName, int type)
{
  char buf2[max_portname_length];
	
	if(type==DIGITAL_INPUT | type==DIGITAL_OUTPUT)
	{

		//If user doesn't specify port name, then create one of the form  "GPION" where N is the port number
		if(strcmp(portName,"")==0)
		{
			char buf1[max_portname_length];
			char buf2[max_portname_length];
      
			flatten_string(buf1,max_portname_length);
			flatten_string(buf2,max_portname_length);

			itoa(portNumber,buf1,10);		
			strcat(buf2,"GPIO");
			strcat(buf2,buf1);			
		}
	}
	else if(type==ANALOG_INPUT)
	{
		//If user doesn't specify an analog_input port name then they are given "ADC"
		if(strcmp(portName,"")==0)
		{
			flatten_string(buf2,max_portname_length);
			strcpy(buf2,"ADC");
		}
	}
  flatten_string(gpios[portNumber].portName,max_portname_length);
  if( strcmp(portName,"") == 0 )
    strcpy( gpios[portNumber].portName, buf2 );
  else
    strcpy( gpios[portNumber].portName, portName );   
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
      LOG_DEBUG_ARGS("INPUT Port %d: %s", i, gpios[i].portName);
    else if(gpios[i].gpio_type==DIGITAL_OUTPUT)
      LOG_DEBUG_ARGS("OUTPUT Port %d: %s", i, gpios[i].portName);
    else if(gpios[i].gpio_type==ANALOG_INPUT)
      LOG_DEBUG_ARGS("ADC Port %d: %s", i, gpios[i].portName);      
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
      LOG_DEBUG_ARGS("INPUT Port %d: %s -- %d", i, gpios[i].portName, gpios[i].value);

    else if(gpios[i].gpio_type==ANALOG_INPUT)
      LOG_DEBUG_ARGS("ADC Port %d: %s -- %d", i, gpios[i].portName, gpios[i].value);      
      
  }  
}


bool IotPipe_GPIO::jsonifyInputScan(char *buf, const int bufLength)
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

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  for(int i = 0; i < 18; i++)
  {
    if( !isValidGPIO(i) )
      continue;

    if(gpios[i].active == false)
      continue;
            
    root[gpios[i].portName] = gpios[i].value;    
  }
  root.set<long>("time", 1351824120);

  int len = root.measureLength();


  
  if(len > bufLength)
  {
    LOG_DEBUG("Error.  The JSON payload cannot fit into the allocated json buffer.");
    return false;
  }
  root.printTo(buf,bufLength);
  return true; 
}

/*
//Scans MQTT payload and checks if any OUTPUT port is mentioned
bool IotPipe_GPIO::gpio_update_outputs(char *jsonString)
{

	jsmn_parser parser;
	jsmntok_t tokens[max_jsmn_tokens];

	jsmn_init(&parser);

	//r is the # of tokens returned.
	int r = jsmn_parse(&parser, jsonString, strlen(jsonString), tokens, max_jsmn_tokens);
	//if r < 1 or the first token isn't a JSMN_OBJECT
	if (r < 1 || tokens[0].type!=JSMN_OBJECT)
	{
		LOG_DEBUG("Nothing to be updated.");
		return true;
	}

	int i = 0;
	gpio_node_t *node = gpio_head->next;
	while(node!=NULL)
	{
		if(node->gpio_type==DIGITAL_OUTPUT)
		{
			//start at i = 1, since i = 0 is the JSMN_OBJECT token (aka the root)
			for(i = 1; i < r; i++)
			{
				LOG_DEBUG_ARGS("i: %d",i);
				if ( jsoneq(jsonString, &tokens[i],node->portName) == 0)
				{	
					//We may use strndup() to fetch string value 
					int bufLen = tokens[i+1].end - tokens[i+1].start + 1;

					char *buf = (char *)os_zalloc( sizeof(char)*bufLen );
					if(buf==NULL)
					{
						LOG_DEBUG_ARGS("Couldn't allocate memory.  Needed %d bytes.", sizeof(char)*bufLen);
						return false;
					}
			
					int k,z=0;
					for(k=tokens[i+1].start; k < tokens[i+1].end; k++)
					{
						buf[z]=jsonString[k];
						z++;
					}
					buf[z]='\0';
					updateOutput(node,buf);
					os_free(buf);
					break;
				}
			}
		}
		node=node->next;
	}

	return true;
}

void IotPipe_GPIO::updateOutput(gpio_node_t *node, char *newValue)
{

	if( strcmp(newValue,"low")==0 | strcmp(newValue,"high")==0 )
	{
		LOG_DEBUG("Updating output:");
		LOG_DEBUG_ARGS("\t%s: %s-->%s", node->portName, node->value ? "high" : "low", newValue);
	}
	else
	{
		LOG_DEBUG_ARGS("Cannot update output because %s isn't a valid value.", newValue);
	}

	if(strcmp(newValue,"low")==0)
	{
		node->value = 0;
		set_gpio_output_low(node->portNumber);
	}
	else if(strcmp(newValue,"high")==0)
	{
		node->value = 1;
		set_gpio_output_high(node->portNumber);
	}	
}


int IotPipe_GPIO::jsoneq(const char *json, jsmntok_t *tok, const char *s) 
{
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start && strncmp(json + tok->start, s, tok->end - tok->start) == 0) 
	{
		return 0;
	}
	return -1;
}
*/
