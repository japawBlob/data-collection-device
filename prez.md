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

Logging temperature, humidity, vibrations

Measuring time

Displaying logged data

---

# Measuring values 

For temperature and humidity: sensor DHT11

Internal temperature sensor highly innacurate

Built in accelerometer for virbrations

---

# Measuring time

Originally wanted to use DS3231 for RTC

In application build in RTC is used

---


# Displaying logged data

Data is stored in EEPROM memory

Able to be read out using UART in .csv format

Or directly on display

---

# User interface

## Home hub

![h:400](media/home.jpg)

---

# User interface

## Thermostat

![h:400](media/thermostat.jpg)

---

# User interface

## Data screen

![h:400](media/data.jpg)
