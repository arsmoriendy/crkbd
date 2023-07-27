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

#include "quantum.h"

#ifdef SWAP_HANDS_ENABLE
__attribute__((weak)) const keypos_t PROGMEM hand_swap_config[MATRIX_ROWS][MATRIX_COLS] = {
    // Left
    {{0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4}},
    {{0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}},
    {{0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}},
    {{0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7}},
    // Right
    {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}},
    {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}},
    {{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}},
    {{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3}}
};
#endif

#ifdef OLED_ENABLE

static const char PROGMEM border_top[] = {0x9d, 0x9f, 0x9f, 0x9f, 0x9e, 0};
static const char PROGMEM border_section[] = {0xde, 0x9f, 0x9f, 0x9f, 0xdf, 0};
static const char PROGMEM border_bottom[] = {0xbd, 0x9f, 0x9f, 0x9f, 0xbe, 0};
static const char PROGMEM border_vertical[] = {0xbf, 0};

oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    return is_keyboard_master() ? rotation : OLED_ROTATION_270;
}

void borderPad(char * string, int n, int padAmount) {
    oled_write_P(border_vertical, false);
    oled_write(string, false);
    for (int i = n; i < padAmount-2; i++) {
        oled_write_char(' ', false);
    }
    oled_write_P(border_vertical, false);
}

static void oled_render_caps_state(void) {
    oled_write_P(border_top, false);
    borderPad("CAP", 3, 5);
    oled_write_P(border_section, false);
    char lock_open[] = {0x20, 0x93, 0xb3, 0};
    char lock_close[] = {0x93, 0x94, 0x92, 0};
    char lock_bottom_open[] = {0xb4, 0xb4, 0x20, 0};
    char lock_bottom_close[] = {0xb4, 0xb4, 0xbf, 0};
    char lock_label_open[] = {'O', 'F', 'F', 0};
    char lock_label_close[] = {'O', 'N', 0xb2, 0};
    if (host_keyboard_led_state().caps_lock) {
        borderPad(lock_close, 3, 5);
        borderPad(lock_bottom_close, 3, 5);
        borderPad(lock_label_close, 3, 5);
    } else {
        borderPad(lock_open, 3, 5);
        borderPad(lock_bottom_open, 3, 5);
        borderPad(lock_label_open, 3, 5);
    }
    oled_write_P(border_section, false);
}

static void oled_render_layer_state(void) {
    borderPad("LYR", 3, 5);
    oled_write_P(border_section, false);
    char current_layer[] = {0x20, 0x30 + get_highest_layer(layer_state), 0};
    borderPad(current_layer, 2, 5);
    oled_write_P(border_section, false);
}

static void oled_render_wpm(void) {
    borderPad("WPM", 3, 5);
    oled_write_P(border_section, false);
    borderPad((char *) get_u8_str(get_current_wpm(), '0'), 3, 5);
    oled_write_P(border_bottom, false);
}

// char     key_name = ' ';
// uint16_t last_keycode;
int8_t last_row = -1;
int8_t last_col = -1;
//
// static const char PROGMEM code_to_name[60] = {' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\', '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

static void set_keylog(keyrecord_t *record) {
    if (record->event.key.row > 3) {
        last_row = (record->event.key.row - 4) * 8;
        last_col = 120 - (record->event.key.col * 8);
    } else {
        last_row = record->event.key.row * 8;
        last_col = record->event.key.col * 8;
    }
}

// static const char *depad_str(const char *depad_str, char depad_char) {
//     while (*depad_str == depad_char)
//         ++depad_str;
//     return depad_str;
// }

// static void oled_render_keylog(void) {
//     borderPad("CxR", 3, 5);
//     oled_write_P(border_section, false);
//     oled_write_P(border_vertical, false);
//     oled_write(last_col, false);
//     oled_write("x", false);
//     oled_write(last_row, false);
//     oled_write_P(border_vertical, false);
//     oled_write_P(border_bottom, false);
// }

// static void render_bootmagic_status(bool status) {
//     /* Show Ctrl-Gui Swap options */
//     static const char PROGMEM logo[][2][3] = {
//         {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
//         {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
//     };
//     if (status) {
//         oled_write_ln_P(logo[0][0], false);
//         oled_write_ln_P(logo[0][1], false);
//     } else {
//         oled_write_ln_P(logo[1][0], false);
//         oled_write_ln_P(logo[1][1], false);
//     }
// }

// __attribute__((weak)) void oled_render_logo(void) {
//     // clang-format off
//     static const char PROGMEM crkbd_logo[] = {
//         0x20, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x20, 0x20,
//         0x20, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0x20, 0x20,
//         0x20, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0x20, 0x20,
//         0};
//     // clang-format on
//     oled_write_P(crkbd_logo, false);
// }

void oled_render_keylog(void) {
    static const char PROGMEM crkbd_layout[] = {
        // 'glyphs', 128x32px
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xf0, 0xf8, 0xf8, 0xfc, 0xfc, 0x7c, 0x7c, 0x7c, 0x3c,
        0x3c, 0x38, 0x30, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xbf, 0xbf,
        0xbf, 0xbf, 0xbf, 0xbf, 0x3e, 0x3e, 0x3c, 0x78, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfc, 0xfc, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0x00,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0f, 0x1f, 0x3f, 0x31, 0x20, 0x20, 0x00, 0x00, 0x00, 0x01,
        0x01, 0x03, 0x03, 0x07, 0x07, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x07, 0x00, 0x00, 0x00, 0x00,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
        0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    oled_write_raw_P(crkbd_layout, sizeof(crkbd_layout));

    if (last_row != -1)
        for (int x = 0; x < 4; x++)
            for (int y = 0; y < 4; y++)
                oled_write_pixel(last_col + 2 + x, last_row + 2 + y, true);
}

bool oled_task_kb(void) {
    if (!oled_task_user()) {
        return false;
    }
    if (is_keyboard_master()) {
        oled_render_keylog();
    } else {
        oled_render_caps_state();
        oled_render_layer_state();
        oled_render_wpm();
    }
    return false;
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        set_keylog(record);
    }
    return process_record_user(keycode, record);
}
#endif // OLED_ENABLE
