<h1>ESP8266 Library's for the IoT Pipe web service</h1>

<h2>What is IoT Pipe</h2>
IoT Pipe is a web service which makes it easy for you, the DIY-er, to connect your projects to the internet.  Our APIs make it easy to send and receive data between your project and our web service.  You can send and analyze your data, setup alerts and notifications, and much more.

<h2>How does it work</h2>
IoT Pipe uses MQTT, which is the language the Internet of Things uses to communicate. It is a lightweight messaging protocoll that makes it easy to communicate between computers and devices.

IoT Pipe has built a web interface and API on top of MQTT which makes it easy for users to send and receive data between their devices and our web service.  With this data our web service makes it easy to graph and analyze your data, setup alerts and notifications, and much more.


<h2>Built on-top of the ESP8266/Arduino library</h2>
Use this API to connect your ESP8266 to IoT Pipe.  You can find more detailed tutorials, with pictures and videos for the ESP8266 at the site linked below.

<h3>https://iotpipe.io/esp8266</h3>


<h2>Dependencies (available through Arduino's Library Manager):</h2>
<ul>
<li>ArduinoJSON</li>
<li>PubSubClient</li>
<li>ESP8266 Board Manager</li>
</ul>


<h2>Usage</h2>
In Input mode the ESP8266 can gather data from sensors and send them to the IoT Pipe web service. This is accomplished by periodically scanning selected GPIOs and the ADC of the device.

It is best to start with one of our examples.  For example, the 'digital sensor' example shows how to samples GPIOs and send data to IoT Pipe.

At the core all IoT Pipe programs is the following:

<ul>
<li>An IoT Pipe object.</li>
<li>An MQTT library which can publish and subscribe to messages.  Our examples use the PubSubClient.</li>
<li>A Wi-Fi library which can connect to the internet.  We use the Arduino/ESP8266 library.</li>
</ul>

Each of the above is covered in our examples.

<h3>Sending a single data value to IoT Pipe Service (Using PubSubClient)</h3>
The below snippet will send data from a single sensor to the IoT Pipe service.  This code is typically run inside of the loop() function and already assumes the user has connected to WiFi and to the MQTT service using the PubSubClient library.
```
c++
String topic, payload;
//Use the IoT Pipe library to get the correct MQTT Topic
iotpipe.getSamplingTopic(topic);

int val = analogRead(A0);

iotpipe.jsonifyResult( val , sensorName, payload);   
if(payload.length()>0)
client.publish(topic.c_str(),payload.c_str(),payload.length());
```
The above code will send a JSON payload to IoT Pipe Service.  The JSON payload will look something like this:
{"timestamp": 1475543846435, "sunlight" : 483} where the timestamp is given in milli-seconds of epoch time and our sensor name is 'sunlight'.

<h3>Sending multiple data values to IoT Pipe Service (Using PubSubClient)</h3>
The below snippet will send data from multiiple sensors to the IoT Pipe service.  This code is typically run inside of the loop() function and already assumes the user has connected to WiFi and to the MQTT service using the PubSubClient library.
```
c++
String topic, payload;
//Use the IoT Pipe library to get the correct MQTT Topic
iotpipe.getSamplingTopic(topic);

int vals[3];
String names[3];

vals[0] = digitalRead(sensorPin_1);
vals[1] = digitalRead(sensorPin_2);
vals[2] = analogRead(A0);
names[0] = sensorName1;
names[1] = sensorName1;
names[2] = sensorName2;

iotpipe.jsonifyResult( vals , names, 3, payload);   
if(payload.length()>0)
client.publish(topic.c_str(),payload.c_str(),payload.length());
```
<h3>Sending batched data to IoT Pipe</h3>
Sometimes it is necessary to store sensor readings for some time before sending to the IoT Pipe service.  This can be done using the ArduinoJSON library.  The IoT Pipe service expects to see batched data as a JSON array whose name is 'data'.

For example,
```
json
{
	"data": [
	  {"timestamp": 1455598505401.151,"humidity": 35.0}, 
	  {"timestamp": 1455598804178.124,"humidity": 35.0}, 
	  {"timestamp": 1455599105018.538,"humidity": 36.0}, 
	  {"timestamp": 1455599404337.2979,"humidity": 35.0}, 
	  {"timestamp": 1455599703800.3171,"humidity": 35.0}, 
	  {"timestamp": 1455600013769.206,"humidity": 35.0}
	]
}
```

<h3>Communicating with your project from the IoT Pipe web app</h3>
The IoT Pipe web service can communicate with your project via the MQTT protocol.  With this communication you can drive GPIOs on your device high and low from the IoT Pipe web app.

Note the below example won't compile.  It is meant to give an overview of how things work.   Go to our Blinky example for a working example.

```
c++
//In setup() designate which GPIOs on your device can be controlled from IoT Pipe
IoTPipe iotpipe("DEVICEID");    //Use your IoT Pipe device id.
void setup() {

  //This will allow IoT Pipe to control GPIO4 and GPIO5 on your device.  We give the GPIOs names for easier access.
  iotpipe.registerSensor(4,"LED");  
  iotpipe.registerSensor(5,"Buzzer");   


  //Using PubSubClient we register the message_received callback
  pubSubClient.setCallback(message_received);

  //Other setup for project goes here...
  
}

void message_received(char* topic, byte* payload, unsigned int length) 
{
	String msg = String( reinterpret_cast< char const* >(payload) ) ;
	String top = String(topic);

	//This will parse the message and updat GPIOs as necessary
	iotpipe.updateOutputs(top, msg);
}
```

<h2>More information</h2>
For more information, including tutorials, getting started guides, and videos check out <a href="www.iotpipe.io/howitworks">www.iotpipe.io/howitworks</a>.
