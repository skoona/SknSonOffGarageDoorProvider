#include <Arduino.h>

#include <Homie.h>

#include <Wire.h>

#include "RelayNode.hpp"
#include "LoxRanger.hpp"
#include "ControllerNode.hpp"

#define SKN_MOD_NAME "Door Operator"
#define SKN_MOD_VERSION "1.0.0"
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
#define LOX_PIN_SDA 21
#define LOX_PIN_SCL 22
#define LOX_PIN_GPIO 19
#define RELAY_PIN 18 

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

HomieSetting<long> cfgRelayHoldMS("relayHoldTimeMS", "Relay hold time in milliseconds.");
HomieSetting<long> cfgIntervalSec("positionIntervalSec", "Seconds between ranging to verify door position.");
HomieSetting<long> cfgDuration("duration", "Seconds to measure distance after triggered.");

RelayNode door(SKN_RELAY_ID, SKN_RELAY_TITLE, SKN_RELAY_TYPE, RELAY_PIN, DEFAULT_HOLD_MS);
LoxRanger ranger(SKN_RANGER_ID, SKN_RANGER_TITLE, SKN_RANGER_TYPE, LOX_RUNTIME_SECONDS, LOX_PIN_GPIO);
ControllerNode ctrl(SKN_CTRL_ID, SKN_CTRL_TITLE, SKN_CTRL_TYPE, door, ranger);

bool broadcastHandler(const String &level, const String &value)
{
  Homie.getLogger() << "Received broadcast level " << level << ": " << value << endl;
  return true;
}

void setup()
{
  Serial.begin(115200);
  vTaskDelay(200);
  if (!Serial)
  {
    Homie.disableLogging();
  }

  Wire.begin(LOX_PIN_SDA, LOX_PIN_SCL, 400000U);

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

  door.setHoldTimeInMilliseconds(cfgRelayHoldMS.get());
  ranger.setRunDuration(cfgDuration.get());
  ctrl.setIntervalInSeconds(cfgIntervalSec.get());

  Homie.setBroadcastHandler(broadcastHandler)
      .setLedPin(LED_BUILTIN, LOW)
      .disableResetTrigger();

  Homie.setup();
}

void loop()
{
  Homie.loop();
}
