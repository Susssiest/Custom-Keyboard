// Copyright 2026 Susssiest
// SPDX-License-Identifier: GPL-2.0-or-later
//
// hackpad66 -- 66-key ANSI 60% + F-row, Raspberry Pi Pico
//   matrix   : 13 cols x 6 rows, COL2ROW (1N4148 cathodes on rows)
//   RGB      : 66 WS2812 on GP15 via SN74AHCT125N buffer
//   OLED     : SSD1306 128x32 on I2C0
//   expander : PCF8574A @ 0x38 -- 2 encoder buttons + 2 latching rockers
//
// ---------------------------------------------------------------------------
// !! READ THIS BEFORE DEBUGGING A "WRONG KEY" !!
//
// Three switches are wired into matrix row 5 even though they sit physically
// in other rows. This was done to free a GPIO pin, and nothing in the
// electrical design hints at it:
//
//     SW7   "1"        physically number row  -> matrix (row 5, col 2)
//     SW21  "Q"        physically QWERTY row  -> matrix (row 5, col 3)
//     SW60  Left Opt   physically bottom row  -> matrix (row 5, col 4)
//
// The LAYOUT_66 macro below is in *physical* reading order, so you should
// never need to think about this -- but if one of those three keys ever
// misbehaves, this is why.
// ---------------------------------------------------------------------------

#include QMK_KEYBOARD_H
#include "i2c_master.h"
#include <string.h>
#include <stdio.h>

enum layers { _BASE = 0, _FN };

enum custom_keycodes {
    WS_NEXT = QK_USER,  // encoder 2 clockwise  -> Cmd-Tab forward
    WS_PREV,            // encoder 2 counter-cw -> Cmd-Tab backward
};

// ===========================================================================
//  Keymap
// ===========================================================================

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_BASE] = LAYOUT_66(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
        KC_LCTL, KC_LALT, KC_LGUI,          KC_SPC,                    KC_RGUI, KC_RALT, MO(_FN), KC_RCTL
    ),

    // Fn layer. QK_BOOT lives on Fn+Esc -- the Pico's RUN pin is not broken
    // out to a button, so this is your only convenient way to reflash.
    [_FN] = LAYOUT_66(
        QK_BOOT, RM_TOGG, RM_NEXT, RM_VALD, RM_VALU,
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______,          _______,                   _______, _______, _______, _______
    ),
};

#if defined(ENCODER_MAP_ENABLE)
// encoder_update_user() is end-of-life in current QMK, so the encoders are
// declared here and the interesting behaviour hangs off custom keycodes that
// process_record_user() intercepts.
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),   ENCODER_CCW_CW(WS_PREV, WS_NEXT) },
    [_FN]   = { ENCODER_CCW_CW(RM_VALD, RM_VALU),   ENCODER_CCW_CW(RM_PREV, RM_NEXT) },
};
#endif

// ===========================================================================
//  Encoder 2: the Cmd-Tab app switcher
// ===========================================================================
//
// Physical Cmd-Tab is "hold Cmd, tap Tab to advance, release Cmd to commit".
// An encoder cannot be held in a turned position -- it only emits pulses while
// actually rotating -- so "release Cmd" is driven by an inactivity timer
// instead: stop turning for CMDTAB_COMMIT_MS and the switcher commits.

#define CMDTAB_COMMIT_MS 1000  // how long after you stop turning before it commits
#define CMDTAB_FAST_MS     70  // detents closer together than this count as "spinning"
#define CMDTAB_FAST_STEP    2  // Tab taps per detent while spinning

static bool     cmdtab_held  = false;
static uint16_t cmdtab_idle  = 0;  // time of the last detent
static uint16_t cmdtab_last  = 0;  // for acceleration measurement

// Drop Cmd immediately and let macOS bring the highlighted app forward.
static void cmdtab_commit(void) {
    if (cmdtab_held) {
        unregister_code(KC_LGUI);
        cmdtab_held = false;
    }
}

