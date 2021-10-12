/**
 * Homie Node for Relays.
 *
 */

#pragma once

#include <Homie.hpp>

class RelayNode : public HomieNode {

public:
  RelayNode(const char* id, const char* name, const char* cType, const int relayPin, const int relayHold);

  bool isReady();
  void operate();
  void setHoldTimeInMilliseconds(const int ms);

protected:
  virtual void setup() override;
  virtual void onReadyToOperate() override;
  virtual bool handleInput(const HomieRange& range, const String& property, const String& value);
  virtual void loop() override;

private:
  int _relayPin;
  int _relayHold;
  
  const char *cCaption = "• Relay Module:";
  const char* cIndent  = "  ◦ ";

  const char *cRelayID = "operate";

  volatile bool vbEnabled = false;                 // operating trigger


  void printCaption();
};
