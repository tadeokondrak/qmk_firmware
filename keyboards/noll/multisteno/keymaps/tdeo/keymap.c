/* Copyright 2022 Tadeo Kondrak
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "keymap_steno.h"
#include QMK_KEYBOARD_H

enum layer { STENO, ORTHO, CONTROL, QWERTY, SYMBOL, NKRO };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [STENO] = LAYOUT(
        MO(CONTROL), STN_N1, STN_N2, STN_N3, STN_N4,  STN_N5,  STN_N6, STN_N7, STN_N8, STN_N9, STN_NA, STN_FN,
        STN_RES1,    STN_S1, STN_TL, STN_PL, STN_HL, STN_ST1, STN_ST3, STN_FR, STN_PR, STN_LR, STN_TR, STN_DR,
        STN_RES2,    STN_S2, STN_KL, STN_WL, STN_RL, STN_ST2, STN_ST4, STN_RR, STN_BR, STN_GR, STN_SR, STN_ZR,
        STN_A, STN_O, STN_NB, STN_NC, STN_E, STN_U
    ),
    [ORTHO] = LAYOUT(
        TO(STENO), STN_N1, STN_N2, STN_N3, STN_N4, STN_N5,  STN_N6,  STN_N7, STN_N8, STN_N9, STN_NA, STN_FN,
        STN_RES1,  STN_S1, STN_TL, STN_PL, STN_HL, STN_ST1, STN_ST3, STN_FR, STN_PR, STN_LR, STN_TR, STN_DR,
        STN_RES2,  STN_S2, STN_KL, STN_WL, STN_RL, STN_ST2, STN_ST4, STN_RR, STN_BR, STN_GR, STN_SR, STN_ZR,
        STN_A, STN_O, STN_NB, STN_NC, STN_E, STN_U
    ),
    [CONTROL] = LAYOUT(
        KC_NO,      KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        TO(QWERTY), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        TO(ORTHO),  KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        TO(NKRO),   KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    ),
    [QWERTY] = LAYOUT(
        TO(STENO), KC_Q,    KC_W,    KC_E,   KC_R,     KC_T, KC_Y, KC_U, KC_I,    KC_O,   KC_P,     KC_TAB,
        KC_BSPC,   KC_A,    KC_S,    KC_D,   KC_F,     KC_G, KC_H, KC_J, KC_K,    KC_L,   KC_SCLN,  KC_ENTER,
        KC_LSHIFT, KC_Z,    KC_X,    KC_C,   KC_V,     KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLASH, KC_QUOT,
        KC_LWIN,   KC_LALT, KC_LCTL, KC_ESC, KC_SPACE, MO(SYMBOL)
    ),
    [SYMBOL] = LAYOUT(
        TO(STENO), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO,     KC_1,  KC_2,  KC_3,  KC_4,  KC_5,  KC_6,  KC_7,  KC_8,  KC_9,  KC_0,  KC_NO,
        KC_LSHIFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_LWIN, KC_LALT, KC_LCTL, KC_NO, KC_NO, KC_NO
    ),
    [NKRO] = LAYOUT(
        TO(STENO), KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0,    KC_MINS,
        KC_NO, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P,    KC_LBRC,
        KC_NO, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
        KC_C, KC_V, KC_Z, KC_X, KC_N, KC_M
    ),
};

void matrix_init_user(void) {
    steno_set_mode(STENO_MODE_GEMINI);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case STENO:
        palClearPad(GPIOA, 0);
        palSetPad(GPIOA, 1);
        break;
    case ORTHO:
        palSetPad(GPIOA, 0);
        palClearPad(GPIOA, 1);
        break;
    case QWERTY:
    case NKRO:
        palClearPad(GPIOA, 0);
        palClearPad(GPIOA, 1);
        break;
    }
    return state;
}

/* Orthographic system from Cocoa Theory:
 * https://github.com/Kaoffie/cocoa-specs
 */

enum {
	NUMBER_KEY = 1 << 0,
	LEFT_S = 1 << 1,
	LEFT_T = 1 << 2,
	LEFT_K = 1 << 3,
	LEFT_P = 1 << 4,
	LEFT_W = 1 << 5,
	LEFT_H = 1 << 6,
	LEFT_R = 1 << 7,

        INITIALS
            = NUMBER_KEY
            | LEFT_S
            | LEFT_T
            | LEFT_K
            | LEFT_P
            | LEFT_W
            | LEFT_H
            | LEFT_R,