static void cmdtab_step(bool forward) {
    if (!cmdtab_held) {
        register_code(KC_LGUI);
        cmdtab_held = true;
        // macOS needs a moment to put the switcher on screen before it will
        // act on Tab presses. Without this the first detent is often eaten.
        wait_ms(30);
    }

    uint8_t steps = 1;
    if (cmdtab_last && TIMER_DIFF_16(timer_read(), cmdtab_last) < CMDTAB_FAST_MS) {
        steps = CMDTAB_FAST_STEP;
    }

    for (uint8_t i = 0; i < steps; i++) {
        if (forward) {
            tap_code(KC_TAB);
        } else {
            // Cmd stays down; Shift is added only for this tap.
            register_code(KC_LSFT);
            tap_code(KC_TAB);
            unregister_code(KC_LSFT);
        }
    }

    cmdtab_last = timer_read();
    cmdtab_idle = cmdtab_last;
}

// ===========================================================================
//  PCF8574A expander -- 2 encoder buttons, 2 latching rockers
// ===========================================================================
//
// The PCF8574 has no direction register. You put a pin into "input" by
// writing a 1 to it, which leaves only a weak ~100 uA pull-up holding it
// high. Every switch here shorts its pin to GND, so asserted == 0.

#define EXP_ADDR    (0x38 << 1)   // QMK wants the address in the upper 7 bits
#define EXP_TIMEOUT 20
#define EXP_POLL_MS 15

#define EXP_RE1_BTN (1 << 0)      // encoder 1 shaft  -> mute
#define EXP_RE2_BTN (1 << 1)      // encoder 2 shaft  -> Mission Control
#define EXP_ROCK_L  (1 << 2)      // left rocker      -> RGB on/off
#define EXP_ROCK_R  (1 << 3)      // right rocker     -> screen on/off

static uint8_t  exp_stable  = 0xFF;  // last debounced reading
static uint8_t  exp_pending = 0xFF;  // candidate awaiting confirmation
static uint16_t exp_timer   = 0;
static bool     exp_primed  = false;

static bool exp_read(uint8_t *out) {
    return i2c_receive(EXP_ADDR, out, 1, EXP_TIMEOUT) == I2C_STATUS_SUCCESS;
}

// Rockers are latching, not momentary. Their position must be *applied*, not
// merely reacted to, or the board powers up with the LEDs lit while the switch
// already says off.
static void exp_apply_rockers(uint8_t v) {
    if (v & EXP_ROCK_L) {
        rgb_matrix_enable_noeeprom();
    } else {
        rgb_matrix_disable_noeeprom();
    }

    if (v & EXP_ROCK_R) {
        oled_on();
    } else {
        oled_off();
    }
}

static void exp_task(void) {
    if (timer_elapsed(exp_timer) < EXP_POLL_MS) return;
    exp_timer = timer_read();

    uint8_t v;
    if (!exp_read(&v)) return;  // a dropped I2C read is survivable; try again

    // Two matching reads in a row before we believe it.
    if (v != exp_pending) { exp_pending = v; return; }
    if (v == exp_stable)  { return; }

    uint8_t changed = v ^ exp_stable;
    exp_stable = v;

    // --- momentary buttons: act on the falling edge (press) ---
    if ((changed & EXP_RE1_BTN) && !(v & EXP_RE1_BTN)) {
        tap_code(KC_MUTE);
    }

    if ((changed & EXP_RE2_BTN) && !(v & EXP_RE2_BTN)) {
        // If the turn logic is currently holding Cmd, firing Mission Control
        // now would send Cmd+MissionControl. Let go of Cmd first.
        cmdtab_commit();
        tap_code(KC_MCTL);
    }

    // --- latching rockers: act on level, not edge ---
    if (changed & EXP_ROCK_L) {
        if (v & EXP_ROCK_L) rgb_matrix_enable_noeeprom();
        else                rgb_matrix_disable_noeeprom();
    }

    if (changed & EXP_ROCK_R) {
        if (v & EXP_ROCK_R) oled_on();
        else                oled_off();
    }
}

// ===========================================================================
//  OLED: show what is being held right now, e.g. "CMD+SHFT+4"
// ===========================================================================

#define HELD_MAX 6
static uint16_t held[HELD_MAX];
static uint8_t  held_n = 0;

