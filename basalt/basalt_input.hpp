#pragma once

#include "basalt_macros.hpp"

enum KeyCode : uint;
enum ScanCode : uint;

Point GetMousePosition();

bool IsMouseDown();
bool IsMousePressed();
bool IsMouseUp();

bool IsKeyDown(KeyCode code);
bool IsKeyUp(KeyCode code);

bool IsKeyPressed(KeyCode code);
bool IsKeyReleased(KeyCode code);

#define SCANCODE_MASK (1 << 30)
#define SCANCODE_TO_KEYCODE(X) (X | SCANCODE_MASK)

enum ScanCode : uint {
    SCANCODE_UNKNOWN = 0,

    /**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
    /* @{ */

    SCANCODE_A = 4,
    SCANCODE_B = 5,
    SCANCODE_C = 6,
    SCANCODE_D = 7,
    SCANCODE_E = 8,
    SCANCODE_F = 9,
    SCANCODE_G = 10,
    SCANCODE_H = 11,
    SCANCODE_I = 12,
    SCANCODE_J = 13,
    SCANCODE_K = 14,
    SCANCODE_L = 15,
    SCANCODE_M = 16,
    SCANCODE_N = 17,
    SCANCODE_O = 18,
    SCANCODE_P = 19,
    SCANCODE_Q = 20,
    SCANCODE_R = 21,
    SCANCODE_S = 22,
    SCANCODE_T = 23,
    SCANCODE_U = 24,
    SCANCODE_V = 25,
    SCANCODE_W = 26,
    SCANCODE_X = 27,
    SCANCODE_Y = 28,
    SCANCODE_Z = 29,

    SCANCODE_1 = 30,
    SCANCODE_2 = 31,
    SCANCODE_3 = 32,
    SCANCODE_4 = 33,
    SCANCODE_5 = 34,
    SCANCODE_6 = 35,
    SCANCODE_7 = 36,
    SCANCODE_8 = 37,
    SCANCODE_9 = 38,
    SCANCODE_0 = 39,

    SCANCODE_RETURN = 40,
    SCANCODE_ESCAPE = 41,
    SCANCODE_BACKSPACE = 42,
    SCANCODE_TAB = 43,
    SCANCODE_SPACE = 44,

    SCANCODE_MINUS = 45,
    SCANCODE_EQUALS = 46,
    SCANCODE_LEFTBRACKET = 47,
    SCANCODE_RIGHTBRACKET = 48,
    SCANCODE_BACKSLASH = 49, /**< Located at the lower left of the return
                              *   key on ISO keyboards and at the right end
                              *   of the QWERTY row on ANSI keyboards.
                              *   Produces REVERSE SOLIDUS (backslash) and
                              *   VERTICAL LINE in a US layout, REVERSE
                              *   SOLIDUS and VERTICAL LINE in a UK Mac
                              *   layout, NUMBER SIGN and TILDE in a UK
                              *   Windows layout, DOLLAR SIGN and POUND SIGN
                              *   in a Swiss German layout, NUMBER SIGN and
                              *   APOSTROPHE in a German layout, GRAVE
                              *   ACCENT and POUND SIGN in a French Mac
                              *   layout, and ASTERISK and MICRO SIGN in a
                              *   French Windows layout.
                              */
    SCANCODE_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                              *   instead of 49 for the same key, but all
                              *   OSes I've seen treat the two codes
                              *   identically. So, as an implementor, unless
                              *   your keyboard generates both of those
                              *   codes and your OS treats them differently,
                              *   you should generate SCANCODE_BACKSLASH
                              *   instead of this code. As a user, you
                              *   should not rely on this code because SDL
                              *   will never generate it with most (all?)
                              *   keyboards.
                              */
    SCANCODE_SEMICOLON = 51,
    SCANCODE_APOSTROPHE = 52,
    SCANCODE_GRAVE = 53, /**< Located in the top left corner (on both ANSI
                          *   and ISO keyboards). Produces GRAVE ACCENT and
                          *   TILDE in a US Windows layout and in US and UK
                          *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                          *   and NOT SIGN in a UK Windows layout, SECTION
                          *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                          *   layouts on ISO keyboards, SECTION SIGN and
                          *   DEGREE SIGN in a Swiss German layout (Mac:
                          *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                          *   DEGREE SIGN in a German layout (Mac: only on
                          *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                          *   French Windows layout, COMMERCIAL AT and
                          *   NUMBER SIGN in a French Mac layout on ISO
                          *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                          *   SIGN in a Swiss German, German, or French Mac
                          *   layout on ANSI keyboards.
                          */
    SCANCODE_COMMA = 54,
    SCANCODE_PERIOD = 55,
    SCANCODE_SLASH = 56,

    SCANCODE_CAPSLOCK = 57,

