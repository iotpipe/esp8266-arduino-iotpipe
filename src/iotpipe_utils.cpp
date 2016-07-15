#include "iotpipe_utils.h"

const int max_msg_length = 256;
char buf[max_msg_length+1];

namespace IotPipe_Utils
{
	void LOG_DEBUG(const char *message)
	{
		Serial.println(message);
	}

	void LOG_DEBUG_ARGS(const char *message,...)
	{

		va_list arguments;
		va_start(arguments,message);

		flatten_string(buf, max_msg_length);

		vsnprintf(buf, max_msg_length, message, arguments);
		
		if(buf[max_msg_length]!='\0')
		{
			Serial.println("Error.  Cannot print message longer than 255 characters");
		}
		else
		{
			Serial.println(buf);
		}
	}

	void flatten_string(char *str,int len)
	{
		int i = 0;
		while(i<len)
		{
			str[i]='\0';
			i++;
		}
	}
}
