# Project Writeup

<img width="1008" height="637" alt="Screenshot 2026-09-19 at 10 59 16 AM" src="https://github.com/user-attachments/assets/b4cfb4ea-b78d-4952-b10f-fcfceef68a3b" />

## Overview

This is a custom 60% keyboard equipped with per key rgb leds, rotary encoders, rocker switches, and an oled screen. My orginal idea was to just have leds, rotary encoders, and spst switches, but I quickly realized that I needed to add many additional components to get it to work like I intended. The finished keyboard will have many custom features:

- Leds will uniformly glow, and whn is a key ismpressed the led under it will flash brighter than all the other leds for a few seconds before fading.
- Rotary encoders will control screen brightness and window switching.
- Spsts will turn leds on and off, and full keyboard on and off.
- I may change function keys to isntead of working like default function keys, open apps, like kicad or fusion.

## Keyboard Layout

The keyboards top layer, (R0), is a little different than a normal 60% keyboard. It's main difference is instead of having a full function key layout, the row starts with escape key, and ends with f4, and then has the screen, rotary encoders, and spsts.

## Controller and GPIO

I'm planning on using a rp2040 pi pico as the controller, and having an extra gpio extender for the 4 extra pins that don't fit on the pico. The pico will be connected to the full matrix, and link to the led data and rotary encoder a and b signals. The gpio extender will have the two rotary encoder buttons connected to it, snd the two spsts.

## LED Behavior

LEDs will glow normally and then individually flash when key is pressed, I'm going to limit brightness in firmware as a safeguard. I will have a level shifter for the rgb data to convert the 3.3v data signal to 5v so the leds dont flicker or potentially not recieve commands.

## Rotary Encoders

I'm planning on one the rotary encoders for controlling volume, and another for controlling windows, I'm planning on configuring it to open the CMD+TAB menu and scroll to select and open windows, and then once the rotary encoder has stopped turnign for say, 1 second, the window will open snd the menu will close.

## SPST Switches

Already mentioned above, one of the spst switches will toggle the LEDs on and off, the other spst will just turn the keyboard on and off.


