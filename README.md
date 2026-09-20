# Project Writeup

<img width="1008" height="637" alt="Screenshot 2026-09-19 at 10 59 16 AM" src="https://github.com/user-attachments/assets/b4cfb4ea-b78d-4952-b10f-fcfceef68a3b" />

## Overview

This is a custom 60% keyboard equipped with per key rgb leds, rotary encoders, rocker switches, and an oled screen. My orginal idea was to just have leds, rotary encoders, and spst switches, but I quickly realized that I needed to add many additional components to get it to work like I intended. The finished keyboard will have many custom features:

- Leds will uniformly glow, and whn is a key ismpressed the led under it will flash brighter than all the other leds for a few seconds before fading.
- Rotary encoders will control screen brightness and window switching.
- Spsts will turn leds on and off, and full keyboard on and off.
- I may change function keys to isntead of working like default function keys, open apps, like kicad or fusion.
- Will have Kaihl hot swap sockets.
- OLED will display simple live keylogger.

## Keyboard Layout

The keyboard's top layer, (R0), is a little different than a normal 60% keyboard. It's main difference is instead of having a full function key layout, the row starts with escape key, and ends with f4, and then has the screen, rotary encoders, and spsts. I will solder on the hot swap sockets to the pcb, and use cherry mx key switches. The matrix will have diodes to right of each key, for each key. 

<img width="511.5" height="237.5" alt="Screenshot 2026-09-19 at 5 26 41 PM" src="https://github.com/user-attachments/assets/54d9110c-0149-491b-b2ae-6c71396a2815" />

## Controller and GPIO

I'm planning on using a rp2040 pi pico as the controller, and having an extra gpio extender for the 4 extra pins that don't fit on the pico. The pico will be connected to the full matrix, and link to the led data and rotary encoder a and b signals. The gpio extender will have the two rotary encoder buttons connected to it, snd the two spsts. The PCF8574AP will have a 0.1uf capacitor next to it to protect the 3v3 pin.

<img width="434" height="342.5" alt="Screenshot 2026-09-19 at 5 28 19 PM" src="https://github.com/user-attachments/assets/0fbd4cb0-e746-4997-ac84-0c74dbdc96c4" />

## LEDs

LEDs will glow normally and then individually flash when key is pressed, I'm going to limit brightness in firmware as a safeguard. I will have a level shifter for the rgb data to convert the 3.3v data signal to 5v so the leds dont flicker or potentially not recieve commands. I will have a 1000uf capacitor on 5V roght before it reaches the first led. The level shifter will have a 330r resistor before the first led as well. The level shifter will have a 0.1 uf capacitor before 5V to protect the vcc pin.

## Rotary Encoders

I'm planning on one the rotary encoders for controlling volume, and another for controlling windows, I'm planning on configuring it to open the CMD+TAB menu and scroll to select and open windows, and then once the rotary encoder has stopped turnign for say, 1 second, the window will open snd the menu will close.

## SPST Switches

Already mentioned above, one of the spst switches will toggle the LEDs on and off, the other spst will just turn the keyboard on and off.

## OLED

The oled will show a live display of what keys are being pressed, but I may also just put a simple animation on it or logo. There are two 4.7k resistors to protect sda and scl from 3v3.

# BOM

<img width="1083" height="274" alt="Screenshot 2026-09-19 at 5 29 30 PM" src="https://github.com/user-attachments/assets/cc28ee22-2b15-4e14-9c7c-1f72234bb463" />

