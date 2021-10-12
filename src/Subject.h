// Subject.h


#ifndef _SUBJECT_h
#define _SUBJECT_h

#include <Arduino.h>
#include <functional>

typedef std::function<void(int value)> sknNotifyCallback;

class Observer;

class Subject {
public:
	Subject();
	void sknRegisterObserver(sknNotifyCallback callback);
	void sknUnregisterObserver();
	void registerObserver(Observer *);
	void unregisterObserver();
	int getValue() const;
	void setVal(const int val);

private:
	int mValue;
	void _notifyObserver();
	Observer* mObserver;
	sknNotifyCallback notifyCB;
};

#endif

