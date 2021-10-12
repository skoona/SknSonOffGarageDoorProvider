// Observer.h

#ifndef _OBSERVER_h
#define _OBSERVER_h

#include <Arduino.h>

class Subject;

class Observer {
public:
	virtual void onReceivedDataFromSubject(const Subject*) = 0;
	void attachSubject(Subject *subject);
	virtual void onNotifyFromSubject(int value) = 0;
};

#endif

