#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"

#define PIN_RED 12
#define PIN_GREEN 14
#define PIN_BLUE 13

#define DHTPIN 27
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

enum LEDState
{
  RED,
  GREEN,
  BLUE
};

LEDState ledColour;
const int lightDelay = 500;
const int debugDelay = 5000;
const int minTemp = 15;
const int maxTemp = 22;
const int minHumid = 30;
const int maxHumid = 80;
unsigned long lastChangeTime;
unsigned long lastDebug;
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

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  dht.begin();
  greenLight();
  lastChangeTime = 0;
  lastDebug = 0;
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
    if (humid < maxHumid && humid > minHumid)
    {
      greenLight();
    }
    else if (temp > maxTemp || temp < minTemp)
    {
      redLight();
    }
    break;
  case RED:
    if (temp < maxTemp && temp > minTemp)
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
}