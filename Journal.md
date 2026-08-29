
# Custom-Keyboard Project


**Schematics**

I want to make a keyboard with a 60% key layout that has a added row on top for a OLED screen, two rotary encoders, and two toggle switches. I started off doing the basic keyboard matrix and quickly moving on to adding the OLED and rotary encoders. While wiring to the pico, I realized I didn't have enough gpio space for all the electronics. To solve this, I added an mcp23017 to give me more room. After adding it, I realized I also had space for WS2812B leds. I spent maybe 30 minutes adding all the leds, and then moved on to PCB mode. This whole process took 2 Hours.

<img width="1088" height="730" alt="Screenshot 2026-08-08 at 9 17 37 AM" src="https://github.com/user-attachments/assets/1f4f2c6d-74a4-4a4b-aeaf-a1b288d92066" />

**Orienting Everything in PCB Mode** 

I started off with the edge cut for the board outline, and then setting the grid to the distancing between keys, and placing diodes in between. Positioning the rotary encoders, screen, and toggle switches were also pretty easy. The LEDs where my main problem in the PCB editor, after placing a few of them I realized that the LED wouldn't be able to fit on the top side of the board and that they would be crushed by the switches, so I switched it to the bottom layer. After switching layers, the LEDs did give enough room for the keys, but they now faced downwards. I asked a friend and they recommended I use the Sk6812 Mini E Reverse Mount for my keyboard. I found a footprint of the LED and switched all WS2812bBs to the new LEDs, but the LED's footprint was messed up, and I spent around an hour trying to fix it until I just gave up and asked claude. It told me to downlaod one off a github repo, and luckily it worked. I spent another 40 minutes orienting all the LEDs. After finishing, I realized I could add hotswap sockets to make the pcb have holes for the pins in each key instead of having to solder each one on and off. I spent another hour adding the sockets. 3 Hours

<img width="446" height="340" alt="PNG image" src="https://github.com/user-attachments/assets/1d54c4da-b653-4fec-8e3e-98954c007b97" />

**Routing**

I spent around 2 hours routing my keyboard until I realized that I had the diodes connected to each other in a random order, and had to redo their nets in the schematic editor, and then delete all the routed wires. On my second try my routing vastly improved and it took me maybe an hour and thirty minutes to do everything. I started with 5V to the leds and some of the special electronics, and then 3V3 and SDA and SCK. After that I redid the keyboard matrix with my newly organized diodes and connected the other component's data paths like the toggle switches, rotary encoders, and OLED. I spent maybe 30 minutes playing with the ground fill and another hour making DRC stop having a panic attack, and finally was done routing. 3 Hours.


<img width="1126" height="758" alt="Screenshot 2026-08-09 at 4 27 30 PM" src="https://github.com/user-attachments/assets/2b2a7ea1-8b23-4e64-9884-85ce934b52b7" />

**Cadding Case**

I made my case around the imported 3D model of the pcb. I made the plate and top part split into three pieces to fit on the printer bed, and have a interlocking zigzag connection that will be secured with glue after printing. 30 Minutes.

<img width="1470" height="923" alt="Screenshot 2026-08-11 at 8 10 50 PM" src="https://github.com/user-attachments/assets/2c2dae7a-f4d5-4aa7-9a5f-b16b3417e933" />

**Finishing touches**

I added an explosion svg lol. 10 minutes.

<img width="896" height="455" alt="Screenshot 2026-08-09 at 4 57 58 PM" src="https://github.com/user-attachments/assets/fd9c340d-e958-4f96-ae82-24cc13d89c62" />