	MID_A = 1 << 8,
	MID_O = 1 << 9,
	ASTERISK = 1 << 10,
	MID_E = 1 << 11,
	MID_U = 1 << 12,

        MEDIALS
            = MID_A
            | MID_O
            | ASTERISK
            | MID_E
            | MID_U,

	RIGHT_F = 1 << 13,
	RIGHT_R = 1 << 14,
	RIGHT_P = 1 << 15,
	RIGHT_B = 1 << 16,
	RIGHT_L = 1 << 17,
	RIGHT_G = 1 << 18,
	RIGHT_T = 1 << 19,
	RIGHT_S = 1 << 20,
	RIGHT_D = 1 << 21,
	RIGHT_Z = 1 << 22,

        FINALS
            = RIGHT_F
            | RIGHT_R
            | RIGHT_P
            | RIGHT_B
            | RIGHT_L
            | RIGHT_G
            | RIGHT_T
            | RIGHT_S
            | RIGHT_D
            | RIGHT_Z,
};

static uint32_t gemini_chord_to_stroke(uint8_t chord[MAX_STROKE_SIZE]) {
    uint32_t stroke = 0;
    if (chord[0] & 0x3F || chord[2] & 0x0C || chord[5] & 0x7E) stroke |= NUMBER_KEY;
    if (chord[1] & 0x60) stroke |= LEFT_S;
    if (chord[1] & 0x10) stroke |= LEFT_T;
    if (chord[1] & 0x08) stroke |= LEFT_K;
    if (chord[1] & 0x04) stroke |= LEFT_P;
    if (chord[1] & 0x02) stroke |= LEFT_W;
    if (chord[1] & 0x01) stroke |= LEFT_H;
    if (chord[2] & 0x40) stroke |= LEFT_R;
    if (chord[2] & 0x20) stroke |= MID_A;
    if (chord[2] & 0x10) stroke |= MID_O;
    if (chord[3] & 0x30) stroke |= ASTERISK;
    if (chord[3] & 0x08) stroke |= MID_E;
    if (chord[3] & 0x04) stroke |= MID_U;
    if (chord[3] & 0x02) stroke |= RIGHT_F;
    if (chord[3] & 0x01) stroke |= RIGHT_R;
    if (chord[4] & 0x40) stroke |= RIGHT_P;
    if (chord[4] & 0x20) stroke |= RIGHT_B;
    if (chord[4] & 0x10) stroke |= RIGHT_L;
    if (chord[4] & 0x08) stroke |= RIGHT_G;
    if (chord[4] & 0x04) stroke |= RIGHT_T;
    if (chord[4] & 0x02) stroke |= RIGHT_S;
    if (chord[4] & 0x01) stroke |= RIGHT_D;
    if (chord[5] & 0x01) stroke |= RIGHT_Z;
    return stroke;
}

struct entry {
    uint32_t stroke;
    char const text[4];
};

#define DOUBLE "\x01"
#define CAPITALIZE "\x02"
#define LOWERCASE "\x03"
#define ATTACH "\x04"

static struct entry const commands[] = {
    { LEFT_S | LEFT_W | LEFT_R, ATTACH },
    { RIGHT_P | RIGHT_L, CAPITALIZE },
    { RIGHT_B | RIGHT_G, LOWERCASE },
};

static struct entry const punctuation[] = {
    { LEFT_T | LEFT_P | RIGHT_P | RIGHT_L, ATTACH "." CAPITALIZE },
    { LEFT_K | LEFT_W | RIGHT_B | RIGHT_G, ATTACH "," },
    { LEFT_T | LEFT_P | RIGHT_B | RIGHT_G, ATTACH "!" CAPITALIZE },
    { LEFT_K | LEFT_W | RIGHT_P | RIGHT_L, ATTACH "?" CAPITALIZE },
};

