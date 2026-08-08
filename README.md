<img width="1088" height="730" alt="Screenshot 2026-08-08 at 9 17 37 AM" src="https://github.com/user-attachments/assets/1f4f2c6d-74a4-4a4b-aeaf-a1b288d92066" />
# Custom-Keyboard Project

**Schematics**

I want to make a keyboard with a 60% key layout that has a added row on top for a OLED screen, two rotary encoders, and two toggle switches. I started off doing the basic keyboard matrix and quickly moving on to adding the OLED and rotary encoders. While wiring to the pico, I realized I didn't have enough gpio space for all the electronics. To solve this, I added an mcp23017 to give me more room. After adding it, I realized I also had space for RGB leds. I spent maybe 30 minutes adding all the leds, and then moved on to PCB mode. This whole process took a few hours.

<img width="1100" height="754" alt="Screenshot 2026-08-08 at 9 17 57 AM" src="https://github.com/user-attachments/assets/79f42c52-13cc-4c58-85f3-a7e9f812a678" />
