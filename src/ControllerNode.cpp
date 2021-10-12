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
  printCaption();
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
      _relay.operate();
    }

    Homie.getLogger() << cIndent << "[Stop ] Operating Door" << endl;
  }
}

/**
 *
 */
void ControllerNode::printCaption() {
  Homie.getLogger() << cCaption << endl;
}

/**
 * Handles the received MQTT messages from Homie.
 *
 */
bool ControllerNode::handleInput(const HomieRange& range, const String& property, const String& value) {

  printCaption();
  Homie.getLogger() << cIndent << "〽 handleInput -> property '" << property << "' value=" << value << endl;

  if (property.equalsIgnoreCase(cControllerID))
  {
    if (value.equalsIgnoreCase("open"))
    {
      if (_ranger.isClosed()) {
        operate();
      } else {
        Homie.getLogger() << cIndent << "Door is already OPEN" << endl;
        setProperty(cControllerID).send("OPEN");
      }
    } else if (value.equalsIgnoreCase("close")) {
      if (_ranger.isOpen())
      {
        operate();
      } else {
        Homie.getLogger() << cIndent << "Door is already CLOSED" << endl;
        setProperty(cControllerID).send("CLOSE");
      }
    }

    setProperty(cControllerID).send("OFF");

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
  printCaption();

  vbEnabled = false;

  advertise(cControllerID)
      .setName("Door Operator")
      .setDatatype("enum")
      .setFormat("OPEN,CLOSE,IDLE")
      .setRetained(false)
      .settable();
}
