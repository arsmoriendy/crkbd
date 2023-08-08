/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "transactions.h"

enum custom_keycodes {
    OLED_T = QK_USER_0
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_ESC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, KC_LALT,  KC_SPC,     MO(1),  MO(2),   MO(3)
                                      //`--------------------------'  `--------------------------'

  ),

  [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_TRNS,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_TRNS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TRNS, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_LEFT, KC_DOWN,   KC_UP,KC_RIGHT, KC_TILD,  KC_ENT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TRNS, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_HOME, KC_PGDN, KC_PGUP,  KC_END,  KC_DEL, KC_CAPS,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS
                                      //`--------------------------'  `--------------------------'
  ),

  [2] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_TRNS,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                        KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10, KC_TRNS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TRNS,  KC_F11,  KC_F12, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TRNS, RGB_VAI, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, XXXXXXX, KC_TRNS,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS
                                      //`--------------------------'  `--------------------------'
  ),

  [3] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      QK_BOOT,  OLED_T, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS
                                      //`--------------------------'  `--------------------------'
  )
};

static const char PROGMEM border_top[] = {0x9d, 0x9f, 0x9f, 0x9f, 0x9e, 0};
static const char PROGMEM border_section[] = {0xde, 0x9f, 0x9f, 0x9f, 0xdf, 0};
static const char PROGMEM border_bottom[] = {0xbd, 0x9f, 0x9f, 0x9f, 0xbe, 0};
static const char PROGMEM border_vertical[] = {0xbf, 0};

void borderPad(char * string, int n, int padAmount) {
    oled_write_P(border_vertical, false);
    oled_write(string, false);
    for (int i = n; i < padAmount-2; i++) {
        oled_write_char(' ', false);
    }
    oled_write_P(border_vertical, false);
}

void borderPad_P(const char string[], int n, int padAmount) {
    oled_write_P(border_vertical, false);
    oled_write_P(string, false);
    for (int i = n; i < padAmount-2; i++) {
        oled_write_char(' ', false);
    }
    oled_write_P(border_vertical, false);
}

static void oled_render_caps_state(void) {
    oled_write_P(border_top, false);
    borderPad_P(PSTR("CAP"), 3, 5);
    oled_write_P(border_section, false);
    static const char PROGMEM lock_open[] = {0x20, 0x93, 0xb3, 0};
    static const char PROGMEM lock_close[] = {0x93, 0x94, 0x92, 0};
    static const char PROGMEM lock_bottom_open[] = {0xb4, 0xb4, 0x20, 0};
    static const char PROGMEM lock_bottom_close[] = {0xb4, 0xb4, 0xbf, 0};
    static const char PROGMEM lock_label_open[] = {'O', 'F', 'F', 0};
    static const char PROGMEM lock_label_close[] = {'O', 'N', 0xb2, 0};
    if (host_keyboard_led_state().caps_lock) {
        borderPad_P(lock_close, 3, 5);
        borderPad_P(lock_bottom_close, 3, 5);
        borderPad_P(lock_label_close, 3, 5);
    } else {
        borderPad_P(lock_open, 3, 5);
        borderPad_P(lock_bottom_open, 3, 5);
        borderPad_P(lock_label_open, 3, 5);
    }
    oled_write_P(border_section, false);
}

static void oled_render_layer_state(void) {
    borderPad_P(PSTR("LYR"), 3, 5);
    oled_write_P(border_section, false);
    char current_layer[] = {0x20, 0x30 + get_highest_layer(layer_state), 0};
    borderPad(current_layer, 2, 5);
    oled_write_P(border_section, false);
}

static void oled_render_wpm(void) {
    borderPad_P(PSTR("WPM"), 3, 5);
    oled_write_P(border_section, false);
    borderPad((char *) get_u8_str(get_current_wpm(), '0'), 3, 5);
    oled_write_P(border_bottom, false);
}

int8_t last_row = -1;
int8_t last_col = -1;

void key_matrix_sync_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t _out_buflen, void* _out_data) {
    memcpy(&last_row, in_data, sizeof(int8_t));
    memcpy(&last_col, ++in_data, sizeof(int8_t));
}

static void set_keylog(keyrecord_t *record) {
    if (record->event.key.row > 3) {
        last_row = (record->event.key.row - 4) * 8;
        last_col = 120 - (record->event.key.col * 8);
    } else {
        last_row = record->event.key.row * 8;
        last_col = record->event.key.col * 8;
    }
    if (is_keyboard_master()) {
        int8_t key_matrix[] = {last_row, last_col};
        while (!transaction_rpc_send(KEY_MATRIX_SYNC, sizeof(key_matrix), &key_matrix));
    }
}

void oled_render_keylog(void) {
    static const char PROGMEM crkbd_layout[] = { // 512
        // 'glyphs', 128x32px
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xf0,
        0xf8, 0xf8, 0xfc, 0xfc, 0x7c, 0x7c, 0x7c, 0x3c,
        0x3c, 0x38, 0x30, 0x20, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xf9, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xbf, 0xbf,
        0xbf, 0xbf, 0xbf, 0xbf, 0x3e, 0x3e, 0x3c, 0x78,
        0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xfb, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe,
        0xfc, 0xfc, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0x00,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0f, 0x1f,
        0x3f, 0x31, 0x20, 0x20, 0x00, 0x00, 0x00, 0x01,
        0x01, 0x03, 0x03, 0x07, 0x07, 0x0f, 0x0f, 0x0f,
        0x0f, 0x0f, 0x0f, 0x07, 0x00, 0x00, 0x00, 0x00,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    oled_write_raw_P(crkbd_layout, sizeof(crkbd_layout));
    if (last_row != -1) {
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                oled_write_pixel(last_col + 2 + x, last_row + 2 + y, true);
            }
        }
    }
}

bool allow_oled_on = true;

void allow_oled_on_sync_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t _out_buflen, void* _out_data) {
    memcpy(&allow_oled_on, in_data, in_buflen);
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(ALLOW_OLED_ON_SYNC, allow_oled_on_sync_slave_handler);
    transaction_register_rpc(KEY_MATRIX_SYNC, key_matrix_sync_slave_handler);
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return is_keyboard_master() ? OLED_ROTATION_270 : rotation;
}

bool oled_task_user(void) {
    if (!allow_oled_on) {
        oled_clear();
        oled_off();
        return false;
    }
    if (is_keyboard_master()) {
        oled_render_caps_state();
        oled_render_layer_state();
        oled_render_wpm();
    } else {
        oled_render_keylog();
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch(keycode) {
            case OLED_T: // toggle oled
                allow_oled_on = !allow_oled_on;
                if (is_keyboard_master()) {
                    while (!transaction_rpc_send(ALLOW_OLED_ON_SYNC, sizeof(allow_oled_on), &allow_oled_on));
                }
                return false;
        }
        set_keylog(record);
    }
    return true;
}
