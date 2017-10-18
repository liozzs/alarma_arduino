#ifndef Alarm_h
#define Alarm_h

#include "Arduino.h"
#include "Status.h"

class Alarm
{
	public:
		Alarm();
		void check(int enable, int fail, int ack);
	private:
		Status *_status;
};

#endif