    SCANCODE_F1 = 58,
    SCANCODE_F2 = 59,
    SCANCODE_F3 = 60,
    SCANCODE_F4 = 61,
    SCANCODE_F5 = 62,
    SCANCODE_F6 = 63,
    SCANCODE_F7 = 64,
    SCANCODE_F8 = 65,
    SCANCODE_F9 = 66,
    SCANCODE_F10 = 67,
    SCANCODE_F11 = 68,
    SCANCODE_F12 = 69,

    SCANCODE_PRINTSCREEN = 70,
    SCANCODE_SCROLLLOCK = 71,
    SCANCODE_PAUSE = 72,
    SCANCODE_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
    SCANCODE_HOME = 74,
    SCANCODE_PAGEUP = 75,
    SCANCODE_DELETE = 76,
    SCANCODE_END = 77,
    SCANCODE_PAGEDOWN = 78,
    SCANCODE_RIGHT = 79,
    SCANCODE_LEFT = 80,
    SCANCODE_DOWN = 81,
    SCANCODE_UP = 82,

    SCANCODE_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                 */
    SCANCODE_KP_DIVIDE = 84,
    SCANCODE_KP_MULTIPLY = 85,
    SCANCODE_KP_MINUS = 86,
    SCANCODE_KP_PLUS = 87,
    SCANCODE_KP_ENTER = 88,
    SCANCODE_KP_1 = 89,
    SCANCODE_KP_2 = 90,
    SCANCODE_KP_3 = 91,
    SCANCODE_KP_4 = 92,
    SCANCODE_KP_5 = 93,
    SCANCODE_KP_6 = 94,
    SCANCODE_KP_7 = 95,
    SCANCODE_KP_8 = 96,
    SCANCODE_KP_9 = 97,
    SCANCODE_KP_0 = 98,
    SCANCODE_KP_PERIOD = 99,

    SCANCODE_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                    *   keyboards have over ANSI ones,
                                    *   located between left shift and Y.
                                    *   Produces GRAVE ACCENT and TILDE in a
                                    *   US or UK Mac layout, REVERSE SOLIDUS
                                    *   (backslash) and VERTICAL LINE in a
                                    *   US or UK Windows layout, and
                                    *   LESS-THAN SIGN and GREATER-THAN SIGN
                                    *   in a Swiss German, German, or French
                                    *   layout. */
    SCANCODE_APPLICATION = 101,    /**< windows contextual menu, compose */
    SCANCODE_POWER = 102,          /**< The USB document says this is a status flag,
                                    *   not a physical key - but some Mac keyboards
                                    *   do have a power key. */
    SCANCODE_KP_EQUALS = 103,
    SCANCODE_F13 = 104,
    SCANCODE_F14 = 105,
    SCANCODE_F15 = 106,
    SCANCODE_F16 = 107,
    SCANCODE_F17 = 108,
    SCANCODE_F18 = 109,
    SCANCODE_F19 = 110,
    SCANCODE_F20 = 111,
    SCANCODE_F21 = 112,
    SCANCODE_F22 = 113,
    SCANCODE_F23 = 114,
    SCANCODE_F24 = 115,
    SCANCODE_EXECUTE = 116,
    SCANCODE_HELP = 117,  /**< AL Integrated Help Center */
    SCANCODE_MENU = 118,  /**< Menu (show menu) */
    SCANCODE_SELECT = 119,
    SCANCODE_STOP = 120,  /**< AC Stop */
    SCANCODE_AGAIN = 121, /**< AC Redo/Repeat */
    SCANCODE_UNDO = 122,  /**< AC Undo */
    SCANCODE_CUT = 123,   /**< AC Cut */
    SCANCODE_COPY = 124,  /**< AC Copy */
    SCANCODE_PASTE = 125, /**< AC Paste */
    SCANCODE_FIND = 126,  /**< AC Find */
    SCANCODE_MUTE = 127,
    SCANCODE_VOLUMEUP = 128,
    SCANCODE_VOLUMEDOWN = 129,
    /* not sure whether there's a reason to enable these */
    /*     SCANCODE_LOCKINGCAPSLOCK = 130,  */
    /*     SCANCODE_LOCKINGNUMLOCK = 131, */
    /*     SCANCODE_LOCKINGSCROLLLOCK = 132, */
    SCANCODE_KP_COMMA = 133,
    SCANCODE_KP_EQUALSAS400 = 134,

    SCANCODE_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
    SCANCODE_INTERNATIONAL2 = 136,
    SCANCODE_INTERNATIONAL3 = 137, /**< Yen */
    SCANCODE_INTERNATIONAL4 = 138,
    SCANCODE_INTERNATIONAL5 = 139,
    SCANCODE_INTERNATIONAL6 = 140,
    SCANCODE_INTERNATIONAL7 = 141,
    SCANCODE_INTERNATIONAL8 = 142,
    SCANCODE_INTERNATIONAL9 = 143,
    SCANCODE_LANG1 = 144,    /**< Hangul/English toggle */
    SCANCODE_LANG2 = 145,    /**< Hanja conversion */
    SCANCODE_LANG3 = 146,    /**< Katakana */
    SCANCODE_LANG4 = 147,    /**< Hiragana */
    SCANCODE_LANG5 = 148,    /**< Zenkaku/Hankaku */
    SCANCODE_LANG6 = 149,    /**< reserved */
    SCANCODE_LANG7 = 150,    /**< reserved */
    SCANCODE_LANG8 = 151,    /**< reserved */
    SCANCODE_LANG9 = 152,    /**< reserved */

