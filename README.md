# Custom-Keyboard Project

**Schematics**

I want to make a keyboard with a 60% key layout that has a added row on top for a OLED screen, two rotary encoders, and two toggle switches. I started off doing the basic keyboard matrix and quickly moving on to adding the OLED and rotary encoders. While wiring to the pico, I realized I didn't have enough gpio space for all the electronics. To solve this, I added an mcp23017 to give me more room. After adding it, I realized I also had space for RGB leds. I spent maybe 30 minutes adding all the leds, and then moved on to PCB mode. This whole process took a few hours.

[Custom Keyboard (1).pdf](https://github.com/user-attachments/files/30858810/Custom.Keyboard.1.pdf)
