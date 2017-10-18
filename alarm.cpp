#include "Arduino.h"
#include "Alarm.h"

Alarm::Alarm(){
	_status=new Status();
}

void Alarm::check(int enable, int fail, int ack){
	if(_status){
		if(enable){
			Status statusAux = _status.execute(enable, fail, ack);
			delete _status;
			_status=statusAux;
		}else{
			disable();
		}
	}else if(!_status){
		enable();
	}
}

void enable(){
	if(!_status){
		_status=new Status();
	}
}

void disable(){
	if(_status){
		delete _status;
		_status=NULL;
	}
}
