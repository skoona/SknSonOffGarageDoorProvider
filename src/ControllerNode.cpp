/**
 * Homie Node for Controller.
 *
 */
#include "ControllerNode.hpp"

ControllerNode::ControllerNode(const char *id, const char *name, const char *cType, RelayNode& relay, LoxRanger& ranger)
    : HomieNode(id, name, cType),
      _relay(relay),
      _ranger(ranger)
{
  setIntervalInSeconds(VALIDATION_DEFAULT_SECONDS);
  printCaption();
}

/**
 *
 */
void ControllerNode::setIntervalInSeconds(const long seconds) {
  if (seconds > VALIDATION_MIN_SECONDS && seconds < VALIDATION_MAX_SECONDS)
  {
    _interval = seconds * 1000; // ms
  }
  else
  {
    _interval = VALIDATION_DEFAULT_SECONDS * 1000;
  }
}

/**
 *
 */
void ControllerNode::operate()
{
  if (vbEnabled) {
    Homie.getLogger() << cIndent << "[Start] Operating Door" << endl;
    
    if (_ranger.isReady()) {
      _ranger.operate();
    }

    if (_relay.isReady())
    {
      _relay.operate(false); // normal or slow
    }

    Homie.getLogger() << cIndent << "[Stop ] Operating Door" << endl;
  }
}

/**
 *
 */
void ControllerNode::printCaption() {
  Homie.getLogger() << cCaption << " " << getId() << endl;
}

/**
 * Handles the received MQTT messages from Homie.
 *
 */
bool ControllerNode::handleInput(const HomieRange& range, const String& property, const String& value) {

  Homie.getLogger() << cIndent << "〽 handleInput -> property '" << property << "' value=" << value << endl;

  if (property.equalsIgnoreCase(cControllerID))
  {
    if (value.equalsIgnoreCase("open"))
    {
      if (_ranger.isClosed()) {
        operate();
      } else {
        Homie.getLogger() << cIndent << "Door is already OPEN" << endl;
      }
      setProperty(cControllerID).send("OPEN");

    } else if (value.equalsIgnoreCase("close")) {
      if (_ranger.isOpen())
      {
        operate();
      } else {
        Homie.getLogger() << cIndent << "Door is already CLOSED" << endl;
      }
      setProperty(cControllerID).send("CLOSE");

    } else {

      setProperty(cControllerID).send("IDLE");
    }

    return true;
  }

  return false;
}

/**
 *
 */
void ControllerNode::loop() {
  if (vbEnabled)
  {
    // nothing to do here
    if ((millis() - ulLastTimebase) >= _interval) {
      ulLastTimebase = millis();
      _ranger.operate();
    }
  }
}

/**
 *
 */
void ControllerNode::onReadyToOperate() {
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
void ControllerNode::setup() {
  vbEnabled = false;

  advertise(cControllerID)
      .setName("Door Operator")
      .setDatatype("enum")
      .setFormat("OPEN,CLOSE,IDLE")
      .setRetained(true)
      .settable();
}
