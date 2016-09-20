// ----------------------------------------------------------------------------
// File: iotpipe_utils.h
// Copyright (c) 2015 IoT Pipe. All Right Reserved
// ----------------------------------------------------------------------------

#ifndef IOTPIPE_UTILS_H
#define IOTPIPE_UTILS_H

#include <stdarg.h>
#include <Arduino.h> //needed for Serial.println

namespace IotPipe_Utils
{
	
	void LOG_DEBUG_ARGS(const char *message,...);
	void LOG_DEBUG(const char *message);

	//flatten strings with null characters
	void flatten_string(char *str,int len);
	void padStringWithZeros(String &str, int desiredLength);
}


#endif
