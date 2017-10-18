#ifndef Status_h
#define Status_h

#include "Arduino.h"

class Status
{
	public:
		Status();
		Status execute();
	private:
		int _active;
};

#endif