    SCANCODE_ALTERASE = 153, /**< Erase-Eaze */
    SCANCODE_SYSREQ = 154,
    SCANCODE_CANCEL = 155,   /**< AC Cancel */
    SCANCODE_CLEAR = 156,
    SCANCODE_PRIOR = 157,
    SCANCODE_RETURN2 = 158,
    SCANCODE_SEPARATOR = 159,
    SCANCODE_OUT = 160,
    SCANCODE_OPER = 161,
    SCANCODE_CLEARAGAIN = 162,
    SCANCODE_CRSEL = 163,
    SCANCODE_EXSEL = 164,

    SCANCODE_KP_00 = 176,
    SCANCODE_KP_000 = 177,
    SCANCODE_THOUSANDSSEPARATOR = 178,
    SCANCODE_DECIMALSEPARATOR = 179,
    SCANCODE_CURRENCYUNIT = 180,
    SCANCODE_CURRENCYSUBUNIT = 181,
    SCANCODE_KP_LEFTPAREN = 182,
    SCANCODE_KP_RIGHTPAREN = 183,
    SCANCODE_KP_LEFTBRACE = 184,
    SCANCODE_KP_RIGHTBRACE = 185,
    SCANCODE_KP_TAB = 186,
    SCANCODE_KP_BACKSPACE = 187,
    SCANCODE_KP_A = 188,
    SCANCODE_KP_B = 189,
    SCANCODE_KP_C = 190,
    SCANCODE_KP_D = 191,
    SCANCODE_KP_E = 192,
    SCANCODE_KP_F = 193,
    SCANCODE_KP_XOR = 194,
    SCANCODE_KP_POWER = 195,
    SCANCODE_KP_PERCENT = 196,
    SCANCODE_KP_LESS = 197,
    SCANCODE_KP_GREATER = 198,
    SCANCODE_KP_AMPERSAND = 199,
    SCANCODE_KP_DBLAMPERSAND = 200,
    SCANCODE_KP_VERTICALBAR = 201,
    SCANCODE_KP_DBLVERTICALBAR = 202,
    SCANCODE_KP_COLON = 203,
    SCANCODE_KP_HASH = 204,
    SCANCODE_KP_SPACE = 205,
    SCANCODE_KP_AT = 206,
    SCANCODE_KP_EXCLAM = 207,
    SCANCODE_KP_MEMSTORE = 208,
    SCANCODE_KP_MEMRECALL = 209,
    SCANCODE_KP_MEMCLEAR = 210,
    SCANCODE_KP_MEMADD = 211,
    SCANCODE_KP_MEMSUBTRACT = 212,
    SCANCODE_KP_MEMMULTIPLY = 213,
    SCANCODE_KP_MEMDIVIDE = 214,
    SCANCODE_KP_PLUSMINUS = 215,
    SCANCODE_KP_CLEAR = 216,
    SCANCODE_KP_CLEARENTRY = 217,
    SCANCODE_KP_BINARY = 218,
    SCANCODE_KP_OCTAL = 219,
    SCANCODE_KP_DECIMAL = 220,
    SCANCODE_KP_HEXADECIMAL = 221,

    SCANCODE_LCTRL = 224,
    SCANCODE_LSHIFT = 225,
    SCANCODE_LALT = 226, /**< alt, option */
    SCANCODE_LGUI = 227, /**< windows, command (apple), meta */
    SCANCODE_RCTRL = 228,
    SCANCODE_RSHIFT = 229,
    SCANCODE_RALT = 230, /**< alt gr, option */
    SCANCODE_RGUI = 231, /**< windows, command (apple), meta */

    SCANCODE_MODE = 257, /**< I'm not sure if this is really not covered
                          *   by any of the above, but since there's a
                          *   special KMOD_MODE for it I'm adding it here
                          */

    /* @} */             /* Usage page 0x07 */

    /**
     *  \name Usage page 0x0C
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     *  See https://usb.org/sites/default/files/hut1_2.pdf
     *
     *  There are way more keys in the spec than we can represent in the
     *  current scancode range, so pick the ones that commonly come up in
     *  real world usage.
     */
    /* @{ */

