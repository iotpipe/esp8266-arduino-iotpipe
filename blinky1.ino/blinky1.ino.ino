
/*
  Basic ESP8266 MQTT example
  This sketch demonstrates the capabilities of the pubsub library in combination
  with the ESP8266 board/library.
  It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off
  It will reconnect to the server if the connection is lost using a blocking
  reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
  achieve the same result without blocking the main loop.
  To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Arduino.h"
#include "iotpipe.h"
#include "iotpipe_utils.h"

using namespace IotPipe_Utils;

// Update these with values suitable for your network.
//const char* ssid = "CenturyLink2926";
//const char* password = "deec634bbfrbfe";
const char *ssid = "Adam’s iPhone";                    
const char *password = "asdftemp";
const char* mqtt_server = "broker.iotpipe.io";
const char* deviceId = "a75dfbb18d71a7f";
const char* mqtt_user = "e59ab9c78622f2689e1f533ffbdf78d";
const char* mqtt_pass = "5cf3f075d166cea7e59e8116c65185e";


WiFiClient espClient;
PubSubClient client(espClient);
IotPipe iotpipe(deviceId);

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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  String msg = String( reinterpret_cast< char const* >(payload) ) ;
  String top = String(topic);

  iotpipe.update_outputs(top, msg);

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

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);

  iotpipe.addDigitalOutputPort(4, "R_LED");
  iotpipe.addDigitalOutputPort(5, "G_LED");
  iotpipe.addDigitalInputPort(12,"Temperature");
  iotpipe.addDigitalInputPort(13,"Humidity");
  iotpipe.addAnalogInputPort("Light");

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop()
{

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  String buf;
  iotpipe.scan(buf);
  String topic;
  iotpipe.get_sampling_topic(topic);
  client.publish(topic.c_str(), buf.c_str());
  delay(3000);
}
