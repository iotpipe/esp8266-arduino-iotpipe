#include "Arduino.h"
#include "iotpipe_utils.h"
#include "iotpipe_gpio.h"
  
using namespace IotPipe_Utils;




IotPipe_GPIO gpio;

void setup()
{

  Serial.begin(115200);


  gpio.setPortAsDigitalInput(2, "sliding_door");
  gpio.setPortAsDigitalInput(0, "garage_door");  
  
  gpio.setPortAsDigitalInput(4, "");
  gpio.setPortAsDigitalOutput(5, "window");
  gpio.setPortAsAnalogInput("Humidity");

  gpio.print();
  delay(2000);
}

void loop()
{
  // put your main code here, to run repeatedly:
  char buf[256];
  flatten_string(buf,256);
  gpio.jsonifyInputScan(buf,256);
  LOG_DEBUG_ARGS("%s",buf);
  delay(3000);
}