    SCANCODE_AUDIONEXT = 258,
    SCANCODE_AUDIOPREV = 259,
    SCANCODE_AUDIOSTOP = 260,
    SCANCODE_AUDIOPLAY = 261,
    SCANCODE_AUDIOMUTE = 262,
    SCANCODE_MEDIASELECT = 263,
    SCANCODE_WWW = 264,          /**< AL Internet Browser */
    SCANCODE_MAIL = 265,
    SCANCODE_CALCULATOR = 266,   /**< AL Calculator */
    SCANCODE_COMPUTER = 267,
    SCANCODE_AC_SEARCH = 268,    /**< AC Search */
    SCANCODE_AC_HOME = 269,      /**< AC Home */
    SCANCODE_AC_BACK = 270,      /**< AC Back */
    SCANCODE_AC_FORWARD = 271,   /**< AC Forward */
    SCANCODE_AC_STOP = 272,      /**< AC Stop */
    SCANCODE_AC_REFRESH = 273,   /**< AC Refresh */
    SCANCODE_AC_BOOKMARKS = 274, /**< AC Bookmarks */

    /* @} */                     /* Usage page 0x0C */

    /**
     *  \name Walther keys
     *
     *  These are values that Christian Walther added (for mac keyboard?).
     */
    /* @{ */

    SCANCODE_BRIGHTNESSDOWN = 275,
    SCANCODE_BRIGHTNESSUP = 276,
    SCANCODE_DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                           switch, video mode switch */
    SCANCODE_KBDILLUMTOGGLE = 278,
    SCANCODE_KBDILLUMDOWN = 279,
    SCANCODE_KBDILLUMUP = 280,
    SCANCODE_EJECT = 281,
    SCANCODE_SLEEP = 282, /**< SC System Sleep */

    SCANCODE_APP1 = 283,
    SCANCODE_APP2 = 284,

    /* @} */ /* Walther keys */

    /**
     *  \name Usage page 0x0C (additional media keys)
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     */
    /* @{ */

    SCANCODE_AUDIOREWIND = 285,
    SCANCODE_AUDIOFASTFORWARD = 286,

    /* @} */ /* Usage page 0x0C (additional media keys) */

    /**
     *  \name Mobile keys
     *
     *  These are values that are often used on mobile phones.
     */
    /* @{ */

    SCANCODE_SOFTLEFT = 287,  /**< Usually situated below the display on phones and
                                       used as a multi-function feature key for selecting
                                       a software defined function shown on the bottom left
                                       of the display. */
    SCANCODE_SOFTRIGHT = 288, /**< Usually situated below the display on phones and
                                       used as a multi-function feature key for selecting
                                       a software defined function shown on the bottom right
                                       of the display. */
    SCANCODE_CALL = 289,      /**< Used for accepting phone calls. */
    SCANCODE_ENDCALL = 290,   /**< Used for rejecting phone calls. */

    /* @} */                  /* Mobile keys */

    /* Add any other keys here. */

    NUM_SCANCODES = 512 /**< not a key, just marks the number of scancodes
                                 for array bounds */
};

enum KeyCode : uint {
    KEY_UNKNOWN = 0,

    KEY_RETURN = '\r',
    KEY_ESCAPE = '\x1B',
    KEY_BACKSPACE = '\b',
    KEY_TAB = '\t',
    KEY_SPACE = ' ',
    KEY_EXCLAIM = '!',
    KEY_QUOTEDBL = '"',
    KEY_HASH = '#',
    KEY_PERCENT = '%',
    KEY_DOLLAR = '$',
    KEY_AMPERSAND = '&',
    KEY_QUOTE = '\'',
    KEY_LEFTPAREN = '(',
    KEY_RIGHTPAREN = ')',
    KEY_ASTERISK = '*',
    KEY_PLUS = '+',
    KEY_COMMA = ',',
    KEY_MINUS = '-',
    KEY_PERIOD = '.',
    KEY_SLASH = '/',
    KEY_0 = '0',
    KEY_1 = '1',
    KEY_2 = '2',
    KEY_3 = '3',
    KEY_4 = '4',
    KEY_5 = '5',
    KEY_6 = '6',
    KEY_7 = '7',
    KEY_8 = '8',
    KEY_9 = '9',
    KEY_COLON = ':',
    KEY_SEMICOLON = ';',
    KEY_LESS = '<',
    KEY_EQUALS = '=',
    KEY_GREATER = '>',
    KEY_QUESTION = '?',
    KEY_AT = '@',

    /*
       Skip uppercase letters
     */

    KEY_LEFTBRACKET = '[',
    KEY_BACKSLASH = '\\',
    KEY_RIGHTBRACKET = ']',
    KEY_CARET = '^',
    KEY_UNDERSCORE = '_',
    KEY_BACKQUOTE = '`',
    KEY_a = 'a',
    KEY_b = 'b',
    KEY_c = 'c',
    KEY_d = 'd',
    KEY_e = 'e',
    KEY_f = 'f',
    KEY_g = 'g',
    KEY_h = 'h',
    KEY_i = 'i',
    KEY_j = 'j',
    KEY_k = 'k',
    KEY_l = 'l',
    KEY_m = 'm',
    KEY_n = 'n',
    KEY_o = 'o',
    KEY_p = 'p',
    KEY_q = 'q',
    KEY_r = 'r',
    KEY_s = 's',
    KEY_t = 't',
    KEY_u = 'u',
    KEY_v = 'v',
    KEY_w = 'w',
    KEY_x = 'x',
    KEY_y = 'y',
    KEY_z = 'z',