static struct entry const initial_exceptions[] = {
    /* Number key */
    { NUMBER_KEY | LEFT_K | LEFT_W | LEFT_H, DOUBLE },
    /* Left S */
    { LEFT_S | LEFT_T | LEFT_K | LEFT_P | LEFT_W, "gn" },
    { LEFT_S | LEFT_K | LEFT_P | LEFT_H | LEFT_R, DOUBLE "l" },
    { LEFT_S | LEFT_T | LEFT_K | LEFT_P, "sph" },
    { LEFT_S | LEFT_T | LEFT_K | LEFT_W, "tz" },
    { LEFT_S | LEFT_K | LEFT_P | LEFT_H, "x" },
    { LEFT_S | LEFT_K | LEFT_P | LEFT_R, DOUBLE "r" },
    { LEFT_S | LEFT_K | LEFT_W | LEFT_R, "j" },
    { LEFT_S | LEFT_K | LEFT_H | LEFT_R, "shr" },
    { LEFT_S | LEFT_P | LEFT_H | LEFT_R, "spl" },
    { LEFT_S | LEFT_T | LEFT_K, "q" },
    { LEFT_S | LEFT_T | LEFT_W, "ts" },
    { LEFT_S | LEFT_T | LEFT_P, "ps" },
    { LEFT_S | LEFT_K | LEFT_P, "cz" },
    { LEFT_S | LEFT_K | LEFT_W, "z" },
    { LEFT_S | LEFT_R, "v" },
    /* Left K */
    { LEFT_K | LEFT_W | LEFT_H | LEFT_R, "chl" },
    { LEFT_K | LEFT_P | LEFT_R, "rh" },
    { LEFT_K | LEFT_W | LEFT_H, ATTACH },
    { LEFT_K | LEFT_W | LEFT_R, "y" },
    { LEFT_K | LEFT_H | LEFT_R, "chr" },
    { LEFT_K | LEFT_P, "kl" },
    /* Left T */
    { LEFT_T | LEFT_K | LEFT_P | LEFT_H | LEFT_R, "phl" },
    { LEFT_T | LEFT_K | LEFT_P | LEFT_H, "kn" },
    { LEFT_T | LEFT_P | LEFT_H | LEFT_R, "fl" },
    { LEFT_T | LEFT_P | LEFT_W, "pn" },
    { LEFT_T | LEFT_W | LEFT_H, "thw" },
    { LEFT_T | LEFT_W | LEFT_R, "pt" },
    { LEFT_T | LEFT_H | LEFT_R, "thr" },
    /* Left K */
    { LEFT_K | LEFT_W, "q" },
    /* Left P */
    { LEFT_P | LEFT_H | LEFT_R, "pl" },
    { 0 },
};

static struct entry const initials[] = {
    /* Number key */
    { NUMBER_KEY, ATTACH },
    /* Left S */
    { LEFT_S, "s" },
    /* Left T */
    { LEFT_T | LEFT_K | LEFT_P | LEFT_W, "g" },
    { LEFT_T | LEFT_K | LEFT_P, "ph" },
    { LEFT_T | LEFT_P | LEFT_H, "n" },
    { LEFT_T | LEFT_K, "d" },
    { LEFT_T | LEFT_P, "f" },
    { LEFT_T, "t" },
    /* Left K */
    { LEFT_K | LEFT_P | LEFT_W, "c" },
    { LEFT_K, "k" },
    /* Left P */
    { LEFT_P | LEFT_W, "b" },
    { LEFT_P | LEFT_H, "m" },
    { LEFT_P, "p" },
    /* Left W */
    { LEFT_W, "w" },
    /* Left H */
    { LEFT_H | LEFT_R, "l" },
    { LEFT_H, "h" },
    /* Left R */
    { LEFT_R, "r" },
    /* Null terminator */
    { 0 },
};

static struct entry const medials[] = {
    /* Left A */
    { MID_A | MID_O | MID_E | MID_U | ASTERISK, "eo" },
    { MID_A | MID_O | MID_E | MID_U, "ei" },
    { MID_A | MID_O | MID_E | ASTERISK, "ui" },
    { MID_A | MID_O | MID_U | ASTERISK, "eu" },
    { MID_A | MID_E | MID_U | ASTERISK, "ua" },
    { MID_A | MID_O | MID_E, "ee" },
    { MID_A | MID_O | MID_U, "ue" },
    { MID_A | MID_E | MID_U, "ai" },
    { MID_A | MID_E | ASTERISK, "ea" },
    { MID_A | MID_O | ASTERISK, "ao" },
    { MID_A | MID_E | ASTERISK, "ae" },
    { MID_A | MID_U | ASTERISK, "eou" },
    { MID_A | MID_O, "oo" },
    { MID_A | MID_E, "ea" },
    { MID_A | MID_U, "au" },
    { MID_A | ASTERISK, "ia" },
    { MID_A, "a" },
    /* Left O */
    { MID_O | MID_E | MID_U | ASTERISK, "uo" },
    { MID_O | MID_E | MID_U, "oi" },
    { MID_O | MID_E | ASTERISK, "oe" },
    { MID_O | MID_U | ASTERISK, "iou" },
    { MID_O | MID_E, "oa" },
    { MID_O | MID_U, "ou" },
    { MID_O | ASTERISK, "io" },
    { MID_O, "o" },
    /* Right E */
    { MID_E | MID_U | ASTERISK, "y" },
    { MID_E | MID_U, "i" },
    { MID_E | ASTERISK, "ie" },
    { MID_E, "e" },
    /* Right U */
    { MID_U | ASTERISK, "iu" },
    { MID_U, "u" },
    /* Null terminator */
    { 0 },
};


