/**
 * Homie Node for VL53L1x.
 *
 */
#include "LoxRanger.hpp"

LoxRanger::LoxRanger(const char *id, const char *name, const char *cType, const unsigned long durationInSeconds, const int gpioPin)
    : HomieNode(id, name, cType),
      _pinGPIO(gpioPin),
      ulCycleDuration((durationInSeconds * 1000))
{
  printCaption();
  setRunDuration(durationInSeconds);
}

/*
 * Utility to handle Duration Roll Overs
*/
unsigned long LoxRanger::setDuration(unsigned long duration)
{
  unsigned long value = millis() + duration;
  if (value < duration)
  { // rolled
    value = duration;
  }
  return value;
}

/**
 *
 */
void LoxRanger::setDirectionStatus(const int value)
{
  if (value == 0)
  {
    if (value > 2000)
    {
      strcpy(cDirection, "CLOSED");
    }
    else
    {
      strcpy(cDirection, "OPEN");
    }
  }
  else if (value >= 1)
  {
    strcpy(cDirection, "OPENING");
  }
  else
  {
    strcpy(cDirection, "CLOSING");
  }
}

/**
 *
 */
bool LoxRanger::isReady()
{
  return vbEnabled;
}

/**
 *
 */
bool LoxRanger::isOpen()
{
  String value = String(cDirection);

  if (value.equals("OPEN") && isReady()) {
    return true;
  }
  return false;
}

/**
 *
 */
bool LoxRanger::isClosed()
{
  String value = String(cDirection);

  if (value.equals("CLOSED") && isReady())
  {
    return true;
  }
  return false;
}

/**
 *
 */
void LoxRanger::setRunDuration(const int seconds) {
  if (seconds > VALIDATION_LOX_MIN_SECONDS && seconds < VALIDATION_LOX_MAX_SECONDS) {
    _ulCycleTime = (unsigned long)(seconds * 1000);
  } else {
    _ulCycleTime = VALIDATION_LOX_DEFAULT_SECONDS * 1000;
  } 
  ulCycleDuration = setDuration(_ulCycleTime);
  vbLastRangeCycle = true;
}

/**
 *
 */
void LoxRanger::operate()
{
  vbLastRangeCycle = true;
  ulCycleDuration = setDuration(_ulCycleTime);
}

/**
 *
 */
void LoxRanger::stopRanging() {
  lox.stopContinuous();  
  vTaskDelay(ulRangingDuration / portTICK_RATE_MS);
  ulCycleDuration = 0;
  vbLastRangeCycle = true;
}

/**
 *
 */
void LoxRanger::printCaption() {
  Homie.getLogger() << cCaption << "  " << getId() << endl;
}

/**
 * @brief Collect distance and determine direction of travel
 * 
 */
unsigned int LoxRanger::handleLoxRead() {
  const int capacity = (MAX_SAMPLES - 1);
  int idleUpDown = 0;

  unsigned int value = (unsigned int)lox.read(false);
  if (value == 0) {
    return uiDistanceValue;
  }

  for (int idx = 0; idx < capacity; idx++) {
    distances[idx] = distances[idx+1]; // move all down
  }

  distances[capacity] = lox.ranging_data.range_mm;

  if (distances[0] > 0) {
    idleUpDown = 0;

    for (int idx = 0; idx < capacity; idx++)
    {
      if (distances[idx] > (distances[idx+1] + 10)) {
        idleUpDown--; // Closing <-1
      }
      if ((distances[idx] + 10) < distances[idx+1])
      {
        idleUpDown++; // Opening > 1
      }
      // 0 = stable, open or closed
    }

    setDirectionStatus(idleUpDown);
  }

  return uiDistanceValue;
}

/**
 *
 */
