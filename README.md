# 6131COMP - Distributed and Embedded Systems

The developed software is a temperature/humidity alert system which runs on the ESP32 microcontroller. 




## Features

- Reading Temperature and Humidity Sensor readings
- lighting up an RGB LED red, green, or blue dependent on whether values are within range or not
    - Constant Red = Only temperature is too high or too low
    - Constant Blue = Only humidity is too high or too low
    - Constant Green = Both temperature and humidity are within range
    - Flashing Red & Blue at a rate of 2Hz = Both temperature and humidity are out of range
- Debug logging sensor values to a serial monitor every 5 seconds
- User set minimum and maximum values for both temperature and humidity
