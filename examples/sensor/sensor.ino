/*
Much thanks to knolleary for his PubSubClient, from which much of this example was built.
*/


/*
 * This Tutorial will allow you to sample GPIOs on your ESP8266 and send the valuesto the IoT Pipe website.  The tutorial can be found at http://www.iotpipe.io/esp8266arduinointro
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


//Choose a GPIO to connect to your sensor.  Here we are using GPIO4.
void setup_iotpipe()
{
  iotpipe.addDigitalInputPort(4,"Door");
}

//This is our initial setup.
//We connect to Wi-Fi and setup our connection to the IoT Pipe server.
void setup() {
  Serial.begin(115200);
  setup_iotpipe();
  setup_wifi();

  client.setServer("broker.iotpipe.io",1883);
}

//This loop runs indefinitely and does the following:
//Checks if we are connected to IoT Pipe server
//If we aren't connected, then reconnect.
void loop()
{
  if (!client.connected()) {
    reconnect();
  }

  //Once per loop create a payload that contains informatino about all of your input ports so data can be sent to server.
  String payload,topic;
  iotpipe.get_sampling_topic(topic);
  bool success = iotpipe.scan(payload);
  if(success==true)
  {
    Serial.println(payload);  
    client.publish(topic.c_str(),payload.c_str(),payload.length());
  }

  //Wait 2 seconds between data uploads
  delay(2000);    
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
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

