#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
      KC_Q, KC_W, KC_F, KC_P, KC_G,                                                KC_J, KC_L, KC_U, KC_Y, KC_SCLN,
      LCTL_T(KC_A), LSFT_T(KC_R), LALT_T(KC_S), LGUI_T(KC_T), KC_D,                KC_H, RGUI_T(KC_N), RALT_T(KC_E), RSFT_T(KC_I), RCTL_T(KC_O),
      KC_Z,         RALT_T(KC_X), KC_C, KC_V, KC_B,                                KC_K, KC_M, KC_COMM, RALT_T(KC_DOT), KC_SLSH,
                             LT(2,KC_ESC), LT(1,KC_SPC), KC_BSPC ,                 LT(4,KC_TAB), LT(3,KC_ENT), LT(5,KC_DEL)
    ),

    [1] = LAYOUT(
      QK_BOOT, KC_NO, KC_NO, KC_NO, KC_NO,                                         KC_AGIN, KC_UNDO, KC_CUT, KC_COPY, KC_PSTE,
      KC_LCTL, KC_LSFT, KC_LALT, KC_LGUI, KC_NO,                                   KC_CAPS, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT,
      KC_NO, KC_RALT, KC_NO, KC_NO, KC_NO,                                         KC_INS, KC_HOME, KC_PGDN, KC_PGUP, KC_END,
                            KC_NO, KC_NO, KC_NO,                                   KC_TAB, KC_ENT, KC_DEL
    ),

    [2] = LAYOUT(
      QK_BOOT, KC_NO, KC_NO, KC_NO, KC_NO,                                         KC_NO, KC_NO, KC_NO, KC_NO, KC_MUTE,
      KC_LCTL, KC_LSFT, KC_LALT, KC_LGUI, KC_NO,                                   KC_NO, KC_MPRV, KC_MPLY, KC_MNXT, KC_VOLU,
      KC_NO, KC_RALT, KC_NO, KC_NO, KC_NO,                                         KC_NO, KC_NO, KC_NO, KC_NO, KC_VOLD,
                   KC_NO, KC_NO, KC_NO,                                            KC_NO, KC_NO, KC_NO
    ),

    [3] = LAYOUT(
      KC_LBRC, KC_7, KC_8, KC_9, KC_RBRC,          KC_NO, KC_NO, KC_NO, KC_NO, QK_BOOT,
      KC_SCLN, KC_4, KC_5, KC_6, KC_EQL,           KC_NO, KC_LGUI, KC_RALT, KC_RSFT, KC_RCTL,
      KC_GRV, KC_1, KC_2, KC_3, KC_BSLS,           KC_NO, KC_NO, KC_NO, KC_LALT, KC_NO,
             KC_QUOT, KC_0, KC_MINS,                KC_NO, KC_NO, KC_NO
    ),

    [4] = LAYOUT(
      KC_LCBR, KC_AMPR, KC_ASTR, KC_LPRN, KC_RCBR,        KC_NO, KC_NO, KC_NO, KC_NO, QK_BOOT,
      KC_COLN, KC_DLR, KC_PERC, KC_CIRC, KC_PLUS,         KC_NO, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI,
      KC_TILD, KC_EXLM, KC_AT, KC_HASH, KC_PIPE,          KC_NO, KC_NO, KC_NO, KC_RALT, KC_NO,
      KC_LPRN, KC_RPRN, KC_UNDS,                          KC_NO, KC_NO, KC_NO
    ),

    [5] = LAYOUT(
      KC_F12, KC_F7, KC_F8, KC_F9, KC_PSCR,              KC_NO, KC_NO, KC_NO, KC_NO, QK_BOOT,
      KC_F11, KC_F4, KC_F5, KC_F6, KC_NO,                KC_NO, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL,
      KC_F10, KC_F1, KC_F2, KC_F3, KC_NO,                KC_NO, KC_NO, KC_NO, KC_RALT, KC_NO,
      KC_APP, KC_SPC, KC_TAB,                            KC_NO, KC_NO, KC_NO
    )
};
