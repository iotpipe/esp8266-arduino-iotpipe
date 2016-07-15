
/*
Much thanks to knolleary for his PubSubClient, from which much of this example was built.
*/


/*
 * This Tutorial will allow you to turn an LED on and off from the IoT Pipe website.  The tutorial can be found at http://www.iotpipe.io/esp8266arduinointro
*/


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "iotpipe.h"

// Update these with values suitable for your network.
const char* ssid = "PLACEHOLDER";
const char* password = "PLACEHOLDER";
const char* deviceId = "PLACEHOLDER";
const char* mqtt_user = "PLACEHOLDER";
const char* mqtt_pass = "PLACEHOLDER";

WiFiClient espClient;
PubSubClient client(espClient);
IotPipe iotpipe(deviceId);


//Choose a GPIO to connect to the LED.  Here we are using GPIO4 but you can change it.
void setup_iotpipe()
{
  iotpipe.addDigitalOutputPort(4,"LED");
}

//This is our initial setup.
//We connect to Wi-Fi and setup our connection to the IoT Pipe server.
void setup() { 
  Serial.begin(115200);
  setup_iotpipe();
  setup_wifi();

 
  client.setServer("broker.iotpipe.io",1883);
  client.setCallback(message_received);
}

//This loop runs indefinitely and does the following:
//Checks if we are connected to IoT Pipe server
//If we aren't connected, then reconnect.
void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
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
void message_received(char* topic, byte* payload, unsigned int length) {
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
  iotpipe.update_outputs(top, msg);
}

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
      // ... and resubscribe
      String topic;
      iotpipe.get_output_topic(topic);
      client.subscribe(topic.c_str());
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
