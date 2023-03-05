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
const int specifiedDelay = 500;
unsigned long lastChangeTime;

boolean timeDiff(unsigned long start, int specifiedDelay)
{
  return (millis() - start >= specifiedDelay);
}

void greenLight()
{

  if (timeDiff(lastChangeTime, specifiedDelay))
  {
    analogWrite(PIN_RED, 0);
    analogWrite(PIN_GREEN, 200);
    analogWrite(PIN_BLUE, 0);
    ledColour = GREEN;
  }
}

void redLight()
{
  if (timeDiff(lastChangeTime, specifiedDelay))
  {
    analogWrite(PIN_RED, 200);
    analogWrite(PIN_GREEN, 0);
    analogWrite(PIN_BLUE, 0);
    ledColour = RED;
  }
}

void blueLight()
{
  if (timeDiff(lastChangeTime, specifiedDelay))
  {
    analogWrite(PIN_RED, 0);
    analogWrite(PIN_GREEN, 0);
    analogWrite(PIN_BLUE, 200);
    ledColour = BLUE;
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  ledColour = GREEN;
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

  Serial.println(t);

  switch (ledColour)
  {
  case GREEN:
    if (h > 70 || h < 30)
    {
      blueLight();
    }
    else if (t > 22 || t < 20)
    {
      redLight();
    }
    break;
  case BLUE:
    if (h < 70 && h > 30)
    {
      greenLight();
    }
    else if (t > 22 || t < 20)
    {
      redLight();
    }
    break;
  case RED:
    if (t < 22 && t > 20)
    {
      greenLight();
    }
    else if (h > 70 || h < 30)
    {
      blueLight();
    }
    break;
  }
}