static struct entry const final_exceptions[] = {
    /* Right F */
    { RIGHT_F | RIGHT_R | RIGHT_P | RIGHT_B | RIGHT_G | RIGHT_S | RIGHT_Z, "nx" },
    { RIGHT_F | RIGHT_R | RIGHT_P | RIGHT_B | RIGHT_D | RIGHT_Z, "nm" },
    { RIGHT_F | RIGHT_R | RIGHT_P | RIGHT_B | RIGHT_G | RIGHT_T, "nct" },
    { RIGHT_F | RIGHT_R | RIGHT_P | RIGHT_B | RIGHT_G | RIGHT_Z, "nc" },
    { RIGHT_F | RIGHT_R | RIGHT_P | RIGHT_B | RIGHT_D, "mn" },
    { RIGHT_F | RIGHT_R | RIGHT_P | RIGHT_B | RIGHT_Z, "rch" },
    { RIGHT_F | RIGHT_L | RIGHT_G | RIGHT_D | RIGHT_Z, "ddl" },
    { RIGHT_F | RIGHT_L | RIGHT_G | RIGHT_T | RIGHT_S, "ttl" },
    { RIGHT_F | RIGHT_R | RIGHT_P | RIGHT_T | RIGHT_S, "mpts" },
    { RIGHT_F | RIGHT_R | RIGHT_P | RIGHT_T, "mpt" },
    { RIGHT_F | RIGHT_R | RIGHT_P | RIGHT_S, "mps" },
    { RIGHT_F | RIGHT_P | RIGHT_L | RIGHT_Z, "mph" },
    { RIGHT_F | RIGHT_P | RIGHT_S | RIGHT_Z, "sch" },
    { RIGHT_F | RIGHT_P | RIGHT_T | RIGHT_D, "tch" },
    { RIGHT_F | RIGHT_R | RIGHT_B | RIGHT_D, "rv" },
    { RIGHT_F | RIGHT_R | RIGHT_B | RIGHT_Z, "rw" },
    { RIGHT_F | RIGHT_R | RIGHT_L | RIGHT_G, "lk" },
    { RIGHT_F | RIGHT_R | RIGHT_L | RIGHT_Z, "lm" },
    { RIGHT_F | RIGHT_R | RIGHT_B | RIGHT_S, "mbs" },
    { RIGHT_F | RIGHT_R | RIGHT_P | RIGHT_B, "nch" },
    { RIGHT_F | RIGHT_R | RIGHT_P, "mp" },
    { RIGHT_F | RIGHT_B | RIGHT_G, "sc" },
    { RIGHT_F | RIGHT_L | RIGHT_D, "lph" },
    { RIGHT_F | RIGHT_L | RIGHT_G, "sk" },
    { RIGHT_F | RIGHT_L | RIGHT_Z, "lf" },
    { RIGHT_F | RIGHT_P | RIGHT_Z, "sp" },
    { RIGHT_F | RIGHT_R | RIGHT_B, "mb" },
    { RIGHT_F | RIGHT_R | RIGHT_D, "rph" },
    { RIGHT_F | RIGHT_R | RIGHT_L, "ml" },
    { RIGHT_F | RIGHT_D, "v" },
    { RIGHT_F | RIGHT_Z, "ph" },
    /* Right R */
    { RIGHT_P | RIGHT_B | RIGHT_G | RIGHT_T | RIGHT_D, "ngth" },
    { RIGHT_P | RIGHT_B | RIGHT_G | RIGHT_S, "ngs" },
    { RIGHT_P | RIGHT_B | RIGHT_G | RIGHT_Z, "gn" },
    { RIGHT_P | RIGHT_B | RIGHT_L | RIGHT_G, "j" },
    { RIGHT_P | RIGHT_B | RIGHT_L | RIGHT_Z, "ln" },
    { RIGHT_P | RIGHT_L | RIGHT_G | RIGHT_Z, "kh" },
    { RIGHT_R | RIGHT_B | RIGHT_G, "rc" },
    { RIGHT_R | RIGHT_B | RIGHT_Z, "rb" },
    { RIGHT_P | RIGHT_B | RIGHT_L, "nl" },
    { RIGHT_P | RIGHT_B | RIGHT_T, "nt" },
    { RIGHT_P | RIGHT_L | RIGHT_Z, "lp" },
    { RIGHT_P | RIGHT_L | RIGHT_D, "pl" },
    { RIGHT_P | RIGHT_L | RIGHT_G, "gm" },
    { RIGHT_P | RIGHT_L | RIGHT_T, "ment" },
    { RIGHT_P | RIGHT_Z, "h" },
    /* Right B */
    { RIGHT_B | RIGHT_L | RIGHT_G | RIGHT_Z, "lc" },
    { RIGHT_B | RIGHT_L | RIGHT_Z, "lb" },
    { RIGHT_B | RIGHT_Z, "y" },
    /* Right G */
    { RIGHT_G | RIGHT_S | RIGHT_Z, "gs" },
    { RIGHT_G | RIGHT_D, "dg" },
    { RIGHT_G | RIGHT_S, "tion" },
    { RIGHT_G | RIGHT_Z, "q" },
    /* Right L */
    { RIGHT_L | RIGHT_G | RIGHT_D | RIGHT_Z, "dl" },
    { RIGHT_L | RIGHT_G | RIGHT_T | RIGHT_S, "tl" },
    { RIGHT_L | RIGHT_G | RIGHT_Z, "lch" },
    { RIGHT_L | RIGHT_T | RIGHT_D, "lth" },
    { RIGHT_L | RIGHT_G | RIGHT_S, "ks" },
    { RIGHT_L | RIGHT_G | RIGHT_D, "lg" },
    { RIGHT_L | RIGHT_G, "k" },
    { RIGHT_L | RIGHT_Z, "ll" },
    /* Right T */
    { RIGHT_T | RIGHT_S | RIGHT_D | RIGHT_Z, "ss" },
    /* Right S */
    { 0 },
};