    KEY_CAPSLOCK = SCANCODE_TO_KEYCODE(SCANCODE_CAPSLOCK),

    KEY_F1 = SCANCODE_TO_KEYCODE(SCANCODE_F1),
    KEY_F2 = SCANCODE_TO_KEYCODE(SCANCODE_F2),
    KEY_F3 = SCANCODE_TO_KEYCODE(SCANCODE_F3),
    KEY_F4 = SCANCODE_TO_KEYCODE(SCANCODE_F4),
    KEY_F5 = SCANCODE_TO_KEYCODE(SCANCODE_F5),
    KEY_F6 = SCANCODE_TO_KEYCODE(SCANCODE_F6),
    KEY_F7 = SCANCODE_TO_KEYCODE(SCANCODE_F7),
    KEY_F8 = SCANCODE_TO_KEYCODE(SCANCODE_F8),
    KEY_F9 = SCANCODE_TO_KEYCODE(SCANCODE_F9),
    KEY_F10 = SCANCODE_TO_KEYCODE(SCANCODE_F10),
    KEY_F11 = SCANCODE_TO_KEYCODE(SCANCODE_F11),
    KEY_F12 = SCANCODE_TO_KEYCODE(SCANCODE_F12),

    KEY_PRINTSCREEN = SCANCODE_TO_KEYCODE(SCANCODE_PRINTSCREEN),
    KEY_SCROLLLOCK = SCANCODE_TO_KEYCODE(SCANCODE_SCROLLLOCK),
    KEY_PAUSE = SCANCODE_TO_KEYCODE(SCANCODE_PAUSE),
    KEY_INSERT = SCANCODE_TO_KEYCODE(SCANCODE_INSERT),
    KEY_HOME = SCANCODE_TO_KEYCODE(SCANCODE_HOME),
    KEY_PAGEUP = SCANCODE_TO_KEYCODE(SCANCODE_PAGEUP),
    KEY_DELETE = '\x7F',
    KEY_END = SCANCODE_TO_KEYCODE(SCANCODE_END),
    KEY_PAGEDOWN = SCANCODE_TO_KEYCODE(SCANCODE_PAGEDOWN),
    KEY_RIGHT = SCANCODE_TO_KEYCODE(SCANCODE_RIGHT),
    KEY_LEFT = SCANCODE_TO_KEYCODE(SCANCODE_LEFT),
    KEY_DOWN = SCANCODE_TO_KEYCODE(SCANCODE_DOWN),
    KEY_UP = SCANCODE_TO_KEYCODE(SCANCODE_UP),

    KEY_NUMLOCKCLEAR = SCANCODE_TO_KEYCODE(SCANCODE_NUMLOCKCLEAR),
    KEY_KP_DIVIDE = SCANCODE_TO_KEYCODE(SCANCODE_KP_DIVIDE),
    KEY_KP_MULTIPLY = SCANCODE_TO_KEYCODE(SCANCODE_KP_MULTIPLY),
    KEY_KP_MINUS = SCANCODE_TO_KEYCODE(SCANCODE_KP_MINUS),
    KEY_KP_PLUS = SCANCODE_TO_KEYCODE(SCANCODE_KP_PLUS),
    KEY_KP_ENTER = SCANCODE_TO_KEYCODE(SCANCODE_KP_ENTER),
    KEY_KP_1 = SCANCODE_TO_KEYCODE(SCANCODE_KP_1),
    KEY_KP_2 = SCANCODE_TO_KEYCODE(SCANCODE_KP_2),
    KEY_KP_3 = SCANCODE_TO_KEYCODE(SCANCODE_KP_3),
    KEY_KP_4 = SCANCODE_TO_KEYCODE(SCANCODE_KP_4),
    KEY_KP_5 = SCANCODE_TO_KEYCODE(SCANCODE_KP_5),
    KEY_KP_6 = SCANCODE_TO_KEYCODE(SCANCODE_KP_6),
    KEY_KP_7 = SCANCODE_TO_KEYCODE(SCANCODE_KP_7),
    KEY_KP_8 = SCANCODE_TO_KEYCODE(SCANCODE_KP_8),
    KEY_KP_9 = SCANCODE_TO_KEYCODE(SCANCODE_KP_9),
    KEY_KP_0 = SCANCODE_TO_KEYCODE(SCANCODE_KP_0),
    KEY_KP_PERIOD = SCANCODE_TO_KEYCODE(SCANCODE_KP_PERIOD),

