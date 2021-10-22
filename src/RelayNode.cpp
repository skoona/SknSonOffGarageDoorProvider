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
    Homie.getLogger() << cIndent << "[Start] Operating Relay" << endl;
    digitalWrite(_relayPin, _relayOnLevel); // activate door relay
    if (fastSlow) {
      delay((_relayHold / 2));  // half time
    } else {
      delay(_relayHold);
    }
    digitalWrite(_relayPin, !_relayOnLevel); // de-activate door relay
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

  digitalWrite(_relayPin, !_relayOnLevel); // Init door to off

  vbEnabled = false;

  advertise(cRelayID)
      .setName("Operate Door Switch")
      .setDatatype("enum")
      .setFormat("ON,OFF")
      .setRetained(false);
}