static struct entry const finals[] = {
    /* Right F */
    { RIGHT_F | RIGHT_R | RIGHT_P | RIGHT_B | RIGHT_G, "nk" },
    { RIGHT_F | RIGHT_P | RIGHT_L, "sm" },
    { RIGHT_P | RIGHT_B | RIGHT_G, "ng" },
    { RIGHT_F | RIGHT_B, "w" },
    { RIGHT_F | RIGHT_R, "rf" },
    { RIGHT_F | RIGHT_R | RIGHT_Z, "rf" },
    { RIGHT_F | RIGHT_P, "ch" },
    { RIGHT_F | RIGHT_Z, "ph" },
    { RIGHT_F, "f" },
    /* Right R */
    { RIGHT_R | RIGHT_B, "sh" },
    { RIGHT_R, "r" },
    /* Right P */
    { RIGHT_P | RIGHT_G | RIGHT_T | RIGHT_S, "ghts" },
    { RIGHT_P | RIGHT_G | RIGHT_S, "ghs" },
    { RIGHT_P | RIGHT_G | RIGHT_T, "ght" },
    { RIGHT_P | RIGHT_G, "gh" },
    { RIGHT_P | RIGHT_B, "n" },
    { RIGHT_P | RIGHT_L, "m" },
    { RIGHT_P, "p" },
    /* Right B */
    { RIGHT_B | RIGHT_L | RIGHT_G, "ck" },
    { RIGHT_B | RIGHT_G | RIGHT_S, "x" },
    { RIGHT_B | RIGHT_G, "c" },
    { RIGHT_B, "b" },
    /* Right L */
    { RIGHT_L | RIGHT_G, "k" },
    { RIGHT_L, "l" },
    /* Right G */
    { RIGHT_G, "g" },
    /* Right T */
    { RIGHT_T | RIGHT_D, "th" },
    { RIGHT_T, "t" },
    /* Right S */
    { RIGHT_S | RIGHT_Z, "st" },
    { RIGHT_S, "s" },
    /* Right D */
    { RIGHT_D, "d" },
    /* Right Z */
    { RIGHT_Z, "z" },
    /* Null terminator */
    { 0 }
};

