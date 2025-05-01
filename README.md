# Embedded software for [Toslink Switcher T01](https://cooidea.co/ToslinkSwitcher/T01.html).

Source product T01 can switch one of the incoming SPDIF signals to the output. It can either manually specify the source by the user, or automatically switch to the source where the signal is present and stay there.

I need to switch to the next available source when the main source is switched off, and switch back to the main source when it appears again.

The source software does not have this functionality.

This software is developed for device based on the MCU STM32F030F4.
The original codes of the device manufacturer is not used in any way. Their firmware is locked from reading.
Manufacturer has no relation with this repository.