    KEY_APPLICATION = SCANCODE_TO_KEYCODE(SCANCODE_APPLICATION),
    KEY_POWER = SCANCODE_TO_KEYCODE(SCANCODE_POWER),
    KEY_KP_EQUALS = SCANCODE_TO_KEYCODE(SCANCODE_KP_EQUALS),
    KEY_F13 = SCANCODE_TO_KEYCODE(SCANCODE_F13),
    KEY_F14 = SCANCODE_TO_KEYCODE(SCANCODE_F14),
    KEY_F15 = SCANCODE_TO_KEYCODE(SCANCODE_F15),
    KEY_F16 = SCANCODE_TO_KEYCODE(SCANCODE_F16),
    KEY_F17 = SCANCODE_TO_KEYCODE(SCANCODE_F17),
    KEY_F18 = SCANCODE_TO_KEYCODE(SCANCODE_F18),
    KEY_F19 = SCANCODE_TO_KEYCODE(SCANCODE_F19),
    KEY_F20 = SCANCODE_TO_KEYCODE(SCANCODE_F20),
    KEY_F21 = SCANCODE_TO_KEYCODE(SCANCODE_F21),
    KEY_F22 = SCANCODE_TO_KEYCODE(SCANCODE_F22),
    KEY_F23 = SCANCODE_TO_KEYCODE(SCANCODE_F23),
    KEY_F24 = SCANCODE_TO_KEYCODE(SCANCODE_F24),
    // KEY_EXECUTE = SCANCODE_TO_KEYCODE(SCANCODE_EXECUTE), TODO FIXME HIDDEN MACRO???? DOESNT
    // COMPILE
    KEY_HELP = SCANCODE_TO_KEYCODE(SCANCODE_HELP),
    KEY_MENU = SCANCODE_TO_KEYCODE(SCANCODE_MENU),
    KEY_SELECT = SCANCODE_TO_KEYCODE(SCANCODE_SELECT),
    KEY_STOP = SCANCODE_TO_KEYCODE(SCANCODE_STOP),
    KEY_AGAIN = SCANCODE_TO_KEYCODE(SCANCODE_AGAIN),
    KEY_UNDO = SCANCODE_TO_KEYCODE(SCANCODE_UNDO),
    KEY_CUT = SCANCODE_TO_KEYCODE(SCANCODE_CUT),
    KEY_COPY = SCANCODE_TO_KEYCODE(SCANCODE_COPY),
    KEY_PASTE = SCANCODE_TO_KEYCODE(SCANCODE_PASTE),
    KEY_FIND = SCANCODE_TO_KEYCODE(SCANCODE_FIND),
    KEY_MUTE = SCANCODE_TO_KEYCODE(SCANCODE_MUTE),
    KEY_VOLUMEUP = SCANCODE_TO_KEYCODE(SCANCODE_VOLUMEUP),
    KEY_VOLUMEDOWN = SCANCODE_TO_KEYCODE(SCANCODE_VOLUMEDOWN),
    KEY_KP_COMMA = SCANCODE_TO_KEYCODE(SCANCODE_KP_COMMA),
    KEY_KP_EQUALSAS400 = SCANCODE_TO_KEYCODE(SCANCODE_KP_EQUALSAS400),

    KEY_ALTERASE = SCANCODE_TO_KEYCODE(SCANCODE_ALTERASE),
    KEY_SYSREQ = SCANCODE_TO_KEYCODE(SCANCODE_SYSREQ),
    KEY_CANCEL = SCANCODE_TO_KEYCODE(SCANCODE_CANCEL),
    KEY_CLEAR = SCANCODE_TO_KEYCODE(SCANCODE_CLEAR),
    KEY_PRIOR = SCANCODE_TO_KEYCODE(SCANCODE_PRIOR),
    KEY_RETURN2 = SCANCODE_TO_KEYCODE(SCANCODE_RETURN2),
    KEY_SEPARATOR = SCANCODE_TO_KEYCODE(SCANCODE_SEPARATOR),
    KEY_OUT = SCANCODE_TO_KEYCODE(SCANCODE_OUT),
    KEY_OPER = SCANCODE_TO_KEYCODE(SCANCODE_OPER),
    KEY_CLEARAGAIN = SCANCODE_TO_KEYCODE(SCANCODE_CLEARAGAIN),
    KEY_CRSEL = SCANCODE_TO_KEYCODE(SCANCODE_CRSEL),
    KEY_EXSEL = SCANCODE_TO_KEYCODE(SCANCODE_EXSEL),

