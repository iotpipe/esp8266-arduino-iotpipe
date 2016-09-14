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

void IotPipe_GPIO::registerSensor(int pin, String sensorName)
{
	if(pin<0)
	{
		LOG_DEBUG("Cannot have pin with value less than 0.");
		return;
	}

	if(pin>max_sensors-1)
	{
		LOG_DEBUG("Pin value exceeds max number of pins.");
		return;
	}
	
	if(sensorName=="")
	{
		LOG_DEBUG("Cannot have a blank sensor name.");
		return;
	}

	if(sensorName=="timestamp")
	{
		LOG_DEBUG("timestamp is not a valid sensor name.");
		return;
	}

	if ( findSensor(sensorName) == -1)
	{
		registeredSensors[pin]=sensorName;
		pinMode(pin, OUTPUT);
	}
	else
	{	
		LOG_DEBUG_ARGS("There is already a registered sensor with the name %s", sensorName.c_str());
		return;
	}
	
}

int IotPipe_GPIO::findSensor(String name)
{
	for(int i = 0; i < max_sensors; i++)
	{
		Serial.print(registeredSensors[i]);
		Serial.print(" : ");
		Serial.println(name);
		if( registeredSensors[i] == name )
		{
			return i;
		}
	}

	return -1;	
}

//Scans MQTT payload and checks if any OUTPUT port is mentioned
void IotPipe_GPIO::gpioUpdateOutputs(String msg)
{
	Serial.println("updating................");
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(msg);

	//check json is valid  
	if (!root.success()) 
	{
		LOG_DEBUG_ARGS("JSON received is not valid");
		return;
	}


	//start at i = 1, since i = 0 is the JSMN_OBJECT token (aka the root)
	int pin;
	for (JsonObject::iterator it=root.begin(); it!=root.end(); ++it)
	{
		Serial.println( String(it->key) );
		pin = this->findSensor( String(it->key) );
		if( pin != -1 )
		{
			Serial.println("Found pin.");
			const char* c_val = root[it->key];
			String value( c_val ); //Service always returns a string as the json value.
			updateOutput(pin,value);
		}
	}	

}


void IotPipe_GPIO::updateOutput(int pin, String newValue)
{

	int oldVal = digitalRead(pin);


	newValue.toLowerCase();
	if( newValue.equals("low") | newValue.equals("high") | newValue.equals("flip") )
	{
    		LOG_DEBUG("Updating output:");
		if ( newValue.equals("flip") )
		{
			if ( oldVal==0 )
				LOG_DEBUG_ARGS("\t%d: low-->high", pin);    
			else
				LOG_DEBUG_ARGS("\t%d: high-->low", pin);    
		}
		else
		{
			LOG_DEBUG_ARGS("\t%d: %s-->%s", pin, oldVal ? "high" : "low", newValue.c_str());  
		}
		
	}
	else
	{
		LOG_DEBUG_ARGS("Cannot update output because %s isn't a valid value.", newValue.c_str());
	}

	if( newValue.equals("low") )
	{
		digitalWrite(pin, LOW);
	}
	else if( newValue.equals("high") )
	{
		digitalWrite(pin, HIGH);
	}	
	else if( newValue.equals("flip") )
	{
		if(oldVal==1)
		{      
			digitalWrite(pin, LOW);
		}
		else
		{
			digitalWrite(pin, HIGH);      
		}
	}
}

