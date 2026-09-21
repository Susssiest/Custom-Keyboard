p.# Custom-Keyboard Project


## Schematics

I want to make a keyboard with a 60% key layout that has a added row on top for a OLED screen, two rotary encoders, and two toggle switches. I started off doing the basic keyboard matrix and quickly moving on to adding the OLED and rotary encoders. While wiring to the pico, I realized I didn't have enough gpio space for all the electronics. To solve this, I added an mcp23017 to give me more room. After adding it, I realized I also had space for WS2812B leds. I spent maybe 30 minutes adding all the leds, and then moved on to PCB mode.

<img width="1088" height="730" alt="Screenshot 2026-08-08 at 9 17 37 AM" src="https://github.com/user-attachments/assets/1f4f2c6d-74a4-4a4b-aeaf-a1b288d92066" />

**Total time spent: 2 hours**

## Orienting Everything in PCB Mode 

I started off with the edge cut for the board outline, and then setting the grid to the distancing between keys, and placing diodes in between. Positioning the rotary encoders, screen, and toggle switches were also pretty easy. The LEDs where my main problem in the PCB editor, after placing a few of them I realized that the LED wouldn't be able to fit on the top side of the board and that they would be crushed by the switches, so I switched it to the bottom layer. After switching layers, the LEDs did give enough room for the keys, but they now faced downwards. I asked a friend and they recommended I use the Sk6812 Mini E Reverse Mount for my keyboard. I found a footprint of the LED and switched all WS2812bBs to the new LEDs, but the LED's footprint was messed up, and I spent around an hour trying to fix it until I just gave up and asked claude. It told me to downlaod one off a github repo, and luckily it worked. I spent another 40 minutes orienting all the LEDs. After finishing, I realized I could add hotswap sockets to make the pcb have holes for the pins in each key instead of having to solder each one on and off. I spent another hour adding the sockets.

<img width="446" height="340" alt="PNG image" src="https://github.com/user-attachments/assets/1d54c4da-b653-4fec-8e3e-98954c007b97" />

**Total time spent: 3 hours**

## Routing

I spent around 2 hours routing my keyboard until I realized that I had the diodes connected to each other in a random order, and had to redo their nets in the schematic editor, and then delete all the routed wires. On my second try my routing vastly improved and it took me maybe an hour and thirty minutes to do everything. I started with 5V to the leds and some of the special electronics, and then 3V3 and SDA and SCK. After that I redid the keyboard matrix with my newly organized diodes and connected the other component's data paths like the toggle switches, rotary encoders, and OLED. I spent maybe 30 minutes playing with the ground fill and another hour making DRC stop having a panic attack, and finally was done routing.

<img width="1126" height="758" alt="Screenshot 2026-08-09 at 4 27 30 PM" src="https://github.com/user-attachments/assets/2b2a7ea1-8b23-4e64-9884-85ce934b52b7" />

**Total time spent: 3 hours**

## Cadding Case

I made my case around the imported 3D model of the pcb. I made the plate and top part split into three pieces to fit on the printer bed, and have a interlocking zigzag connection that will be secured with glue after printing.

<img width="1470" height="923" alt="Screenshot 2026-08-11 at 8 10 50 PM" src="https://github.com/user-attachments/assets/2c2dae7a-f4d5-4aa7-9a5f-b16b3417e933" />

**Total time spent: 30 minutes**

## Adding silkscreen

I added an explosion svg lol. For some reason the svg was imported in fully colored in, so it took a little to remove background.

<img width="896" height="455" alt="Screenshot 2026-08-09 at 4 57 58 PM" src="https://github.com/user-attachments/assets/fd9c340d-e958-4f96-ae82-24cc13d89c62" />

**Total time spent: 10 minutes**

## Added data shifter

I spent around 2 hours adding a data shifter for the LEDs, realized I needed one, luckily I ordered one already for another project, and don't have to add to BOM. Had to rearrange some routing, and also realized that resistor had wrong footprint and fixed that.

<img width="852" height="529" alt="Screenshot 2026-09-16 at 5 57 27 PM" src="https://github.com/user-attachments/assets/4a6d4885-ee8b-4a4e-be62-8de87d9c6b31" />

**Total time spent: 2 hours**

## Replaced GPIO extender

Replaced the MCP23017 with smaller one that uses scl instead of sck (PCF8574AP) and added HC logo silkscreen. Also added resistors and swapped C11 with C13 on pico to PCF8574AP. Took forever rerouting stuff. This whole process took 2 hours.

<img width="856" height="387" alt="PCB" src="https://github.com/user-attachments/assets/f6fa596e-02b1-4d0b-ae9a-0537ac25ce44" />

**Total time spent: 2 hours**

## Had wrong level shifter footprint, changed

**Total time spent: 2 hours**

Had footprint for a SN74AH14 instead of a SN74AHCT125N, replaced, and also added a couple resistors for scl  and sda and rearranged capacitor for leds. I also added more silkscreens.

<img width="486" height="493" alt="Screenshot 2026-09-19 at 11 01 39 AM" src="https://github.com/user-attachments/assets/d217f22a-47e2-4c7f-8421-8f570c99b846" />

## Combined C0 with other columns.
 
 Combined C0 with other columns so that I don't have one column on the PCF8574AP, and added bambu labs logo silkscreen and my very questionable signature.

<img width="1008" height="637" alt="Screenshot 2026-09-19 at 10 59 16 AM" src="https://github.com/user-attachments/assets/762b2791-39d6-427f-b16d-070c5529cb09" />

**Total time spent: 2 hours**

## Finalizing repo.

Replaced all outdated files, made readme, formatted journal. For some reason my schematic reverted to previous version and I needed to redo capacitors.

<img width="1470" height="802" alt="Screenshot 2026-09-19 at 5 32 04 PM" src="https://github.com/user-attachments/assets/677c727a-f89f-4293-b275-a848010b893a" />

**Total time spent: 1 hour**

## Finalized code with qmk.

Talked a bit with perplexity and final code is now finished, I will tweak a little if I run into problems when I have the physical keyboard.

<img width="832" height="736" alt="Screenshot 2026-09-19 at 5 39 36 PM" src="https://github.com/user-attachments/assets/7ea436ed-005a-44aa-b003-799fd8b19a6e" />

**Total time spent: 1 hour**

## Replaced wrong KCD1 THT footrpint

Made a custom footprint for the kcd1, I'm going to solder the pins onto the pcb, but I had a wrong footprint that made small pin holes, and I had to make my own in footprint editor and implement it into the pcb.

<img width="1470" height="898" alt="Screenshot 2026-09-21 at 7 50 41 AM" src="https://github.com/user-attachments/assets/f8b6dabb-c7a2-45ea-a5d6-5d1731df7c67" />

**Total time spent: 30 minutes**

## Added 3d models and fixed footprints

Added all 3D models to board, using step files from grabcad and printables. For some reason some of my footprints were deleted, or switched to other footprints, so I fixed that as well, I also changed the switch's pad orientation because they were a little off centered, and fixed the holes for the hotswap extender thingies because the holes were also a little off centered and they would have had to be sqeezed and potentioally broken to get them to fit. I am pretty confident that I am done with everything.

<img width="1044" height="691" alt="Screenshot 2026-09-20 at 4 58 46 PM" src="https://github.com/user-attachments/assets/87282e24-66ae-47e1-be3d-f7f446f8af3e" />

**Total time spent: 3 hours 30 minutes**

