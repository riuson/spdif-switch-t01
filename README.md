# Embedded software for [Toslink Switcher T01](https://cooidea.co/ToslinkSwitcher/T01.html).

## Original product

Original product T01 can switch one of the incoming SPDIF signals to the output. It can either manually specify the source by the user, or automatically switch to the source where the signal is present and stay there.

The original codes of the device manufacturer is not used in any way. Their firmware is locked from reading.
Manufacturer has no relation with this repository.

## Goals

I need to switch to the next available source when the main source is switched off, and switch back to the main source when it appears again.

The original software does not have this functionality.

## Added functionality

### v1.1.0

When switching to "AUTO" mode, the LED "AUTO" flashes 1 or 2 times, depending on which mode is selected.

The first mode is similar to the original device.

The second mode is prioritised:

- If the first source is present, it is selected, otherwise the second source is checked.
- If the second source is present, it is selected, otherwise the third source is checked.
- If the third source is present, it is selected, otherwise everything is switched off.
