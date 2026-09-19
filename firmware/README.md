# Keyboard (66) QMK Firmware

66-key ANSI 60% + F-row on a Raspberry Pi Pico.

## Files

```
keyboards/posner/hackpad66/
  keyboard.json                 matrix, pins, encoders, RGB layout, LAYOUT_66
  config.h                      I2C0 on GP0/GP1, OLED, RGB brightness cap
  mcuconf.h                     enables the RP2040 I2C0 peripheral
  halconf.h                     enables ChibiOS I2C
  keymaps/default/
    keymap.c                    keymap, Cmd-Tab encoder, OLED, expander
    rules.mk                    pulls in the standalone I2C master driver
```

## Build

```bash
# once
python3 -m pip install --user qmk
qmk setup                       # clones qmk_firmware and installs the ARM toolchain

# copy this board in
cp -r keyboards/posner ~/qmk_firmware/keyboards/

# compile
qmk compile -kb posner/hackpad66 -km default
```

Flashing: hold BOOTSEL while plugging in the Pico the first time, then drag the
`.uf2` onto the `RPI-RP2` volume. After that, **Fn+Esc** is `QK_BOOT` and puts
the board back into bootloader mode without touching the Pico — which matters,
because RUN is not broken out to a button.

## Pin map

| Function | Pins |
| --- | --- |
| Matrix columns C0–C12 | GP2–GP14 |
| Matrix rows R0–R5 | GP16–GP21 |
| RGB data (66 WS2812) | GP15 → SN74AHCT125N → 330 Ω |
| I2C SDA / SCL | GP0 / GP1 (I2C0) |
| Encoder 1 A / B | GP22 / GP26 |
| Encoder 2 A / B | GP27 / GP28 |

I2C bus: SSD1306 OLED at 0x3C, PCF8574A expander at 0x38 (no collision).

Expander pins: P0 = encoder 1 button, P1 = encoder 2 button,
P2 = left rocker (RGB), P3 = right rocker (screen).

Diode direction is COL2ROW — 1N4148 cathodes on the rows.

## The three relocated switches

To free one GPIO, three switches are wired into matrix row 5 despite sitting
physically elsewhere on the board:

| Switch | Legend | Physical row | Matrix position |
| --- | --- | --- | --- |
| SW7 | `1` | number row | R5, C2 |
| SW21 | `Q` | QWERTY row | R5, C3 |
| SW60 | Left Opt | bottom row | R5, C4 |

`LAYOUT_66` is written in physical reading order, so the keymap hides this.
It is recorded here and at the top of `keymap.c` because nothing in the
schematic or the code hints at it otherwise.

## Behaviour

**Encoder 1 — volume.** `KC_VOLD` / `KC_VOLU`, shaft press is `KC_MUTE`. This
is a host HID consumer-control command, so it adjusts the computer's system
volume and the OS decides which output device that lands on.

**Encoder 2 — app switcher.** Turning presses and holds Cmd, then taps Tab per
detent (Shift+Tab counter-clockwise). Cmd is released 1000 ms after you stop
turning, which commits the switch. Spinning faster than 70 ms per detent sends
2 taps per detent. Shaft press releases Cmd and fires `KC_MCTL` for Mission
Control. Note Cmd+Tab switches *apps*, not individual windows.

Tunables at the top of the Cmd-Tab section: `CMDTAB_COMMIT_MS`,
`CMDTAB_FAST_MS`, `CMDTAB_FAST_STEP`.

**Screen.** Shows currently held keys as `CMD+SHFT+4`. Modifier order is fixed
at Ctrl, Opt, Cmd, Shift, then regular keys. The display is 21 characters wide,
so long combos fall back from `CTRL+OPT+CMD+SHFT` to `C+O+M+S`, and wrap to a
second line only if still too long. Idle shows the board name and active layer.

**Rockers.** Left toggles RGB, right toggles the screen. Both are latching, so
their position is read at boot and applied immediately rather than waited on.

**RGB current safety.** 66 WS2812s at full white would draw roughly 3.9 A, but
they are fed from Pico VBUS through its polyfuse on a 500 mA port. Brightness
is capped at 110 in both `config.h` and `keyboard.json`. Do not raise it
without external 5 V power, and avoid full-white effects. The LEDs also boot
dark and fade in over ~1 s so they are not drawing current during USB
enumeration while the 1000 µF cap is still charging.

## Things to verify on first flash

These are the parts I could not test without your hardware:

1. **LED chain order.** `keyboard.json` assumes LED *n* in the chain
   corresponds to SW *n*. If your PCB routes the data line in a different
   order, per-key effects will look scrambled — fix the `rgb_matrix.layout`
   array, not the keymap.
2. **Encoder direction.** If an encoder turns the wrong way, swap `pin_a` and
   `pin_b` for it in `keyboard.json`.
3. **Encoder resolution.** Set to 4. If one detent produces two steps, change
   it to 2; if it takes two detents to move once, try 8.
4. **Expander address.** 0x38 assumes the PCF8574**A** variant with A0–A2 tied
   low. The non-A PCF8574 is 0x20 instead.
5. **Rocker polarity.** If a rocker is inverted, flip the `&` tests in
   `exp_task()` and `exp_apply_rockers()`.

## Not compiled

I do not have an ARM toolchain in this sandbox, so this has **not** been
through a compiler. The layout data is machine-verified — 66 keys, 66 LEDs,
66 macro arguments, no duplicate matrix positions — but expect to fix a
warning or two on the first build. The most likely spots are the I2C defines
in `config.h`/`mcuconf.h`, which vary between QMK versions, and the `RM_*`
RGB keycode names, which replaced the older `RGB_*` names in 2025.
