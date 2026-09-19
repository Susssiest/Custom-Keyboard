// Copyright 2026 Susssiest
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

/* ---- I2C (OLED + PCF8574A expander share one bus) ----
 * GP0/GP1 are the RP2040's I2C0 pins. Note the deliberately confusing
 * QMK quirk: the pin defines keep the "I2C1_" prefix even when you
 * select the I2C0 peripheral. This is correct, not a typo. */
#define I2C_DRIVER   I2CD0
#define I2C1_SDA_PIN GP0
#define I2C1_SCL_PIN GP1
#define F_SCL        400000

/* ---- OLED: ER-OLEDM0.91, SSD1306, 128x32 ---- */
#define OLED_DISPLAY_128X32
#define OLED_TIMEOUT    0      // the right rocker owns the screen, not a timer
#define OLED_BRIGHTNESS 140

/* ---- RGB matrix ----
 * 66 WS2812s fed from Pico VBUS through its polyfuse on a 500 mA port.
 * Full white would ask for ~3.9 A, so brightness is hard-capped here and
 * in keyboard.json. Do not raise this without external 5 V power. */
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 110
#define RGB_MATRIX_SLEEP
#define RGB_MATRIX_KEYPRESSES

/* Default animation and colour live in keyboard.json (rgb_matrix.default):
 * cycle_left_right, full saturation, val 0. Val starts at 0 so the board
 * boots dark and fades up in keymap.c -- see the ramp section there. */

/* Diodes are 1N4148 with cathodes on the rows: COL2ROW. */
#define DEBOUNCE 5
