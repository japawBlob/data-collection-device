# Home hub - data collection device

Semestral project for CTU FEL subject B4M38AVS

Author: Jakub Jíra

## Motivation

This project aims to create basic smart home hub for monitoring and storing usefull data. Data, that is beeing monitored is temperature, humidity and vibration. These data are then stored on SD card. Communication via USART is also implemented for transmitting data, and requesting other services from other devices.

## Used Hardware

Microcontroller used in this project is STM32F207VC embedded in Sleepy Cat developement board. This microcontroller handles all sensor data processing and communication with other devices. 

### Sensors

Measured data are: Air humidity, Temperature, Vibrations.

Humidity and temperature is measured by external sensor DHT11. It is able to measure temperatures in range from 0°C to 50°C and air humidity from 20% to 90%. Sensor uses serial interface that sends data with 16-bit resolution. Since temperature range 0-50°C is quite small, one NTC termistor is connected to microcontrollers ADC to allow for large range of resolution.

Vibrations are measured by internal accelrometer.

### HCI

The device implements several peripherals for interaction with user. 

Included LCD display For is used logged data presentation. Interaction with the system is implemented using rotary encoder and two user buttons.

### Other peripherals

For data storage even during loss of power is used interface with micro SD card. If card with supported file system is inserted, it will be used for logging data storrage. 

If measuring exact time is required it is possible to connect DS3231 module to the controllers I2C interface. This module is entirely optional and board is able to function without it. However, if present logged data will have more precise timestamps.

## Wiring diagram

Here is wiring diagram for all external peripheries.

![Alt text](media/wiring_diagram)

## Functionality description

### User interface



