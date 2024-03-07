#include QMK_KEYBOARD_H

#define _QWERTY 0
#define _COLEMAK 1
#define _GAMING 2
#define _LOWER 3
#define _RAISE 4

#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)

#define QWERTY DF(_QWERTY)
#define COLEMAK DF(_COLEMAK)
#define GAMING DF(_GAMING)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_5x6(
        KC_ESC , KC_1  , KC_2  , KC_3  , KC_4  , KC_5  ,                         KC_6  , KC_7  , KC_8  , KC_9  , KC_0  ,KC_BSPC,
        KC_TAB , KC_Q  , KC_W  , KC_E  , KC_R  , KC_T  ,                         KC_Y  , KC_U  , KC_I  , KC_O  , KC_P  ,KC_MINS,
        KC_LSFT, KC_A  , KC_S  , KC_D  , KC_F  , KC_G  ,                         KC_H  , KC_J  , KC_K  , KC_L  ,KC_SCLN,KC_QUOT,
        KC_LCTL, KC_Z  , KC_X  , KC_C  , KC_V  , KC_B  ,                         KC_N  , KC_M  ,KC_COMM,KC_DOT ,KC_SLSH,KC_BSLS,
                         KC_LBRC,KC_RBRC,                                                       KC_PLUS, KC_EQL,
                                         RAISE,KC_SPC,                        KC_ENT, LOWER,
                                         KC_TAB,KC_HOME,                         KC_END,  KC_DEL,
                                         KC_BSPC, KC_GRV,                        KC_LGUI, KC_LALT
    ),

    [_COLEMAK] = LAYOUT_5x6(
        KC_ESC , KC_1  , KC_2  , KC_3  , KC_4   , KC_5  ,                         KC_6   , KC_7  , KC_8     ,  KC_9  , KC_0   , KC_BSPC,
        KC_TAB , KC_Q  , KC_W  , KC_F  , KC_P   , KC_G  ,                         KC_J   , KC_L  , KC_U     ,  KC_Y  , KC_SCLN, KC_MINS,
        KC_LSFT, KC_A  , KC_R  , KC_S  , KC_T   , KC_D  ,                         KC_H   , KC_N  , KC_E     ,  KC_I  , KC_O   , KC_QUOT,
        KC_LCTL, KC_Z  , KC_X  , KC_C  , KC_V   , KC_B  ,                         KC_K   , KC_M  , KC_COMM  ,  KC_DOT, KC_SLSH, KC_BSLS,
                         KC_LBRC,KC_RBRC,                                                          KC_MINUS , KC_EQL,
                                         RAISE  , KC_SPC,                         KC_ENT , LOWER ,
                                         KC_TAB , KC_HOME,                        KC_END , KC_DEL,
                                         KC_BSPC, KC_GRV,                         KC_LGUI, KC_LALT
    ),

    [_GAMING] = LAYOUT_5x6(
        KC_ESC , _______, KC_1  , KC_2  , KC_3  , KC_4  ,                         KC_5   , KC_6  , KC_7  , KC_8  , KC_9  , KC_0  ,
        KC_TAB , _______, KC_Q  , KC_W  , KC_E  , KC_R  ,                         KC_T   , KC_Y  , KC_U  , KC_I  , KC_O  , KC_P  ,
        KC_LSFT, _______, KC_A  , KC_S  , KC_D  , KC_F  ,                         KC_G   , KC_H  , KC_J  , KC_K  , KC_L  ,KC_SCLN,
        KC_LCTL, _______, KC_Z  , KC_X  , KC_C  , KC_V  ,                         KC_B   , KC_N  , KC_M  ,KC_COMM,KC_DOT ,KC_SLSH,
                          KC_LBRC,KC_RBRC,                                                       KC_PLUS, KC_EQL,
                                         RAISE,KC_SPC,                        KC_ENT, LOWER,
                                         KC_TAB,KC_HOME,                         KC_END,  KC_DEL,
                                         KC_BSPC, KC_GRV,                        KC_LGUI, KC_LALT
    ),

    [_LOWER] = LAYOUT_5x6(
        KC_GRV, KC_EXLM, KC_AT ,KC_HASH,KC_DLR ,KC_PERC,                        KC_CIRC,KC_AMPR,KC_ASTR,KC_LPRN,KC_RPRN,KC_DEL,
        QWERTY, _______,_______,_______,_______,KC_LBRC,                        KC_RBRC, KC_P7 , KC_P8 , KC_P9 ,_______,KC_PLUS,
        COLEMAK,KC_HOME,KC_PGUP,KC_PGDN,KC_END ,KC_LPRN,                        KC_RPRN, KC_P4 , KC_P5 , KC_P6 ,KC_MINS,KC_PIPE,
        GAMING, _______,_______,_______,_______,_______,                        _______, KC_P1 , KC_P2 , KC_P3 ,KC_EQL ,KC_UNDS,
                                                _______,KC_PSCR,            _______, KC_P0,
                                                _______,_______,            _______,_______,
                                                KC_LSFT,_______,            _______,KC_RSFT,
                                                _______,_______,            _______,_______

    ),

    [_RAISE] = LAYOUT_5x6(
        KC_F12 , KC_F1 , KC_F2 , KC_F3 , KC_F4 , KC_F5 ,                        KC_F6  , KC_F7 , KC_F8 , KC_F9 ,KC_F10 ,KC_F11 ,
        QWERTY,_______,_______,_______,_______,KC_LBRC,                        KC_RBRC,_______,KC_NUM,KC_INS ,KC_SCRL,KC_MUTE,
        COLEMAK,KC_LEFT,KC_UP  ,KC_DOWN,KC_RGHT,KC_LPRN,                        KC_RPRN,KC_MPRV,KC_MPLY,KC_MNXT,_______,KC_VOLU,
        GAMING,_______,_______,_______,_______,_______,                        _______,_______,_______,_______,_______,KC_VOLD,
                                                _______,_______,            KC_EQL ,_______,
                                                _______,_______,            _______,_______,
                                                KC_LSFT,_______,            _______,KC_RSFT,
                                                _______,_______,            _______,_______
    )
};