static void held_add(uint16_t kc) {
    for (uint8_t i = 0; i < held_n; i++) {
        if (held[i] == kc) return;
    }
    if (held_n < HELD_MAX) held[held_n++] = kc;
}

static void held_del(uint16_t kc) {
    for (uint8_t i = 0; i < held_n; i++) {
        if (held[i] == kc) {
            for (uint8_t j = i; j + 1 < held_n; j++) held[j] = held[j + 1];
            held_n--;
            return;
        }
    }
}

// QMK has no keycode-to-string function, so this table is hand written.
// It only needs to cover the keys that actually exist on this board.
static const char *key_name(uint16_t kc) {
    switch (kc) {
        case KC_A ... KC_Z:  { static char c[2]; c[0] = 'A' + (kc - KC_A); c[1] = 0; return c; }
        case KC_1 ... KC_9:  { static char c[2]; c[0] = '1' + (kc - KC_1); c[1] = 0; return c; }
        case KC_0:    return "0";
        case KC_ESC:  return "ESC";
        case KC_TAB:  return "TAB";
        case KC_SPC:  return "SPACE";
        case KC_ENT:  return "RTN";
        case KC_BSPC: return "DEL";
        case KC_DEL:  return "FWDEL";
        case KC_CAPS: return "CAPS";
        case KC_GRV:  return "`";
        case KC_MINS: return "-";
        case KC_EQL:  return "=";
        case KC_LBRC: return "[";
        case KC_RBRC: return "]";
        case KC_BSLS: return "\\";
        case KC_SCLN: return ";";
        case KC_QUOT: return "'";
        case KC_COMM: return ",";
        case KC_DOT:  return ".";
        case KC_SLSH: return "/";
        case KC_LEFT: return "LEFT";
        case KC_RGHT: return "RIGHT";
        case KC_UP:   return "UP";
        case KC_DOWN: return "DOWN";
        case KC_F1 ... KC_F12: { static char c[4]; snprintf(c, sizeof(c), "F%d", 1 + (kc - KC_F1)); return c; }
        case KC_MUTE: return "MUTE";
        case KC_VOLU: return "VOL+";
        case KC_VOLD: return "VOL-";
        case KC_MCTL: return "MSNCTL";
        default:      return NULL;
    }
}

// Long modifier names first; if the result will not fit the 21-character line
// they collapse to single letters instead of being truncated.
static const char *MOD_LONG[4]  = { "CTRL", "OPT", "CMD", "SHFT" };
static const char *MOD_SHORT[4] = { "C",    "O",   "M",   "S"    };

// Bounded append. Rolling our own because strlcat is not portable across
// every QMK platform and a missing symbol here is a link error, not a warning.
static void sapp(char *out, size_t cap, const char *s) {
    size_t l = strlen(out);
    size_t n = strlen(s);
    if (l + n + 1 > cap) return;
    memcpy(out + l, s, n + 1);
}

static void combo_string(char *out, size_t cap) {
    uint8_t mods  = get_mods();
    bool    on[4] = {
        (mods & MOD_MASK_CTRL)  != 0,
        (mods & MOD_MASK_ALT)   != 0,
        (mods & MOD_MASK_GUI)   != 0,
        (mods & MOD_MASK_SHIFT) != 0,
    };

    // First pass with full names, second pass abbreviated if it overflows.
    for (uint8_t pass = 0; pass < 2; pass++) {
        const char **names = pass ? MOD_SHORT : MOD_LONG;
        out[0] = 0;

        for (uint8_t i = 0; i < 4; i++) {
            if (!on[i]) continue;
            if (out[0]) sapp(out, cap, "+");
            sapp(out, cap, names[i]);
        }

        for (uint8_t i = 0; i < held_n; i++) {
            const char *n = key_name(held[i]);
            if (!n) continue;
            if (out[0]) sapp(out, cap, "+");
            sapp(out, cap, n);
        }

        if (strlen(out) <= 21) return;  // fits one line, done
    }
}

