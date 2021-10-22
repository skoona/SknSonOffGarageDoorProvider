// ESP8266 SonOff 5Vdc Relay

#include <Arduino.h>
#include <Homie.h>
#define SDA 14
#define SCL 12
#define PIN_WIRE_SDA 14
#define PIN_WIRE_SCL 12

#include <Wire.h>
extern "C"
{
#include <user_interface.h>
}

#include "RelayNode.hpp"
#include "LoxRanger.hpp"
#include "ControllerNode.hpp"

#define SKN_MOD_NAME "SonOff Door Operator"
#define SKN_MOD_VERSION "3.0.0"
#define SKN_MOD_BRAND "SknSensors"

#define SKN_RELAY_TITLE "Relay Service"
#define SKN_RELAY_TYPE "switch"
#define SKN_RELAY_ID "relayService"
#define DEFAULT_HOLD_MS 400

#define SKN_RANGER_TITLE "Ranging Service"
#define SKN_RANGER_TYPE "measurement"
#define SKN_RANGER_ID "positionService"
#define LOX_RUNTIME_SECONDS 30

#define SKN_CTRL_TITLE "Controller"
#define SKN_CTRL_TYPE "switch"
#define SKN_CTRL_ID "provider"

// Pins
#define LOX_PIN_SDA   14
#define LOX_PIN_SCL   12
#define LOX_PIN_GPIO  13
#define RELAY_PIN    5

// #ifndef LED_BUILTIN
#define LED_BUILTIN 16
// #endif

HomieSetting<long> cfgRelayHoldMS("relayHoldTimeMS", "Relay hold time in milliseconds.");
HomieSetting<long> cfgIntervalSec("positionIntervalSec", "Seconds between ranging to verify door position.");
HomieSetting<long> cfgDuration("duration", "Seconds to measure distance after triggered.");
HomieSetting<long> cfgOpenMM("rangerOpenMM", "fully open threshold in millimeters.");
HomieSetting<long> cfgClosedMM("rangerClosedMM", "fully closed threshold in millimeters.");

RelayNode relay(SKN_RELAY_ID, SKN_RELAY_TITLE, SKN_RELAY_TYPE, RELAY_PIN, DEFAULT_HOLD_MS);
LoxRanger ranger(SKN_RANGER_ID, SKN_RANGER_TITLE, SKN_RANGER_TYPE, LOX_RUNTIME_SECONDS, LOX_PIN_GPIO);
ControllerNode ctrl(SKN_CTRL_ID, SKN_CTRL_TITLE, SKN_CTRL_TYPE, relay, ranger);

bool broadcastHandler(const String &level, const String &value)
{
  Homie.getLogger() << "Received broadcast level " << level << ": " << value << endl;
  return true;
}

void setup()
{
  Serial.begin(115200);
  delay(200);
  if (!Serial)
  {
    Homie.disableLogging();
  }

  Wire.begin(LOX_PIN_SDA, LOX_PIN_SCL);

  Homie_setFirmware(SKN_MOD_NAME, SKN_MOD_VERSION);
  Homie_setBrand(SKN_MOD_BRAND);

  cfgRelayHoldMS
      .setDefaultValue(DEFAULT_HOLD_MS)
      .setValidator([](long candidate)
                    { return candidate > 200 && candidate < 1000; });
  cfgIntervalSec
      .setDefaultValue(300)
      .setValidator([](long candidate)
                    { return candidate > 59 && candidate < 3601; });
  cfgDuration
      .setDefaultValue(20)
      .setValidator([](long candidate)
                    { return candidate > 0 && candidate < 181; });

  cfgOpenMM
      .setDefaultValue(200)
      .setValidator([](long candidate)
                    { return candidate > 10 && candidate < 400; });
  cfgClosedMM
      .setDefaultValue(2000)
      .setValidator([](long candidate)
                    { return candidate > 1000 && candidate < 3200; });

  relay.setHoldTimeInMilliseconds(cfgRelayHoldMS.get());
  ctrl.setIntervalInSeconds(cfgIntervalSec.get());
  ranger.setRunDuration(cfgDuration.get());
  ranger.setOpenThresholdMM(cfgOpenMM.get());
  ranger.setClosedThresholdMM(cfgClosedMM.get());

  Homie.setBroadcastHandler(broadcastHandler)
      .setLedPin(LED_BUILTIN, LOW)
      .disableResetTrigger();

  Homie.setup();
}

void loop()
{
  Homie.loop();
}