static char const *check_entries(uint32_t *stroke, struct entry const *entries) {
    do {
        if ((*stroke & entries->stroke) == entries->stroke) {
            *stroke &= ~entries->stroke;
            return entries->text;
        }
    } while ((++entries)->stroke);
    return NULL;
}

static char const *check_entries_exact(uint32_t stroke, struct entry const *entries) {
    do {
        if (stroke == entries->stroke)
            return entries->text;
    } while ((++entries)->stroke);
    return NULL;
}

static struct {
    bool attached;
    bool capitalized;
    bool doubled;
    char word_buf[256];
    uint8_t word_buf_index;
    uint8_t stroke_memory[256];
    uint8_t stroke_memory_index;
} ortho_state;

#define STROKE_CAPITALIZED 0x80
#define STROKE_LENGTH 0x7F

static void undo_stroke(void) {
    ortho_state.stroke_memory_index--;
    uint8_t last_stroke = ortho_state.stroke_memory[ortho_state.stroke_memory_index];
    uint8_t last_stroke_length = last_stroke & STROKE_LENGTH;
    ortho_state.stroke_memory[ortho_state.stroke_memory_index] = 0;
    ortho_state.word_buf_index -= last_stroke_length;
    /* Always provide a way to hit backspace */
    if (last_stroke_length == 0) last_stroke_length = 1;
    while (last_stroke_length--) tap_code(KC_BSPC);
    ortho_state.capitalized = last_stroke & STROKE_CAPITALIZED;
}

static void send_char_and_log(char c) {
    send_char(c);
    ortho_state.word_buf[ortho_state.word_buf_index++] = c;
    ortho_state.stroke_memory[ortho_state.stroke_memory_index]++;
}

static void ortho_send_char(char c) {
    if (c >= 32) {
        if (ortho_state.doubled) {
            char last = ortho_state.word_buf[ortho_state.word_buf_index - 1];
            ortho_state.doubled = false;
            if (last > 32)
                send_char_and_log(last);
        }
        if ((ortho_state.stroke_memory[ortho_state.stroke_memory_index] & STROKE_LENGTH) == 0) {
            if (ortho_state.attached)
                ortho_state.attached = false;
            else
                send_char_and_log(' ');
        }
        if (ortho_state.capitalized) {
            ortho_state.capitalized = false;
            if ('a' <= c && c <= 'z')
                c = 95 & c;
        }
        send_char_and_log(c);
    } else if (c == *DOUBLE) {
        ortho_state.doubled = true;
    } else if (c == *CAPITALIZE) {
        ortho_state.capitalized = true;
    } else if (c == *LOWERCASE) {
        ortho_state.capitalized = false;
    } else if (c == *ATTACH) {
        ortho_state.attached = true;
    }
}

static void ortho_send_string(char const *p) {
    while (*p)
        ortho_send_char(*p++);
}

bool send_steno_chord_user(steno_mode_t mode, uint8_t chord[MAX_STROKE_SIZE]) {
    char const *p;
    uint32_t stroke;
    if (mode != STENO_MODE_GEMINI || !layer_state_is(ORTHO))
        return true;
    stroke = gemini_chord_to_stroke(chord);
    if (stroke == ASTERISK) {
        undo_stroke();
        return false;
    }
    if ((p = check_entries_exact(stroke, commands))) {
        ortho_send_string(p);
        return false;
    }
    ortho_state.stroke_memory[ortho_state.stroke_memory_index] =
        ortho_state.capitalized ? STROKE_CAPITALIZED : 0;
    if ((p = check_entries_exact(stroke, punctuation))) {
        ortho_send_string(p);
        stroke = 0;
    }
    if (!(stroke & (INITIALS | MEDIALS)))
        return false;
    if ((p = check_entries_exact(stroke & INITIALS, initial_exceptions))) {
        ortho_send_string(p);
        stroke &= ~INITIALS;
    }
    while ((p = check_entries(&stroke, initials)))
        ortho_send_string(p);
    if ((p = check_entries_exact(stroke & MEDIALS, medials))) {
        ortho_send_string(p);
        stroke &= ~MEDIALS;
    }
    if ((p = check_entries_exact(stroke & FINALS, final_exceptions))) {
        ortho_send_string(p);
        stroke &= ~FINALS;
    }
    while ((p = check_entries(&stroke, finals)))
        ortho_send_string(p);
    ortho_state.stroke_memory_index++;
    return false;
}