void LoxRanger::loop()
{
  if (vbEnabled) 
  {
    ulTimebase = millis();
    ulElapsedTime = ulTimebase - ulLastTimebase;
    vbRangeDuration = (ulElapsedTime >= ulRangingDuration);
    vbRunCycle = (ulCycleDuration >= ulTimebase);

    if (vbLastRangeCycle && vbRunCycle) // ON
    {
      vbLastRangeCycle = false;
      lox.startContinuous((uint32_t)ulRangingDuration);
      setProperty(cOperateID).send("ON");
      Homie.getLogger() << cIndent << "Start continuous ranging @ " << ulRangingDuration << " ms accepted. ts: " << ulTimebase << endl;
    }
    
    if (vbRangeDuration && vbRunCycle) 
    {
      if (!digitalRead(_pinGPIO))
      {
        handleLoxRead();
        char buf[32];
        snprintf(buf, sizeof(buf), cRangeFormat, lox.ranging_data.range_mm);
        setProperty(cRangeID).send(buf);

        snprintf(buf, sizeof(buf), cStatusFormat, lox.rangeStatusToString(lox.ranging_data.range_status));
        setProperty(cStatusID).send(buf);

        snprintf(buf, sizeof(buf), cSignalFormat, lox.ranging_data.peak_signal_count_rate_MCPS);
        setProperty(cSignalID).send(buf);

        snprintf(buf, sizeof(buf), cAmbientFormat, lox.ranging_data.ambient_count_rate_MCPS);
        setProperty(cAmbientID).send(buf);

        setProperty(cDirectionID).send(cDirection);

        Homie.getLogger() << "〽 range: " << lox.ranging_data.range_mm
                          << " mm \tstatus: " << lox.rangeStatusToString(lox.ranging_data.range_status)
                          << "\tsignal: " << lox.ranging_data.peak_signal_count_rate_MCPS
                          << " MCPS\tambient: " << lox.ranging_data.ambient_count_rate_MCPS
                          << " MCPS" 
                          << " Direction: " << cDirection << endl;

        ulLastTimebase = ulTimebase;
      }
    }

    if (!vbLastRangeCycle && !vbRunCycle)
    {
      // OFF
      lox.stopContinuous();
      vbLastRangeCycle = true;
      setProperty(cOperateID).send("OFF");
      Homie.getLogger() << cIndent << "Stopping continuous ranging accepted. ts: " << ulTimebase << endl;
    }
  }
}

/**
 *
 */
void LoxRanger::onReadyToOperate() {
  Homie.getLogger() << "〽 "<< "Node: " << getName() << " Ready to operate." << endl;
  ulCycleDuration = setDuration(_ulCycleTime);
  vbEnabled = true;
}

/**
 *
 */
void LoxRanger::setup() {
  pinMode(_pinGPIO, INPUT_PULLUP);

  vbEnabled = false;

  lox.setTimeout(500);
  if (!lox.init())
  {
    vTaskDelay(1000 / portTICK_RATE_MS);
    while (!lox.init())
    {
      Homie.getLogger() << cIndent << "Failed to detect and initialize sensor!" << endl;
      vTaskDelay(1000 / portTICK_RATE_MS);
    }
  }

  if (lox.setDistanceMode(VL53L1X::Medium))
  {
    Homie.getLogger() << "〽 Medium distance mode accepted." << endl;
  }

  if (lox.setMeasurementTimingBudget(200000))
  {
    Homie.getLogger() << "〽 200us timing budget accepted." << endl;

    ulLastTimebase = millis();
  }

  advertise(cRangeID)
          .setName("distance in mm")
          .setDatatype("integer")
          .setFormat(cRangeFormat)
          .setRetained(false)
          .setUnit("mm");

  advertise(cStatusID)
      .setName("range operating status")
      .setDatatype("string")
      .setFormat(cStatusFormat)
      .setRetained(false)
      .setUnit("#");

  advertise(cSignalID)
      .setName("peak signal count rate")
      .setDatatype("float")
      .setFormat(cSignalFormat)
      .setRetained(false)
      .setUnit("mcps");

  advertise(cAmbientID)
      .setName("ambient light count rate")
      .setDatatype("float")
      .setFormat(cAmbientFormat)
      .setRetained(false)
      .setUnit("mcps");

  advertise(cDirectionID)
      .setName("Direction of movement")
      .setDatatype("enum")
      .setFormat(cDirectionFormat)
      .setRetained(false);

  advertise(cOperateID)
      .setName("Actively Ranging")
      .setDatatype("enum")
      .setFormat(cOperateFormat)
      .setRetained(false);
}
