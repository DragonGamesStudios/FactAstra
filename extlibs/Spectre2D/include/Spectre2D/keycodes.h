#pragma once

#define SPECTRE_MOD_SHIFT 0x0001
#define SPECTRE_MOD_CONTROL 0x0002
#define SPECTRE_MOD_ALT 0x0004
#define SPECTRE_MOD_SUPER 0x0008
#define SPECTRE_MOD_CAPS_LOCK 0x0010
#define SPECTRE_MOD_NUM_LOCK 0x0020

/* The unknown key */
#define SPECTRE_KEY_UNKNOWN            -1

/* Printable keys */

#define SPECTRE_KEY_SPACE              32
#define SPECTRE_KEY_APOSTROPHE         39  /* ' */
#define SPECTRE_KEY_COMMA              44  /* , */
#define SPECTRE_KEY_MINUS              45  /* - */
#define SPECTRE_KEY_PERIOD             46  /* . */
#define SPECTRE_KEY_SLASH              47  /* / */
#define SPECTRE_KEY_0                  48
#define SPECTRE_KEY_1                  49
#define SPECTRE_KEY_2                  50
#define SPECTRE_KEY_3                  51
#define SPECTRE_KEY_4                  52
#define SPECTRE_KEY_5                  53
#define SPECTRE_KEY_6                  54
#define SPECTRE_KEY_7                  55
#define SPECTRE_KEY_8                  56
#define SPECTRE_KEY_9                  57
#define SPECTRE_KEY_SEMICOLON          59  /* ; */
#define SPECTRE_KEY_EQUAL              61  /* = */
#define SPECTRE_KEY_A                  65
#define SPECTRE_KEY_B                  66
#define SPECTRE_KEY_C                  67
#define SPECTRE_KEY_D                  68
#define SPECTRE_KEY_E                  69
#define SPECTRE_KEY_F                  70
#define SPECTRE_KEY_G                  71
#define SPECTRE_KEY_H                  72
#define SPECTRE_KEY_I                  73
#define SPECTRE_KEY_J                  74
#define SPECTRE_KEY_K                  75
#define SPECTRE_KEY_L                  76
#define SPECTRE_KEY_M                  77
#define SPECTRE_KEY_N                  78
#define SPECTRE_KEY_O                  79
#define SPECTRE_KEY_P                  80
#define SPECTRE_KEY_Q                  81
#define SPECTRE_KEY_R                  82
#define SPECTRE_KEY_S                  83
#define SPECTRE_KEY_T                  84
#define SPECTRE_KEY_U                  85
#define SPECTRE_KEY_V                  86
#define SPECTRE_KEY_W                  87
#define SPECTRE_KEY_X                  88
#define SPECTRE_KEY_Y                  89
#define SPECTRE_KEY_Z                  90
#define SPECTRE_KEY_LEFT_BRACKET       91  /* [ */
#define SPECTRE_KEY_BACKSLASH          92  /* \ */
#define SPECTRE_KEY_RIGHT_BRACKET      93  /* ] */
#define SPECTRE_KEY_GRAVE_ACCENT       96  /* ` */
#define SPECTRE_KEY_WORLD_1            161 /* non-US #1 */
#define SPECTRE_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */

#define SPECTRE_KEY_ESCAPE             256
#define SPECTRE_KEY_ENTER              257
#define SPECTRE_KEY_TAB                258
#define SPECTRE_KEY_BACKSPACE          259
#define SPECTRE_KEY_INSERT             260
#define SPECTRE_KEY_DELETE             261
#define SPECTRE_KEY_RIGHT              262
#define SPECTRE_KEY_LEFT               263
#define SPECTRE_KEY_DOWN               264
#define SPECTRE_KEY_UP                 265
#define SPECTRE_KEY_PAGE_UP            266
#define SPECTRE_KEY_PAGE_DOWN          267
#define SPECTRE_KEY_HOME               268
#define SPECTRE_KEY_END                269
#define SPECTRE_KEY_CAPS_LOCK          280
#define SPECTRE_KEY_SCROLL_LOCK        281
#define SPECTRE_KEY_NUM_LOCK           282
#define SPECTRE_KEY_PRINT_SCREEN       283
#define SPECTRE_KEY_PAUSE              284
#define SPECTRE_KEY_F1                 290
#define SPECTRE_KEY_F2                 291
#define SPECTRE_KEY_F3                 292
#define SPECTRE_KEY_F4                 293
#define SPECTRE_KEY_F5                 294
#define SPECTRE_KEY_F6                 295
#define SPECTRE_KEY_F7                 296
#define SPECTRE_KEY_F8                 297
#define SPECTRE_KEY_F9                 298
#define SPECTRE_KEY_F10                299
#define SPECTRE_KEY_F11                300
#define SPECTRE_KEY_F12                301
#define SPECTRE_KEY_F13                302
#define SPECTRE_KEY_F14                303
#define SPECTRE_KEY_F15                304
#define SPECTRE_KEY_F16                305
#define SPECTRE_KEY_F17                306
#define SPECTRE_KEY_F18                307
#define SPECTRE_KEY_F19                308
#define SPECTRE_KEY_F20                309
#define SPECTRE_KEY_F21                310
#define SPECTRE_KEY_F22                311
#define SPECTRE_KEY_F23                312
#define SPECTRE_KEY_F24                313
#define SPECTRE_KEY_F25                314
#define SPECTRE_KEY_KP_0               320
#define SPECTRE_KEY_KP_1               321
#define SPECTRE_KEY_KP_2               322
#define SPECTRE_KEY_KP_3               323
#define SPECTRE_KEY_KP_4               324
#define SPECTRE_KEY_KP_5               325
#define SPECTRE_KEY_KP_6               326
#define SPECTRE_KEY_KP_7               327
#define SPECTRE_KEY_KP_8               328
#define SPECTRE_KEY_KP_9               329
#define SPECTRE_KEY_KP_DECIMAL         330
#define SPECTRE_KEY_KP_DIVIDE          331
#define SPECTRE_KEY_KP_MULTIPLY        332
#define SPECTRE_KEY_KP_SUBTRACT        333
#define SPECTRE_KEY_KP_ADD             334
#define SPECTRE_KEY_KP_ENTER           335
#define SPECTRE_KEY_KP_EQUAL           336
#define SPECTRE_KEY_LEFT_SHIFT         340
#define SPECTRE_KEY_LEFT_CONTROL       341
#define SPECTRE_KEY_LEFT_ALT           342
#define SPECTRE_KEY_LEFT_SUPER         343
#define SPECTRE_KEY_RIGHT_SHIFT        344
#define SPECTRE_KEY_RIGHT_CONTROL      345
#define SPECTRE_KEY_RIGHT_ALT          346
#define SPECTRE_KEY_RIGHT_SUPER        347
#define SPECTRE_KEY_MENU               348