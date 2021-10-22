/**
 * Homie Node for Relays.
 *
 */
#include "RelayNode.hpp"

RelayNode::RelayNode(const char *id, const char *name, const char *cType, const int relayPin, const int relayHold)
    : HomieNode(id, name, cType), 
    _relayPin(relayPin),
    _relayHold(relayHold)
{
  printCaption();
  if ((_relayHold > VALIDATION_MAX_MS) && (_relayHold < VALIDATION_MIN_MS)) {
    _relayHold = VALIDATION_DEFAULT_MS;
  }
}

/**
 *
 */
bool RelayNode::isReady()
{
  return vbEnabled;
}

/**
 *
 */
void RelayNode::setHoldTimeInMilliseconds(const int ms)
{
  if (ms > VALIDATION_MIN_MS && ms < VALIDATION_MAX_MS)
  {
    _relayHold = ms;
    Homie.getLogger() << cIndent << "setHoldTimeInMilliseconds() set: " << ms << endl;
  } else {
    _relayHold = VALIDATION_DEFAULT_MS;
  }
}

/**
 *
 */
void RelayNode::operate(bool fastSlow)
{
  if (vbEnabled) {
    int delayTime = _relayHold;
    unsigned long time_now = millis();

    if (fastSlow) {
      delayTime = _relayHold / 2; // half time
    }

    Homie.getLogger() << cIndent << "[Start] Operating Relay pin: " << _relayPin << " level: " << _relayOnLevel << " Hold: " << delayTime << " Fast: " << fastSlow << endl;
    digitalWrite(_relayPin, _relayOnLevel); // activate door relay

    // delay(delayTime); // Delay is not working in this class ???
    while (millis() < time_now + delayTime) {}

    digitalWrite(_relayPin, _relayOffLevel); // de-activate door relay
    Homie.getLogger() << cIndent << "[Stop ] Operating Relay" << endl;
  }
}

/**
 *
 */
void RelayNode::printCaption() {
  Homie.getLogger() << cCaption << "  " << getId() << endl;
}

/**
 *
 */
void RelayNode::loop() {  
  yield();
}

/**
 *
 */
void RelayNode::onReadyToOperate() {
  vbEnabled = true;
  Homie.getLogger()
      << "〽 "
      << "Node: " << getName()
      << " Ready to operate."
      << endl;
}

/**
 *
 */
void RelayNode::setup() {
  pinMode(_relayPin, OUTPUT); // Door operator
  digitalWrite(_relayPin, _relayOffLevel); // Init door to off

  vbEnabled = false;

  advertise(cRelayID)
      .setName("Operate Door Switch")
      .setDatatype("enum")
      .setFormat("ON,OFF")
      .setRetained(false);
}
