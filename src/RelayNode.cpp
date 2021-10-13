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
void RelayNode::operate()
{
  if (vbEnabled) {
    Homie.getLogger() << cIndent << "[Start] Operating Door" << endl;
    digitalWrite(_relayPin, HIGH); // activate door relay
    vTaskDelay(_relayHold / portTICK_RATE_MS);
    digitalWrite(_relayPin, LOW); // de-activate door relay
    Homie.getLogger() << cIndent << "[Stop ] Operating Door" << endl;
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
  taskYIELD();
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

  digitalWrite(_relayPin, LOW); // Init door to off

  vbEnabled = false;

  advertise(cRelayID)
      .setName("Operate Door Switch")
      .setDatatype("enum")
      .setFormat("ON,OFF")
      .setRetained(false);
}
