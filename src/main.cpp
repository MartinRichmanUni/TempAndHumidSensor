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
const int minTemp = 17;
const int maxTemp = 22;
const int minHumid = 30;
const int maxHumid = 70;
unsigned long lastChangeTime;

boolean timeDiff(unsigned long start, int specifiedDelay)
{
  return (millis() - start >= lightDelay);
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

void setup()
{
  Serial.begin(9600);
  greenLight();
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  dht.begin();
  lastChangeTime = 0;
}

void loop()
{
  int t = (int)dht.readTemperature();
  int h = (int)dht.readHumidity();

  switch (ledColour)
  {
  case GREEN:
    if (h > maxHumid || h < minHumid)
    {
      blueLight();
    }
    else if (t > maxTemp || t < minTemp)
    {
      redLight();
    }
    break;
  case BLUE:
    if (h < maxHumid && h > minHumid)
    {
      greenLight();
    }
    else if (t > maxTemp || t < minTemp)
    {
      redLight();
    }
    break;
  case RED:
    if (t < maxTemp && t > minTemp)
    {
      greenLight();
    }
    else if (h > maxHumid || h < minHumid)
    {
      blueLight();
    }
    break;
  }
}