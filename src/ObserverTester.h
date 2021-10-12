// ObserverTester.h

#ifndef _TESTOBSERVER_h
#define _TESTOBSERVER_h

#include <Arduino.h>
#include "Observer.h"

class ObserverTester : public Observer
{
	public:
	void onReceivedDataFromSubject(const Subject*) override;
	void onNotifyFromSubject(int value) override;
	
	// static void Callback(int other_arg, void *this_pointer);
};

#endif

