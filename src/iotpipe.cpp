#include "iotpipe.h"

IotPipe::IotPipe(String deviceId, void (*udf_cb)(String))
{
        this->samplingTopic = "sampling/" + deviceId;
        this->outputTopic = "outputport/" + deviceId;
	this->realTimeTopic = "realtime/" + deviceId;

	udfMsgRcvd_cb = udf_cb;
}

IotPipe::IotPipe(const char* deviceId, void (*udf_cb)(String))
{
        this->samplingTopic = "sampling/" + String(deviceId);
        this->outputTopic = "outputport/" + String(deviceId);
	this->realTimeTopic = "realtime/" + String(deviceId);

	udfMsgRcvd_cb = udf_cb;
}

IotPipe::~IotPipe()
{
        //do nothing
}

//Adds a sensor to the list of outputs which can receive messages
void IotPipe::registerSensor(int pin, String name)
{
	gpio.registerSensor(pin,name);
}

//Reads a JSON payload from IoT Pipe web service that contains the desired values of output ports.
void IotPipe::messageHandler(String topic, String msg)
{
	if( !topic.equals(this->outputTopic) )
		return;

	DynamicJsonBuffer jsonBuffer;
	JsonObject &root = jsonBuffer.parseObject(msg);

	if(!root.success())
	{
		LOG_DEBUG("Failed to parse msg in messageHandler.");
		return;
	}

	String msgType = String(root["message_type"].asString());
	
	if( msgType == "update_outputs")
		this->updateOutputs(msg);
	if( msgType == "user_defined")
		this->userDefinedMsg(msg);
}

void IotPipe::updateOutputs(String msg)
{
	gpio.gpioUpdateOutputs(msg);
}


//todo: only pass user the pertinent part of the message
void IotPipe::userDefinedMsg(String msg)
{
	DynamicJsonBuffer jsonBuffer;
	JsonObject &root = jsonBuffer.parseObject(msg);

	if(!root.success())
	{
		LOG_DEBUG("Failed to parse msg in messageHandler.");
		return;
	}

	String msg_body = root["message_body"].as<String>();

	if (udfMsgRcvd_cb!=NULL)
		(*udfMsgRcvd_cb)(msg_body);
	else
		LOG_DEBUG("No callback defined for user defined messages.");
}


