#include "ObserverTester.h"
#include "Subject.h"

void ObserverTester::onReceivedDataFromSubject(const Subject *sub) {
	Serial.printf("onReceivedDataFromSubject() VALUE is %02d\n", sub->getValue());
}
void ObserverTester::onNotifyFromSubject(int value)
{
	Serial.printf("onNotifyFromSubject() VALUE is %d\n", value);
}

// Interesting Approach
// ref: https://stackoverflow.com/questions/400257/how-can-i-pass-a-class-member-function-as-a-callback/45525074#45525074
// ref: https://blog.mbedded.ninja/programming/languages/c-plus-plus/callbacks/
// static void Callback(int other_arg, void *this_pointer)
// {
// 	ObserverTester *self = static_cast<ObserverTester *>(this_pointer);
// 	self->onNotifyFromSubject(other_arg);
// }
