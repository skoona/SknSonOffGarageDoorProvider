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
  if (ms > 200 && ms < 1000) {
    _relayHold = ms;
    Homie.getLogger() << cIndent << "[setHoldTimeInMilliseconds] set: " << ms << endl;
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
    vTaskDelay(_relayHold);
    digitalWrite(_relayPin, LOW); // de-activate door relay
    Homie.getLogger() << cIndent << "[Stop ] Operating Door" << endl;
  }
}

/**
 *
 */
void RelayNode::printCaption() {
  Homie.getLogger() << cCaption << endl;
}

/**
 * Handles the received MQTT messages from Homie.
 *
 */
bool RelayNode::handleInput(const HomieRange& range, const String& property, const String& value) {

  printCaption();
  Homie.getLogger() << cIndent << "〽 handleInput -> property '" << property << "' value=" << value << endl;

  if (property.equalsIgnoreCase(cRelayID))
  {
    if (value.equalsIgnoreCase("on"))
    {
      operate();
    }
    else if (value.equalsIgnoreCase("off"))
    {
      operate();
    }

    setProperty(cRelayID).send("OFF");

    return true;
  }

  return false;
}

/**
 *
 */
void RelayNode::loop() {
  if (vbEnabled)
  {
    // nothing to do here
  }
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
  printCaption();

  pinMode(_relayPin, OUTPUT); // Door operator

  digitalWrite(_relayPin, LOW); // Init door to off

  vbEnabled = false;

  advertise(cRelayID)
      .setName("Operate Door Switch")
      .setDatatype("enum")
      .setFormat("ON,OFF")
      .setRetained(false)
      .settable();
}