    KEY_KP_00 = SCANCODE_TO_KEYCODE(SCANCODE_KP_00),
    KEY_KP_000 = SCANCODE_TO_KEYCODE(SCANCODE_KP_000),
    KEY_THOUSANDSSEPARATOR = SCANCODE_TO_KEYCODE(SCANCODE_THOUSANDSSEPARATOR),
    KEY_DECIMALSEPARATOR = SCANCODE_TO_KEYCODE(SCANCODE_DECIMALSEPARATOR),
    KEY_CURRENCYUNIT = SCANCODE_TO_KEYCODE(SCANCODE_CURRENCYUNIT),
    KEY_CURRENCYSUBUNIT = SCANCODE_TO_KEYCODE(SCANCODE_CURRENCYSUBUNIT),
    KEY_KP_LEFTPAREN = SCANCODE_TO_KEYCODE(SCANCODE_KP_LEFTPAREN),
    KEY_KP_RIGHTPAREN = SCANCODE_TO_KEYCODE(SCANCODE_KP_RIGHTPAREN),
    KEY_KP_LEFTBRACE = SCANCODE_TO_KEYCODE(SCANCODE_KP_LEFTBRACE),
    KEY_KP_RIGHTBRACE = SCANCODE_TO_KEYCODE(SCANCODE_KP_RIGHTBRACE),
    KEY_KP_TAB = SCANCODE_TO_KEYCODE(SCANCODE_KP_TAB),
    KEY_KP_BACKSPACE = SCANCODE_TO_KEYCODE(SCANCODE_KP_BACKSPACE),
    KEY_KP_A = SCANCODE_TO_KEYCODE(SCANCODE_KP_A),
    KEY_KP_B = SCANCODE_TO_KEYCODE(SCANCODE_KP_B),
    KEY_KP_C = SCANCODE_TO_KEYCODE(SCANCODE_KP_C),
    KEY_KP_D = SCANCODE_TO_KEYCODE(SCANCODE_KP_D),
    KEY_KP_E = SCANCODE_TO_KEYCODE(SCANCODE_KP_E),
    KEY_KP_F = SCANCODE_TO_KEYCODE(SCANCODE_KP_F),
    KEY_KP_XOR = SCANCODE_TO_KEYCODE(SCANCODE_KP_XOR),
    KEY_KP_POWER = SCANCODE_TO_KEYCODE(SCANCODE_KP_POWER),
    KEY_KP_PERCENT = SCANCODE_TO_KEYCODE(SCANCODE_KP_PERCENT),
    KEY_KP_LESS = SCANCODE_TO_KEYCODE(SCANCODE_KP_LESS),
    KEY_KP_GREATER = SCANCODE_TO_KEYCODE(SCANCODE_KP_GREATER),
    KEY_KP_AMPERSAND = SCANCODE_TO_KEYCODE(SCANCODE_KP_AMPERSAND),
    KEY_KP_DBLAMPERSAND = SCANCODE_TO_KEYCODE(SCANCODE_KP_DBLAMPERSAND),
    KEY_KP_VERTICALBAR = SCANCODE_TO_KEYCODE(SCANCODE_KP_VERTICALBAR),
    KEY_KP_DBLVERTICALBAR = SCANCODE_TO_KEYCODE(SCANCODE_KP_DBLVERTICALBAR),
    KEY_KP_COLON = SCANCODE_TO_KEYCODE(SCANCODE_KP_COLON),
    KEY_KP_HASH = SCANCODE_TO_KEYCODE(SCANCODE_KP_HASH),
    KEY_KP_SPACE = SCANCODE_TO_KEYCODE(SCANCODE_KP_SPACE),
    KEY_KP_AT = SCANCODE_TO_KEYCODE(SCANCODE_KP_AT),
    KEY_KP_EXCLAM = SCANCODE_TO_KEYCODE(SCANCODE_KP_EXCLAM),
    KEY_KP_MEMSTORE = SCANCODE_TO_KEYCODE(SCANCODE_KP_MEMSTORE),
    KEY_KP_MEMRECALL = SCANCODE_TO_KEYCODE(SCANCODE_KP_MEMRECALL),
    KEY_KP_MEMCLEAR = SCANCODE_TO_KEYCODE(SCANCODE_KP_MEMCLEAR),
    KEY_KP_MEMADD = SCANCODE_TO_KEYCODE(SCANCODE_KP_MEMADD),
    KEY_KP_MEMSUBTRACT = SCANCODE_TO_KEYCODE(SCANCODE_KP_MEMSUBTRACT),
    KEY_KP_MEMMULTIPLY = SCANCODE_TO_KEYCODE(SCANCODE_KP_MEMMULTIPLY),
    KEY_KP_MEMDIVIDE = SCANCODE_TO_KEYCODE(SCANCODE_KP_MEMDIVIDE),
    KEY_KP_PLUSMINUS = SCANCODE_TO_KEYCODE(SCANCODE_KP_PLUSMINUS),
    KEY_KP_CLEAR = SCANCODE_TO_KEYCODE(SCANCODE_KP_CLEAR),
    KEY_KP_CLEARENTRY = SCANCODE_TO_KEYCODE(SCANCODE_KP_CLEARENTRY),
    KEY_KP_BINARY = SCANCODE_TO_KEYCODE(SCANCODE_KP_BINARY),
    KEY_KP_OCTAL = SCANCODE_TO_KEYCODE(SCANCODE_KP_OCTAL),
    KEY_KP_DECIMAL = SCANCODE_TO_KEYCODE(SCANCODE_KP_DECIMAL),
    KEY_KP_HEXADECIMAL = SCANCODE_TO_KEYCODE(SCANCODE_KP_HEXADECIMAL),