bool oled_task_user(void) {
    char line[48];
    combo_string(line, sizeof(line));

    oled_clear();

    if (line[0] == 0) {
        oled_set_cursor(0, 0);
        oled_write_P(PSTR("hackpad66"), false);
        oled_set_cursor(0, 2);
        oled_write_P(IS_LAYER_ON(_FN) ? PSTR("layer: FN") : PSTR("layer: base"), false);
    } else {
        oled_set_cursor(0, 1);
        oled_write(line, false);
        // Anything still too long after abbreviating wraps to the next line.
        if (strlen(line) > 21) {
            oled_set_cursor(0, 2);
            oled_write(line + 21, false);
        }
    }
    return false;
}

// ===========================================================================
//  Boot: bring the LEDs up gently
// ===========================================================================
//
// The board runs a moving rainbow (cycle_left_right) whenever it is on -- that
// is set as the default animation in keyboard.json.
//
// 1000 uF on the 5 V rail means a hefty inrush the moment USB is plugged in,
// and the Pico is enumerating at the same time. So value starts at 0 and is
// faded up to RGB_RAMP_TARGET here once enumeration has settled. The cycle
// animation drives hue by itself; this only touches brightness.

#define RGB_RAMP_DELAY_MS 600
#define RGB_RAMP_STEP_MS   12
#define RGB_RAMP_TARGET   110

static uint16_t boot_timer = 0;
static uint16_t ramp_timer = 0;
static uint8_t  ramp_val   = 0;
static bool     ramp_done  = false;

static void rgb_ramp_task(void) {
    if (ramp_done) return;
    if (timer_elapsed(boot_timer) < RGB_RAMP_DELAY_MS) return;
    if (timer_elapsed(ramp_timer) < RGB_RAMP_STEP_MS) return;
    ramp_timer = timer_read();

    ramp_val += 4;
    if (ramp_val >= RGB_RAMP_TARGET) {
        ramp_val  = RGB_RAMP_TARGET;
        ramp_done = true;
    }
    rgb_matrix_sethsv_noeeprom(rgb_matrix_get_hue(), rgb_matrix_get_sat(), ramp_val);
}

// ===========================================================================
//  Hooks
// ===========================================================================

void keyboard_post_init_user(void) {
    i2c_init();

    // Park all eight expander pins high so they behave as inputs.
    uint8_t all_high = 0xFF;
    i2c_transmit(EXP_ADDR, &all_high, 1, EXP_TIMEOUT);

    // Read the rockers once and obey them straight away.
    uint8_t v;
    if (exp_read(&v)) {
        exp_stable  = v;
        exp_pending = v;
        exp_primed  = true;
        exp_apply_rockers(v);
    }

    rgb_matrix_sethsv_noeeprom(rgb_matrix_get_hue(), rgb_matrix_get_sat(), 0);
    boot_timer = timer_read();
    ramp_timer = timer_read();
    exp_timer  = timer_read();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case WS_NEXT:
            if (record->event.pressed) cmdtab_step(true);
            return false;

        case WS_PREV:
            if (record->event.pressed) cmdtab_step(false);
            return false;
    }

    // Safety net: any real keystroke while the switcher is open commits it,
    // so a virtually stuck Cmd cannot outlive the gesture.
    if (record->event.pressed && cmdtab_held) {
        cmdtab_commit();
    }

    // Track held keys for the display. Modifiers are read from get_mods()
    // instead, so they are skipped here to avoid showing up twice.
    if (keycode >= KC_A && keycode <= KC_EXSEL) {
        if (!IS_MODIFIER_KEYCODE(keycode)) {
            if (record->event.pressed) held_add(keycode);
            else                       held_del(keycode);
        }
    }

    return true;
}

void housekeeping_task_user(void) {
    // Encoder 2 has stopped turning long enough -- commit the app switch.
    if (cmdtab_held && timer_elapsed(cmdtab_idle) > CMDTAB_COMMIT_MS) {
        cmdtab_commit();
    }

    // If the expander was not reachable at boot, keep trying.
    if (!exp_primed) {
        uint8_t v;
        if (exp_read(&v)) {
            exp_stable  = v;
            exp_pending = v;
            exp_primed  = true;
            exp_apply_rockers(v);
        }
    }

    exp_task();
    rgb_ramp_task();
}

void suspend_power_down_user(void) {
    cmdtab_commit();  // never sleep with Cmd virtually held down
}
