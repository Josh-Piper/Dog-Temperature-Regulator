# Dog-Temperature-Regulator
This repository is Deakin University's SIT210 final assessment. We had to create an embedded software solution to an ill-defined real world problem. I decided that there is no temperature regulator's for pets in the real world. While air conditioners and heaters exist, not all houses are equipped with these. Moreover, dog jumpers and cooling pads aren't adequate for Australian summers where we have temperatures of 40 degrees celsius and above.

## Overview
The temperature regulator will measure a dog's temperature, and apply heating/cooling when needed to ensure optimal temperatures are maintained. If the dog's temperature is not being regulated expectedly and is at a dangerously high temperature, a push notification is sent to a user's phone using ITFFF. PWM and digital controls can be used for granular control of the fan to act as a multi-layered system.

![How the system works](./images/TooHotArchitecture.png)

![Flow chart of the system logic](./images/flow-chart.png "Flow chart of the system logic")

![How the system triggers push notifications](./images/ITFFF.png)

## Ease of use
To make the embedded system easier to test (notifications and hardware wise) we have developed a NextJS web application.
Here you can view and update the current optimal dog temperature range. This was added since different dog's have different
temperature needs.
![Web ui](./images/web-ui.png)

## Why is there two files?
To ensure the code developed was in working, the simulation program Tinkercad was used for prototyping purposes.
Tinkercad was based on an Arduino device while the solution utilises a Particle Argon and a different fan and heater
from the proposed motors. Thus, there are small differences between the code to accommodate this.

### [TinkerCad Setup](./images/tinkercad-code.ino)
![TinkerCad Schematics](./images/tinkercad-schematic.png "TinkerCad Schematics")

![TinkerCad Device List](./images/tinkercad-device-list.png "TinkerCad Device List")

### [In-person Setup](./particle-argon-code.ino)

![In-person Setup 1](./images/in-person-device-set-up-1.png "In-person setup 1")
![In-person Setup 2](./images/in-person-device-set-up-2.png "In-person setup 2")
