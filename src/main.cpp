#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <ESP32Encoder.h>
#include "DHT.h"

#define PIN_RED 12
#define PIN_GREEN 14
#define PIN_BLUE 13

#define ROTARY_A 19
#define ROTARY_B 18

#define DHTPIN 27
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define BUTTON_PIN 15

enum LEDState
{
  RED,
  GREEN,
  BLUE
};

enum ChangeState
{
  DEF,
  MINTEMP,
  MAXTEMP,
  MINHUMID,
  MAXHUMID
};

LEDState ledColour;
ChangeState changeState;
ESP32Encoder encoder;

const int bounce_delay = 500;
const int lightDelay = 500;
const int debugDelay = 5000;
const int templimitLow = 5;
const int templimitHigh = 30;
const int humidlimitLow = 0;
const int humidlimitHigh = 100;
int minTemp = 15;
int maxTemp = 22;
int minHumid = 30;
int maxHumid = 80;
unsigned long lastChangeTime;
unsigned long lastDebug;
unsigned long lastbtnPress;
String tempStatus;
String humidStatus;
int temp;
int humid;

boolean timeDiff(unsigned long start, int specifiedDelay)
{
  return (millis() - start >= specifiedDelay);
}

void greenLight()
{
  if (timeDiff(lastChangeTime, lightDelay))
  {
    analogWrite(PIN_RED, 0);
    analogWrite(PIN_GREEN, 200);
    analogWrite(PIN_BLUE, 0);
    ledColour = GREEN;
  }
}

void redLight()
{
  if (timeDiff(lastChangeTime, lightDelay))
  {
    analogWrite(PIN_RED, 200);
    analogWrite(PIN_GREEN, 0);
    analogWrite(PIN_BLUE, 0);
    ledColour = RED;
  }
}

void blueLight()
{
  if (timeDiff(lastChangeTime, lightDelay))
  {
    analogWrite(PIN_RED, 0);
    analogWrite(PIN_GREEN, 0);
    analogWrite(PIN_BLUE, 200);
    ledColour = BLUE;
  }
}

void debugHumid()
{
  if (humid > maxHumid)
  {
    humidStatus = "Too High";
  }
  else if (humid < minHumid)
  {
    humidStatus = "Too Low";
  }
  else
  {
    humidStatus = "Within Range";
  }
}

void debugTemp()
{
  if (temp > maxTemp)
  {
    tempStatus = "Too High";
  }
  else if (temp < minTemp)
  {
    tempStatus = "Too Low";
  }
  else
  {
    tempStatus = "Within Range";
  }
}

void displayValues()
{
  if (timeDiff(lastDebug, debugDelay))
  {
    Serial.print("Humidity: ");
    Serial.print(humid);
    Serial.print("%\t");
    Serial.print("\t");
    Serial.print("Humidity Status: ");
    Serial.println(humidStatus);
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print("C\t");
    Serial.print("Temperature Status: ");
    Serial.println(tempStatus);
    lastDebug = millis();
  }
}

