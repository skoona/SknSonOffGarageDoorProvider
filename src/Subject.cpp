#include "Subject.h"
#include "Observer.h"

Subject::Subject() {
}

void Subject::sknRegisterObserver(sknNotifyCallback callback)
{
	notifyCB = callback;
}
void Subject::sknUnregisterObserver()
{
	notifyCB = nullptr;
}

void Subject::registerObserver(Observer* obs) {
	mObserver = obs; //we will only allow one observer
}

void Subject::unregisterObserver() {
	mObserver = nullptr;
}

int Subject::getValue() const {
	return mValue;
}

void Subject::_notifyObserver() {
	if (mObserver != nullptr) {
		//we are passing the 'subject' instance so that the observer can get value
		mObserver->onReceivedDataFromSubject(this);
		mObserver->onNotifyFromSubject(getValue());
	}
	if (notifyCB != nullptr)
	{
		//we are calling a method directly with the changed value
		notifyCB(getValue());
	}
}

void Subject::setVal(const int val) {
	mValue = val;
	_notifyObserver(); //where there is a change, notify the observer
}

