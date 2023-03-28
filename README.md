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

![Wiring diagram](media/wiring_diagram)

## Functionality description

### User interface

Basic user interface consists of three main screens.

#### __Main screen__

The main screen displays currently measured values. Function of user interface buttons is displayed on the bottom part of the screen. 
Rotary encoder is dormant.

![Main hub screen](media/home_hub.png)

By pressing set thermostat, we get to __Temperature screen__ and if we choose show data, we will get to the __Data screen__

#### __Temperature screen__

On this screen you can set your desired temperature. This entered temperature will be sent via USART to connected device.

For setting temperature, rotary encoder is used. With left User button, we confirm the selection and with right we return to the __Main screen__. If you leave this screen without confirming the new value, it will be discarded, and original value will be kept.

![Main hub screen](media/temperature_hub.png)

#### __Data screen__

On data screen we can browse all logged data. Using rotary encoder as scroll wheel we can browse all logged data. 
Logged data is stored in .csv format. After pressing left user button, all user data will be sent in .csv format to USART. 
By pressing right user button, we return back to __Main screen__

![Main hub screen](media/data_hub.png)

### Comunication format

Communication with peripheral devices, which use custom communication format follows communication the format. Namely [DHT11](https://components101.com/sites/default/files/component_datasheet/DHT11-Temperature-Sensor.pdf) and [DS3231](https://www.analog.com/media/en/technical-documentation/data-sheets/DS3231.pdf)

Communication via USART device uses 0x03 character as symbol for End of transmition. 

Device supports following transmitions:

#### Send temperature

When setting up desired temperature in __Temperature screen__, confirmation the message is sent via usart. Confiramtion message contains ASCII coded temperature in Celsius and is terminated by 0x03 character.

When sending data on __Data screen__ all data is sent via USART. Message respects .csv format, and ends each line with new line and carrige return characters. Final character is also 0x03 character.




