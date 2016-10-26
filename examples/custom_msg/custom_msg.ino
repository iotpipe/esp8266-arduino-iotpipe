/*
Much thanks to knolleary for his PubSubClient, from which much of this example was built.
*/


/*
 * This tutorial shows you how to receive custom messages sent from IoT Pipe.  Specifically, we will flash an LED by defining the number of flashes on the fly, through IoT Pipe. 

   A typical, custom MQTT message from IoT Pipe looks like this:
	{ 
          "message_type" : "user_defined" ,
	  "message_body" : <Anything you like here> 
	}

   The message_body can be anything defined by the user and can be used to perform custom logic.  For example, use it to communicate with an i2c device, set a message on an LCD screen, sound a buzzer for some amount of time, etc.

   In this tutorial, the value of message_body looks like this:
	{
	  "delay" : <an integer>
	  "num_flashes" : <an integer>
	}

*/


#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <iotpipe.h>

// Update these with suitable values.
const char* ssid = "PLACEHOLDER";
const char* password = "PLACEHOLDER";
const char* deviceId = "PLACEHOLDER";
const char* mqtt_user = "PLACEHOLDER";
const char* mqtt_pass = "PLACEHOLDER";
const char* server = "broker.iotpipe.io";
const int port = 1883;

void myMsgRcvd(String msg);

WiFiClient espClient;
PubSubClient client(espClient);
IotPipe iotpipe(deviceId,customMsgCallback);

const int ledPin = 4;
//This is our initial setup.
//We connect to Wi-Fi and setup our connection to the IoT Pipe server.
void setup() 
{
	Serial.begin(115200);
	pinMode(ledPin, OUTPUT);
	setup_wifi();
	client.setServer(server, port);
	client.setCallback(message_received);
}

void loop() 
{
	if (!client.connected()) 
		reconnect();
	client.loop();
	delay(100);
}


void customMsgCallback(String customMsgBody)
{
	DynamicJsonBuffer jsonBuffer;
	JsonObject &root = jsonBuffer.parseObject(customMsgBody);

	int delayInMS = root["delay"].as<int>();
	int numFlashes = root["num_flashes"].as<int>();

	for(int i = 0; i < numFlashes; i++)
	{
		digitalWrite(ledPin, HIGH);
		delay(delayInMS);
		digitalWrite(ledPin,LOW);
		delay(delayInMS);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//              Beyond this point is functionality for Wi-Fi connectivity and MQTT messaging.  Don't touch unless you know what you are doing. :)
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//This function is called when a message is received from the server
void message_received(char* topic, byte* payload, unsigned int length) 
{
	String msg = String( reinterpret_cast< char const* >(payload) ) ;
	String top = String(topic);

	Serial.print("Message received: ");
	Serial.println(msg);

	//This will parse the message.  If the message_type is "user_defined" then it will call our myMsgRcvd callback
	iotpipe.messageHandler(top, msg);
}

void setup_wifi() 
{
	delay(10);
	// We start by connecting to a WiFi network
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);


	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) 
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}

void reconnect() 
{
	// Loop until we're reconnected
	while (!client.connected()) 
	{
		Serial.print("Attempting MQTT connection...");
		if (client.connect("ESP8266Client",mqtt_user,mqtt_pass)) 
		{
			Serial.println("connected");
			String topic;
			iotpipe.getOutputTopic(topic);
			Serial.println(topic.c_str());
			client.subscribe(topic.c_str());
		} 
		else 
		{
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			delay(5000);
		}
	}
}
