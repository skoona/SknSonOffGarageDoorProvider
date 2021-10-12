/**
 * Homie Node for Controller.
 *
 */

#pragma once

#include <Homie.hpp>
#include "RelayNode.hpp"
#include "LoxRanger.hpp"

class ControllerNode : public HomieNode {

public:
  ControllerNode(const char *id, const char *name, const char *cType, RelayNode& relay, LoxRanger& ranger);

protected:
  virtual void setup() override;
  virtual void onReadyToOperate() override;
  virtual bool handleInput(const HomieRange& range, const String& property, const String& value);
  virtual void loop() override;

private:
  RelayNode& _relay;
  LoxRanger& _ranger;

  const char *cCaption = "• Controller Module:";
  const char* cIndent  = "  ◦ ";

  const char *cControllerID = "operator";

  volatile bool vbEnabled = false;                 // operating trigger

  void operate();
  void printCaption();
};