    KEY_LCTRL = SCANCODE_TO_KEYCODE(SCANCODE_LCTRL),
    KEY_LSHIFT = SCANCODE_TO_KEYCODE(SCANCODE_LSHIFT),
    KEY_LALT = SCANCODE_TO_KEYCODE(SCANCODE_LALT),
    KEY_LGUI = SCANCODE_TO_KEYCODE(SCANCODE_LGUI),
    KEY_RCTRL = SCANCODE_TO_KEYCODE(SCANCODE_RCTRL),
    KEY_RSHIFT = SCANCODE_TO_KEYCODE(SCANCODE_RSHIFT),
    KEY_RALT = SCANCODE_TO_KEYCODE(SCANCODE_RALT),
    KEY_RGUI = SCANCODE_TO_KEYCODE(SCANCODE_RGUI),

    KEY_MODE = SCANCODE_TO_KEYCODE(SCANCODE_MODE),

    KEY_AUDIONEXT = SCANCODE_TO_KEYCODE(SCANCODE_AUDIONEXT),
    KEY_AUDIOPREV = SCANCODE_TO_KEYCODE(SCANCODE_AUDIOPREV),
    KEY_AUDIOSTOP = SCANCODE_TO_KEYCODE(SCANCODE_AUDIOSTOP),
    KEY_AUDIOPLAY = SCANCODE_TO_KEYCODE(SCANCODE_AUDIOPLAY),
    KEY_AUDIOMUTE = SCANCODE_TO_KEYCODE(SCANCODE_AUDIOMUTE),
    KEY_MEDIASELECT = SCANCODE_TO_KEYCODE(SCANCODE_MEDIASELECT),
    KEY_WWW = SCANCODE_TO_KEYCODE(SCANCODE_WWW),
    KEY_MAIL = SCANCODE_TO_KEYCODE(SCANCODE_MAIL),
    KEY_CALCULATOR = SCANCODE_TO_KEYCODE(SCANCODE_CALCULATOR),
    KEY_COMPUTER = SCANCODE_TO_KEYCODE(SCANCODE_COMPUTER),
    KEY_AC_SEARCH = SCANCODE_TO_KEYCODE(SCANCODE_AC_SEARCH),
    KEY_AC_HOME = SCANCODE_TO_KEYCODE(SCANCODE_AC_HOME),
    KEY_AC_BACK = SCANCODE_TO_KEYCODE(SCANCODE_AC_BACK),
    KEY_AC_FORWARD = SCANCODE_TO_KEYCODE(SCANCODE_AC_FORWARD),
    KEY_AC_STOP = SCANCODE_TO_KEYCODE(SCANCODE_AC_STOP),
    KEY_AC_REFRESH = SCANCODE_TO_KEYCODE(SCANCODE_AC_REFRESH),
    KEY_AC_BOOKMARKS = SCANCODE_TO_KEYCODE(SCANCODE_AC_BOOKMARKS),

    KEY_BRIGHTNESSDOWN = SCANCODE_TO_KEYCODE(SCANCODE_BRIGHTNESSDOWN),
    KEY_BRIGHTNESSUP = SCANCODE_TO_KEYCODE(SCANCODE_BRIGHTNESSUP),
    KEY_DISPLAYSWITCH = SCANCODE_TO_KEYCODE(SCANCODE_DISPLAYSWITCH),
    KEY_KBDILLUMTOGGLE = SCANCODE_TO_KEYCODE(SCANCODE_KBDILLUMTOGGLE),
    KEY_KBDILLUMDOWN = SCANCODE_TO_KEYCODE(SCANCODE_KBDILLUMDOWN),
    KEY_KBDILLUMUP = SCANCODE_TO_KEYCODE(SCANCODE_KBDILLUMUP),
    KEY_EJECT = SCANCODE_TO_KEYCODE(SCANCODE_EJECT),
    KEY_SLEEP = SCANCODE_TO_KEYCODE(SCANCODE_SLEEP),
    KEY_APP1 = SCANCODE_TO_KEYCODE(SCANCODE_APP1),
    KEY_APP2 = SCANCODE_TO_KEYCODE(SCANCODE_APP2),

    KEY_AUDIOREWIND = SCANCODE_TO_KEYCODE(SCANCODE_AUDIOREWIND),
    KEY_AUDIOFASTFORWARD = SCANCODE_TO_KEYCODE(SCANCODE_AUDIOFASTFORWARD),

    KEY_SOFTLEFT = SCANCODE_TO_KEYCODE(SCANCODE_SOFTLEFT),
    KEY_SOFTRIGHT = SCANCODE_TO_KEYCODE(SCANCODE_SOFTRIGHT),
    KEY_CALL = SCANCODE_TO_KEYCODE(SCANCODE_CALL),
    KEY_ENDCALL = SCANCODE_TO_KEYCODE(SCANCODE_ENDCALL)
};