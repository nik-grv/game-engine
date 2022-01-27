/** \file GLFWCodes.h
*/
#pragma once

/*! Adapted from glfw3.h
 */

#define NG_KEY_RELEASE                0
#define NG_KEY_PRESS                  1
#define NG_KEY_REPEAT                 2

#define NG_BUTTON_RELEASE                0
#define NG_BUTTON_PRESS                  1
#define NG_BUTTON_REPEAT                 2

 /* The unknown key */
#define KEY_UNKNOWN            -1

/* Printable keys */
#define NG_KEY_SPACE              32
#define NG_KEY_APOSTROPHE         39  /* ' */
#define NG_KEY_COMMA              44  /* , */
#define NG_KEY_MINUS              45  /* - */
#define NG_KEY_PERIOD             46  /* . */
#define NG_KEY_SLASH              47  /* / */
#define NG_KEY_0                  48
#define NG_KEY_1                  49
#define NG_KEY_2                  50
#define NG_KEY_3                  51
#define NG_KEY_4                  52
#define NG_KEY_5                  53
#define NG_KEY_6                  54
#define NG_KEY_7                  55
#define NG_KEY_8                  56
#define NG_KEY_9                  57
#define NG_KEY_SEMICOLON          59  /* ; */
#define NG_KEY_EQUAL              61  /* = */
#define NG_KEY_A                  65
#define NG_KEY_B                  66
#define NG_KEY_C                  67
#define NG_KEY_D                  68
#define NG_KEY_E                  69
#define NG_KEY_F                  70
#define NG_KEY_G                  71
#define NG_KEY_H                  72
#define NG_KEY_I                  73
#define NG_KEY_J                  74
#define NG_KEY_K                  75
#define NG_KEY_L                  76
#define NG_KEY_M                  77
#define NG_KEY_N                  78
#define NG_KEY_O                  79
#define NG_KEY_P                  80
#define NG_KEY_Q                  81
#define NG_KEY_R                  82
#define NG_KEY_S                  83
#define NG_KEY_T                  84
#define NG_KEY_U                  85
#define NG_KEY_V                  86
#define NG_KEY_W                  87
#define NG_KEY_X                  88
#define NG_KEY_Y                  89
#define NG_KEY_Z                  90
#define NG_KEY_LEFT_BRACKET       91  /* [ */
#define NG_KEY_BACKSLASH          92  /* \ */
#define NG_KEY_RIGHT_BRACKET      93  /* ] */
#define NG_KEY_GRAVE_ACCENT       96  /* ` */
#define NG_KEY_WORLD_1            161 /* non-US #1 */
#define NG_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define NG_KEY_ESCAPE             256
#define NG_KEY_ENTER              257
#define NG_KEY_TAB                258
#define NG_KEY_BACKSPACE          259
#define NG_KEY_INSERT             260
#define NG_KEY_DELETE             261
#define NG_KEY_RIGHT              262
#define NG_KEY_LEFT               263
#define NG_KEY_DOWN               264
#define NG_KEY_UP                 265
#define NG_KEY_PAGE_UP            266
#define NG_KEY_PAGE_DOWN          267
#define NG_KEY_HOME               268
#define NG_KEY_END                269
#define NG_KEY_CAPS_LOCK          280
#define NG_KEY_SCROLL_LOCK        281
#define NG_KEY_NUM_LOCK           282
#define NG_KEY_PRINT_SCREEN       283
#define NG_KEY_PAUSE              284
#define NG_KEY_F1                 290
#define NG_KEY_F2                 291
#define NG_KEY_F3                 292
#define NG_KEY_F4                 293
#define NG_KEY_F5                 294
#define NG_KEY_F6                 295
#define NG_KEY_F7                 296
#define NG_KEY_F8                 297
#define NG_KEY_F9                 298
#define NG_KEY_F10                299
#define NG_KEY_F11                300
#define NG_KEY_F12                301
#define NG_KEY_F13                302
#define NG_KEY_F14                303
#define NG_KEY_F15                304
#define NG_KEY_F16                305
#define NG_KEY_F17                306
#define NG_KEY_F18                307
#define NG_KEY_F19                308
#define NG_KEY_F20                309
#define NG_KEY_F21                310
#define NG_KEY_F22                311
#define NG_KEY_F23                312
#define NG_KEY_F24                313
#define NG_KEY_F25                314
#define NG_KEY_KP_0               320
#define NG_KEY_KP_1               321
#define NG_KEY_KP_2               322
#define NG_KEY_KP_3               323
#define NG_KEY_KP_4               324
#define NG_KEY_KP_5               325
#define NG_KEY_KP_6               326
#define NG_KEY_KP_7               327
#define NG_KEY_KP_8               328
#define NG_KEY_KP_9               329
#define NG_KEY_KP_DECIMAL         330
#define NG_KEY_KP_DIVIDE          331
#define NG_KEY_KP_MULTIPLY        332
#define NG_KEY_KP_SUBTRACT        333
#define NG_KEY_KP_ADD             334
#define NG_KEY_KP_ENTER           335
#define NG_KEY_KP_EQUAL           336
#define NG_KEY_LEFT_SHIFT         340
#define NG_KEY_LEFT_CONTROL       341
#define NG_KEY_LEFT_ALT           342
#define NG_KEY_LEFT_SUPER         343
#define NG_KEY_RIGHT_SHIFT        344
#define NG_KEY_RIGHT_CONTROL      345
#define NG_KEY_RIGHT_ALT          346
#define NG_KEY_RIGHT_SUPER        347
#define NG_KEY_MENU               348

#define NG_KEY_LAST               KEY_MENU

#define NG_MOD_SHIFT           0x0001
#define NG_MOD_CONTROL         0x0002
#define NG_MOD_ALT             0x0004
#define NG_MOD_SUPER           0x0008
#define NG_MOD_CAPS_LOCK       0x0010
#define NG_MOD_NUM_LOCK        0x0020

#define NG_MOUSE_BUTTON_1         0
#define NG_MOUSE_BUTTON_2         1
#define NG_MOUSE_BUTTON_3         2
#define NG_MOUSE_BUTTON_4         3
#define NG_MOUSE_BUTTON_5         4
#define NG_MOUSE_BUTTON_6         5
#define NG_MOUSE_BUTTON_7         6
#define NG_MOUSE_BUTTON_8         7
#define NG_MOUSE_BUTTON_LAST      NG_MOUSE_BUTTON_8 
#define NG_MOUSE_BUTTON_LEFT      NG_MOUSE_BUTTON_1 
#define NG_MOUSE_BUTTON_RIGHT     NG_MOUSE_BUTTON_2 
#define NG_MOUSE_BUTTON_MIDDLE    NG_MOUSE_BUTTON_3 