/**
 * Homie Node for Controller.
 *
 */

#pragma once

#include <Homie.hpp>
#include "RelayNode.hpp"
#include "LoxRanger.hpp"

#define VALIDATION_MAX_SECONDS 3601
#define VALIDATION_MIN_SECONDS 59
#define VALIDATION_DEFAULT_SECONDS 1200  // 20 minutes

class ControllerNode : public HomieNode {

public:
  ControllerNode(const char *id, const char *name, const char *cType, RelayNode& relay, LoxRanger& ranger);

  void setIntervalInSeconds(const long seconds);

protected:
  virtual void setup() override;
  virtual void onReadyToOperate() override;
  virtual bool handleInput(const HomieRange& range, const String& property, const String& value);
  virtual void loop() override;

private:
  RelayNode& _relay;
  LoxRanger& _ranger;

  long _interval = 300000;  // 300 seconds
  unsigned long ulLastTimebase;

  const char *cCaption = "• Controller Module:";
  const char* cIndent  = "  ◦ ";

  const char *cControllerID = "operator";

  volatile bool vbEnabled = false;                 // operating trigger

  void operate();
  void printCaption();
};
