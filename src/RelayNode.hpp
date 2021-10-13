/**
 * Homie Node for Relays.
 *
 */

#pragma once

#include <Homie.hpp>

#define VALIDATION_MAX_MS 1000
#define VALIDATION_MIN_MS 200
#define VALIDATION_DEFAULT_MS 400 

class RelayNode : public HomieNode {

public:
  RelayNode(const char* id, const char* name, const char* cType, const int relayPin, const int relayHold);

  bool isReady();
  void operate();
  void setHoldTimeInMilliseconds(const int ms);

protected:
  virtual void setup() override;
  virtual void onReadyToOperate() override;
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