// Incomplete for changing states with button and rotary encoder
void changeValues()
{
  switch (changeState)
  {
  case MINTEMP:
    // if button pressed, change state and print
    if (digitalRead(BUTTON_PIN) == HIGH)
    {
      if (timeDiff(lastbtnPress, bounce_delay))
      {
        lastbtnPress = millis();
        changeState = MAXTEMP;
        encoder.setCount(maxTemp);
        Serial.println("Set the maximum temperature");
        Serial.println(maxTemp);
      }
    }
    else if (minTemp != encoder.getCount())
    {
      minTemp = encoder.getCount();

      if (minTemp >= templimitHigh)
      {
        minTemp = templimitHigh;
        encoder.setCount(minTemp);
      }
      else if (minTemp <= templimitLow)
      {
        minTemp = templimitLow;
        encoder.setCount(minTemp);
      }

      Serial.println(minTemp);
    }
    break;
  case MAXTEMP:
    if (digitalRead(BUTTON_PIN) == HIGH)
    {
      if (timeDiff(lastbtnPress, bounce_delay))
      {
        lastbtnPress = millis();
        changeState = MINHUMID;
        encoder.setCount(minHumid);
        Serial.println("Set the minimum humidity");
        Serial.println(minHumid);
      }
    }
    else if (maxTemp != encoder.getCount())
    {
      maxTemp = encoder.getCount();

      if (maxTemp >= templimitHigh)
      {
        maxTemp = templimitHigh;
        encoder.setCount(maxTemp);
      }
      else if (maxTemp <= minTemp)
      {
        maxTemp = minTemp;
        encoder.setCount(maxTemp);
      }

      Serial.println(maxTemp);
    }
    break;
  case MINHUMID:
    if (digitalRead(BUTTON_PIN) == HIGH)
    {
      if (timeDiff(lastbtnPress, bounce_delay))
      {
        lastbtnPress = millis();
        changeState = MAXHUMID;
        encoder.setCount(maxHumid);
        Serial.println("Set the maximum humidity");
        Serial.println(maxHumid);
      }
    }
    else if (minHumid != encoder.getCount())
    {
      minHumid = encoder.getCount();

      if (minHumid >= humidlimitHigh)
      {
        minHumid = humidlimitHigh;
        encoder.setCount(minHumid);
      }
      else if (minHumid <= humidlimitLow)
      {
        minHumid = humidlimitLow;
        encoder.setCount(minHumid);
      }

      Serial.println(minHumid);
    }
    break;
  case MAXHUMID:
    if (digitalRead(BUTTON_PIN) == HIGH)
    {
      if (timeDiff(lastbtnPress, bounce_delay))
      {
        lastbtnPress = millis();
        changeState = MINTEMP;
        encoder.setCount(minTemp);
        Serial.println("Set the minimum temperature");
        Serial.println(minTemp);
      }
    }
    else if (maxHumid != encoder.getCount())
    {
      maxHumid = encoder.getCount();

      if (maxHumid >= humidlimitHigh)
      {
        maxHumid = humidlimitHigh;
        encoder.setCount(maxHumid);
      }
      else if (maxHumid <= minHumid)
      {
        maxHumid = minHumid;
        encoder.setCount(maxHumid);
      }

      Serial.println(maxHumid);
    }
    break;
  case DEF:
    if (digitalRead(BUTTON_PIN) == HIGH)
    {
      if (timeDiff(lastbtnPress, bounce_delay))
      {
        lastbtnPress = millis();
        changeState = MINTEMP;
        encoder.setCount(minTemp);
        Serial.println("Set the minimum temperature");
        Serial.println(minTemp);
      }
    }
    break;
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  dht.begin();
  greenLight();
  lastChangeTime = 0;
  lastDebug = 0;
  ESP32Encoder::useInternalWeakPullResistors = UP;
  encoder.attachHalfQuad(ROTARY_A, ROTARY_B);
  encoder.setCount(20);
  changeState = DEF;
}

void loop()
{
  temp = (int)dht.readTemperature();
  humid = (int)dht.readHumidity();

  LEDState previousColour = ledColour;

  switch (ledColour)
  {
  case GREEN:
    if (humid > maxHumid || humid < minHumid)
    {
      blueLight();
    }
    else if (temp > maxTemp || temp < minTemp)
    {
      redLight();
    }
    break;
  case BLUE:
    if (humid <= maxHumid && humid >= minHumid)
    {
      greenLight();
    }
    else if (temp > maxTemp || temp < minTemp)
    {
      redLight();
    }
    break;
  case RED:
    if (temp <= maxTemp && temp >= minTemp)
    {
      greenLight();
    }
    else if (humid > maxHumid || humid < minHumid)
    {
      blueLight();
    }
    break;
  }

  if (previousColour != ledColour)
  {
    lastChangeTime = millis();
  }

  debugHumid();
  debugTemp();
  displayValues();
  changeValues();
}