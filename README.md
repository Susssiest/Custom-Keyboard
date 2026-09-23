# Project Writeup
<img width="978" height="616" alt="Screenshot 2026-09-23 at 10 50 05 AM" src="https://github.com/user-attachments/assets/6a91bdc4-55dd-4a46-ac8e-0b28b133955f" />
<img width="737" height="450" alt="Render" src="https://github.com/user-attachments/assets/e82c888c-1b3f-405c-9f08-0759391a8ab1" />



## Summary

This is a custom 60% keyboard equipped with per key rgb leds, rotary encoders, rocker switches, and an oled screen. My orginal idea was to just have leds, rotary encoders, and spst switches, but I quickly realized that I needed to add many additional components to get it to work like I intended. The finished keyboard will have many custom features:

- Leds will uniformly glow, and when is a key ismpressed the led under it will flash brighter than all the other leds for a few seconds before fading.
- Rotary encoders will control screen brightness and window switching.
- Spsts will turn leds on and off, and full keyboard on and off.
- I may change function keys to isntead of working like default function keys, open apps, like kicad or fusion.
- Will have Kaihl hot swap sockets.
- OLED will display simple live keylogger.

# Keyboard Overview

## Keyboard Layout

The keyboard's top layer, (R0), is a little different than a normal 60% keyboard. It's main difference is instead of having a full function key layout, the row starts with escape key, and ends with f4, and then has the screen, rotary encoders, and spsts. I will solder on the hot swap sockets to the pcb, and use cherry mx key switches. The matrix will have diodes to right of each key, for each key. 

<img width="511.5" height="237.5" alt="Screenshot 2026-09-19 at 5 26 41 PM" src="https://github.com/user-attachments/assets/54d9110c-0149-491b-b2ae-6c71396a2815" />

## Controller and GPIO

I'm planning on using a rp2040 pi pico as the controller, and having an extra gpio extender for the 4 extra pins that don't fit on the pico. The pico will be connected to the full matrix, and link to the led data and rotary encoder a and b signals. The gpio extender will have the two rotary encoder buttons connected to it, snd the two spsts. The PCF8574AP will have a 0.1uf capacitor next to it to protect the 3v3 pin.

<img width="434" height="342.5" alt="Screenshot 2026-09-19 at 5 28 19 PM" src="https://github.com/user-attachments/assets/0fbd4cb0-e746-4997-ac84-0c74dbdc96c4" />

## LEDs

LEDs will glow normally and then individually flash when key is pressed, I'm going to limit brightness in firmware as a safeguard. I will have a level shifter for the rgb data to convert the 3.3v data signal to 5v so the leds dont flicker or potentially not recieve commands. I will have a 1000uf capacitor on 5V roght before it reaches the first led. The level shifter will have a 330r resistor before the first led as well. The level shifter will have a 0.1 uf capacitor before 5V to protect the vcc pin.

<img width="679" height="278" alt="Screenshot 2026-09-22 at 10 20 40 AM" src="https://github.com/user-attachments/assets/f52edb4c-c2e1-4c65-aa93-13860c07f318" />

## Rotary Encoders

I'm planning on one the rotary encoders for controlling volume, and another for controlling windows, I'm planning on configuring it to open the CMD+TAB menu and scroll to select and open windows, and then once the rotary encoder has stopped turnign for say, 1 second, the window will open snd the menu will close.

<img width="489" height="276" alt="Screenshot 2026-09-22 at 10 21 06 AM" src="https://github.com/user-attachments/assets/d1f2c5f1-1f84-4533-90c7-36db40073875" />

## SPST Switches

Already mentioned above, one of the spst switches will toggle the LEDs on and off, the other spst will just turn the keyboard on and off.

<img width="386" height="261" alt="Screenshot 2026-09-22 at 10 21 21 AM" src="https://github.com/user-attachments/assets/eade31aa-bac5-4100-b02e-08635f4979c1" />

## OLED

The oled will show a live display of what keys are being pressed, but I may also just put a simple animation on it or logo. There are two 4.7k resistors to protect sda and scl from 3v3.

<img width="331" height="216" alt="Screenshot 2026-09-22 at 10 21 35 AM" src="https://github.com/user-attachments/assets/9de5b64d-0b45-49fa-9b46-07e3fba4cc5e" />

# Cad Assembly

https://github.com/user-attachments/assets/58d7ddcc-f08c-4a4f-aa4c-c66ab165dcce

## BOM

## Keyboard BOM

| Item | Part Number | Description | Quantity | Notes | Price | Total |
| ---: | --- | --- | ---: | --- | ---: | ---: |
| 1 | SC0915 | Rasberry Pi Pico | 1 | Micro controller | $2.27 | $64.19 |
| 2 | MCP23017 | GPIO Extender | 1 | Need one. | $3.23 | — |
| 3 | KY040 | Rotary Encoder | 2 | — | $2.19 | — |
| 4 | SSD1306 | 0.91 in Screen | 1 | — | $1.09 | — |
| 5 | SK6812MINI-E | SK6812 MINI-E Reverse Mount | 66 | Specifically need reverse mount. | $6.04 | — |
| 6 | CPG151101S11 | Kailh Hot Swap Sockets | 66 | Type 1, 70 count. | $8.19 | — |
| 7 | 1N4148 | Diodes | 66 | — | $2.11 | — |
| 8 | N/A | Ceramic Capacitors | 2 (0.1 µF) | — | $2.48 | — |
| 9 | N/A | Stabilizers | 6 | Need 2u, 7u. | $6.30 | — |
| 10 | MX1A | Cherry MX Switches | ~65 | Brown key switches. | $19.10 | — |
| 11 | N/A | Shine-through keycaps | ~65 | — | $11.18 | — |
| 12 | KCD1-101 | Rocker Switch | 2 | Have already | N/A | — |
| 13 | N/A | Resistors | 3 (330 Ω and 470 kΩ) | Have already | N/A | — |
| 14 | Level Shifter | SN74AHCT125N | 1 | Have already. | N/A | — |
| 15 | N/A | Metal Film Capacitor | 1 (1000 µF) | — | N/A | — |

**Listed Total:** $64.19
