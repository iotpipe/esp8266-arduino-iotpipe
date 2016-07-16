 <h1>ESP8266 Library's for the IoT Pipe web service</h1>
 <h3>Built on-top of the ESP8266/Arduino library</h3>
 
 
Use this API to connect your ESP8266 to IoT Pipe.  With IoT Pipe you can easily send your data to the cloud where you can graph, analyze, and build alerts and notifications through our web interface.

<h3>https://www.iotpipe.io</h3>


<h2>Dependencies (available through Arduino's Library Manager):</h2>
<ul>
<li>ArduinoJSON</li>
<li>PubSubClient</li>
</ul>


<h2>Usage - Input mode</h2>
In Input mode the ESP8266 can gather data from sensors and send them to the IoT Pipe web service. This is accomplished by periodically scanning selected GPIOs and the ADC of the device.

First create an Iot Pipe object:

```IoTPipe iotpipe("DEVICEID"); ```

where "DEVICEID" is a unique ID for your device created through the IoT Pipe web interface.

To designate a port as an 'input port' call one of the following functions, depending on whether you want a digital or analog input:

```
bool success = iotpipe.addDigitalInputPort(int gpioNumber,String sensorName);  //Call this for digital GPIOs.  

bool success = iotpipe.addAnalogInputPort(String sensorName);  //Call this for the ADC
```

The <i>sensorName</i> variable must be unique across your project.

In order to scan the designated 'input ports' you can call

```
String payload = iotpipe.scan();
```

This will return a json payload which can be sent to the server.  The payload will look something like:
<b>{"timestamp":"1468690759000", "sensorName1": 0, "sensorName2": 1 }</b>

In order to publish this information to the server get the MQTT topic:

```
String topic = iotpipe.get_sampling_topic();
```

and use your MQTT libraries <i>Publish()</i> function.

Note that the IoT Pipe API doesn't include any MQTT functionality, however, our examples use the PubSubClient MQTT library which makes it easy to do so.  

<b>Follow our examples, then build up from there!</b>

<h2>Usage - Output mode</h2>
In Output mode the ESP8266 can receive data from the IoT Pipe webservice and act appropriately.  This is accomplished by scanning incoming MQTT messages for specific commands.  You can use the IoT Pipe web service to easily issue commands, alerts, and notifications which can be sent to your devices.

First create an Iot Pipe object:

```IoTPipe iotpipe("DEVICEID"); ```

where "DEVICEID" is a unique ID for your device created through the IoT Pipe web interface.

To designate a port as an 'out port' call onethe following functions:

```
bool success = iotpipe.addDigitalOutputPort(int gpioNumber,String sensorName);  //Call this only for digital GPIOs.  
```

To listen for messages from the IoT Pipe web service subscribe to the topic generated from:

```
String topic = iotpipe.get_sampling_topic();
```

As messages are received they are parsed by the IoT Pipe API.  Currently, messages have the ability to turn digital GPIOs on and off, or high and low.  

```
bool success = iotpipe.update_outputs(String topic, String message);
```

This function must be placed inside your MQTT libraries callback of when a message is received.  In our <b>blinky</b> example we use the MQTT library, PubSubClient, to demonstrate.

<h2>More information</h2>
For more information, including tutorials, getting started guides, and videos check out <a href="www.iotpipe.io/howitworks">www.iotpipe.io/howitworks</a>.
