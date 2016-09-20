/*
Much thanks to knolleary for his PubSubClient, from which much of this example was built.
*/


/*
 * This Tutorial will allow you to sample Digital GPIOs on your ESP8266 and send the values to the IoT Pipe website.  The an introductory tutorial can be found at http://www.iotpipe.io/esp8266
*/


#include <ESP8266WiFi.h>
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



WiFiClient espClient;
PubSubClient client(espClient);
IotPipe iotpipe(deviceId);



//This is our initial setup.
//We connect to Wi-Fi and setup our connection to the IoT Pipe server.
void setup() 
{
	Serial.begin(115200);

	//Register an output with IoT Pipe so we can update it when we receive an MQTT message
	iotpipe.registerSensor(4,"LED");



	setup_wifi();
	client.setServer(server, port);
	client.setCallback(message_received);
}

void loop()
{
	if (!client.connected()) 
	{
		reconnect();
	}
	client.loop();
	delay(100);
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
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");
	for (int i = 0; i < length; i++) {
	Serial.print((char)payload[i]);
	}
	Serial.println();
	String msg = String( reinterpret_cast< char const* >(payload) ) ;
	String top = String(topic);

	//This will parse the message, and turn the the LED on or off.
	iotpipe.updateOutputs(top, msg);
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
		// Attempt to connect
		if (client.connect("ESP8266Client",mqtt_user,mqtt_pass)) 
		{
			Serial.println("connected");
			// ... and resubscribe
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
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}

