#include <Arduino.h>

#include <Homie.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "ObserverTester.h"
#include "Observer.h"
#include "Subject.h"

#include "RelayNode.hpp"

#define SKN_MOD_NAME "Door Relay"
#define SKN_MOD_VERSION "1.0.0"
#define SKN_MOD_BRAND "SknSensors"

#define SKN_NODE_TITLE "Door Relay"
#define SKN_NODE_TYPE "switch"
#define SKN_NODE_ID   "DoorRelay"
#define DEFAULT_HOLD_MS 500

// Pins
#define PIN_RELAY 19 
#define PIN_SDA 22
#define PIN_SCL 21

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

HomieSetting<long> cfgRelayHoldMS("relayHoldTimeMS", "Relay hold time in milliseconds.");
HomieSetting<long> cfgIntervalSec("positionIntervalSec", "Seconds between ranging to determine door position.");
RelayNode door(SKN_NODE_ID, SKN_NODE_TITLE, SKN_NODE_TYPE, PIN_RELAY, DEFAULT_HOLD_MS);

Subject subject;
ObserverTester observerTester;

void onNotifyFromSubjectCB(int value)
{
  display.printf("%02d ", value);
  display.display();
}

bool broadcastHandler(const String &level, const String &value)
{
  Homie.getLogger() << "Received broadcast level " << level << ": " << value << endl;

  display.printf("%s: %s", level.c_str(), value.c_str());
  display.display();

  return true;
}

void setup()
{
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();

  observerTester.attachSubject(&subject);
  subject.sknRegisterObserver(onNotifyFromSubjectCB);

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

  door.setHoldTimeInMilliseconds(cfgRelayHoldMS.get());

  Homie.setBroadcastHandler(broadcastHandler)
      .setLedPin(LED_BUILTIN, LOW)
      .disableResetTrigger();

  Homie.setup();
}

uint64_t ulTimeBase;

void loop()
{
  Homie.loop();

  if (Homie.isConfigured())
  {
    // The device is configured, in normal mode
    if (Homie.isConnected())
    {
      display.setTextColor(WHITE);
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Registered Observer.");
      display.setCursor(0, 16);
      display.display();
      for (int i = 0; i < 10; i++)
      {
        subject.setVal(i); //this will print data on Serial Monitor
        delay(500);
      }

      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Unregister Observer");
      display.display();
      subject.unregisterObserver();
      // subject.sknUnregisterObserver();

      display.setCursor(0, 16);
      for (int i = 0; i < 10; i++)
      {
        subject.setVal(i); // we will not be seeing any value on Serial Monitor because we unregister Observer
        delay(500);
      }

      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Register again");
      display.display();
      subject.registerObserver(&observerTester);
      // subject.sknRegisterObserver(onNotifyFromSubjectCB);
    }
  }
}
