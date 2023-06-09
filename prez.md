---
marp: true
theme: gaia
_class: lead
style: |
  section {
    font-family: 'Cascadia Code', monospace;
    color: #d0d0d0;
  }
  code {
   font-family: 'Roboto Mono', monospace;
  }
  h1 {
    color: #AA791D;
  }
  a {
   color: gray;
  }
  h2 {
    font-size: 1.1rem;
    color: #AA791D;
  }
  li{
    font-size: 0.9rem;
  }
  ul {
    margin: 0;
  }
  footer a {
   color: gray;
   font-size: .5rem;
  }
  header {
   font-size: .5rem;
   color: grey;
  }

  /* Styling page number */
  section::after {
    color: grey;
    font-weight: bold;
    font-size: 1.5 rem;
  }
paginate: true
backgroundColor: "#252525"
size: 16:9
---
<!--
header: ''
footer: 'Created using Marp'
paginate: false
-->

<!--#
Co budeme probírat? Nebudu Vás učit jak vypadá for cyklus, ale trochu bych s Váma chtěl projít problematiku toho, proč nemáme jen jeden programovací jazyk. 

Nechci z Vás udělat programátory, ale spíš Vám dát nějakej rozhled ohledně toho, co se vlastně děje, a kdybyste se s programátorama bavili, tak ať jim aspoň trochu rozumíte - Alza story
-->

# Data collection device

Jakub Jíra
[github.com/japawBlob](https://github.com/japawBlob)

---


<!--
header: 'Data collection device'
footer: '[japawBlob/data-collection-device](https://github.com/japawBlob/data-collection-device)'
paginate: true
-->

# Goal of project

To create a logging device used in smart home setup

Monitor temperature, humidity, and vibrations and provide measurements to a connected device via UART

Display and store measured values for the user to access

---

# Measuring values 

Sensor DHT11 provides measurements for temperature and humidity

Monitoring temperature using an internal temperature sensor proved highly inaccurate

Built-in accelerometer AIS226DS is used for measuring vibrations

---

# Measuring time

Initially wanted to use an external module DS3231 for RTC

This approach was discarded in favor of built-in RTC on the chip to simplify the design

---


# Displaying logged data

All logged data is stored on the EEPROM

It is able to be read out using UART in .csv format or viewed directly on the display

## UART Communication 

Receiving ```0xdd``` byte sends data in .csv format

Receiving ```0xcc``` byte clears all stored data

---

# User interface

## Home hub

Refreshes every second with new data

Displays current Temperature, Humidity, and Seismic reading

Displayed free space for data entries

![bg right:47% 100%](media/home.jpg)

---

# User interface

## Home hub - Interactions

Button BT2: Open thermostat screen

Button BT1: Open data screen

Rotary encoder: dormant

![bg right:47% 100%](media/home.jpg)

---


# User interface

## Thermostat

Used for sending the desired temperature to a device connected via UART

For setting a temperature on a external device

![bg right:47% 100%](media/thermostat.jpg)

---

# User interface

## Thermostat - Interaction

Button BT2: Send displayed temperature

Button BT1: Back to the Home screen

Rotary encoder: Modify displayed temperature

![bg right:47% 100%](media/thermostat.jpg)

---

# User interface

## Data screen 

Screen displaying all recorded data 

![bg right:47% 100%](media/data.jpg)

---

# User interface

## Data screen - Interaction

Button BT2: Send data in .csv format to UART

Button BT1: Back to the Home screen

Rotary encoder: Scroll through entries

![bg right:47% 100%](media/data.jpg)

---

# Conclusion

With initial simplifications (removing external RTC etc.), all described functionalities work without any known issues

The device successfully records and stores measurements and communicates with connected devices

Overall, I would rate the project as a success
