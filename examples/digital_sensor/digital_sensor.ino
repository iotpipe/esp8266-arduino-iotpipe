/*
Much thanks to knolleary for his PubSubClient, from which much of this example was built.
*/


/*
 * This tutorial collects data from a GPIO and sends that information to IoT Pipe.  The tutorial can be found at http://www.iotpipe.io/esp8266
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



//Wait time between sensor readings.  Note this time is approximate.
const int waitTimeInMilliseconds = 10000; 

//Digita/ Pin # to be read and the name of the pin
const int sensorPin = 4;
const String sensorName="sensor";



WiFiClient espClient;
PubSubClient client(espClient);
IotPipe iotpipe(deviceId);


//This is our initial setup.
//We connect to Wi-Fi and setup our connection to the IoT Pipe server.
void setup() 
{
	Serial.begin(115200);
	setup_wifi();
	client.setServer(server, port);
}

//This loop runs indefinitely and does the following:
//Checks if we are connected to IoT Pipe server
//If we aren't connected, then reconnect.
long lastSampleTime = 0;

void loop()
{
	if (!client.connected()) {
		reconnect();
	}

	//Once per loop create a payload that contains information about all of your input ports so data can be sent to server.
	long curTime = millis();
	if(curTime - lastSampleTime > waitTimeInMilliseconds)
	{

		String topic, payload;
		iotpipe.getSamplingTopic(topic);
		lastSampleTime=curTime;     

		int val = digitalRead(sensorPin);

		iotpipe.jsonifyResult( val , sensorName, payload);   
		if(payload.length()>0)
		{
			Serial.print("Publishing payload: ");
			Serial.println(payload);  
			client.publish(topic.c_str(),payload.c_str(),payload.length());
		}

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

long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);


  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client",mqtt_user,mqtt_pass)) {
      Serial.println("connected");
      String realTimeTopic;
      iotpipe.getRealTimeTopic(realTimeTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

