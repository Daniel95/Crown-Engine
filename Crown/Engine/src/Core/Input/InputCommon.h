#pragma once

#include <cstdint>

// GLFW defines copied from glfw3.h to keep orbis building without including GLFW
// TODO: Something smarter

#define GLFW_TRUE                   1
#define GLFW_FALSE                  0
#define GLFW_RELEASE                0
#define GLFW_PRESS                  1
#define GLFW_REPEAT                 2

#define GLFW_HAT_CENTERED           0
#define GLFW_HAT_UP                 1
#define GLFW_HAT_RIGHT              2
#define GLFW_HAT_DOWN               4
#define GLFW_HAT_LEFT               8
#define GLFW_HAT_RIGHT_UP           (GLFW_HAT_RIGHT | GLFW_HAT_UP)
#define GLFW_HAT_RIGHT_DOWN         (GLFW_HAT_RIGHT | GLFW_HAT_DOWN)
#define GLFW_HAT_LEFT_UP            (GLFW_HAT_LEFT  | GLFW_HAT_UP)
#define GLFW_HAT_LEFT_DOWN          (GLFW_HAT_LEFT  | GLFW_HAT_DOWN)

#define GLFW_KEY_UNKNOWN            -1

/* Printable keys */
#define GLFW_KEY_SPACE              32
#define GLFW_KEY_APOSTROPHE         39  /* ' */
#define GLFW_KEY_COMMA              44  /* , */
#define GLFW_KEY_MINUS              45  /* - */
#define GLFW_KEY_PERIOD             46  /* . */
#define GLFW_KEY_SLASH              47  /* / */
#define GLFW_KEY_0                  48
#define GLFW_KEY_1                  49
#define GLFW_KEY_2                  50
#define GLFW_KEY_3                  51
#define GLFW_KEY_4                  52
#define GLFW_KEY_5                  53
#define GLFW_KEY_6                  54
#define GLFW_KEY_7                  55
#define GLFW_KEY_8                  56
#define GLFW_KEY_9                  57
#define GLFW_KEY_SEMICOLON          59  /* ; */
#define GLFW_KEY_EQUAL              61  /* = */
#define GLFW_KEY_A                  65
#define GLFW_KEY_B                  66
#define GLFW_KEY_C                  67
#define GLFW_KEY_D                  68
#define GLFW_KEY_E                  69
#define GLFW_KEY_F                  70
#define GLFW_KEY_G                  71
#define GLFW_KEY_H                  72
#define GLFW_KEY_I                  73
#define GLFW_KEY_J                  74
#define GLFW_KEY_K                  75
#define GLFW_KEY_L                  76
#define GLFW_KEY_M                  77
#define GLFW_KEY_N                  78
#define GLFW_KEY_O                  79
#define GLFW_KEY_P                  80
#define GLFW_KEY_Q                  81
#define GLFW_KEY_R                  82
#define GLFW_KEY_S                  83
#define GLFW_KEY_T                  84
#define GLFW_KEY_U                  85
#define GLFW_KEY_V                  86
#define GLFW_KEY_W                  87
#define GLFW_KEY_X                  88
#define GLFW_KEY_Y                  89
#define GLFW_KEY_Z                  90
#define GLFW_KEY_LEFT_BRACKET       91  /* [ */
#define GLFW_KEY_BACKSLASH          92  /* \ */
#define GLFW_KEY_RIGHT_BRACKET      93  /* ] */
#define GLFW_KEY_GRAVE_ACCENT       96  /* ` */
#define GLFW_KEY_WORLD_1            161 /* non-US #1 */
#define GLFW_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define GLFW_KEY_ESCAPE             256
#define GLFW_KEY_ENTER              257
#define GLFW_KEY_TAB                258
#define GLFW_KEY_BACKSPACE          259
#define GLFW_KEY_INSERT             260
#define GLFW_KEY_DELETE             261
#define GLFW_KEY_RIGHT              262
#define GLFW_KEY_LEFT               263
#define GLFW_KEY_DOWN               264
#define GLFW_KEY_UP                 265
#define GLFW_KEY_PAGE_UP            266
#define GLFW_KEY_PAGE_DOWN          267
#define GLFW_KEY_HOME               268
#define GLFW_KEY_END                269
#define GLFW_KEY_CAPS_LOCK          280
#define GLFW_KEY_SCROLL_LOCK        281
#define GLFW_KEY_NUM_LOCK           282
#define GLFW_KEY_PRINT_SCREEN       283
#define GLFW_KEY_PAUSE              284
#define GLFW_KEY_F1                 290
#define GLFW_KEY_F2                 291
#define GLFW_KEY_F3                 292
#define GLFW_KEY_F4                 293
#define GLFW_KEY_F5                 294
#define GLFW_KEY_F6                 295
#define GLFW_KEY_F7                 296
#define GLFW_KEY_F8                 297
#define GLFW_KEY_F9                 298
#define GLFW_KEY_F10                299
#define GLFW_KEY_F11                300
#define GLFW_KEY_F12                301
#define GLFW_KEY_F13                302
#define GLFW_KEY_F14                303
#define GLFW_KEY_F15                304
#define GLFW_KEY_F16                305
#define GLFW_KEY_F17                306
#define GLFW_KEY_F18                307
#define GLFW_KEY_F19                308
#define GLFW_KEY_F20                309
#define GLFW_KEY_F21                310
#define GLFW_KEY_F22                311
#define GLFW_KEY_F23                312
#define GLFW_KEY_F24                313
#define GLFW_KEY_F25                314
#define GLFW_KEY_KP_0               320
#define GLFW_KEY_KP_1               321
#define GLFW_KEY_KP_2               322
#define GLFW_KEY_KP_3               323
#define GLFW_KEY_KP_4               324
#define GLFW_KEY_KP_5               325
#define GLFW_KEY_KP_6               326
#define GLFW_KEY_KP_7               327
#define GLFW_KEY_KP_8               328
#define GLFW_KEY_KP_9               329
#define GLFW_KEY_KP_DECIMAL         330
#define GLFW_KEY_KP_DIVIDE          331
#define GLFW_KEY_KP_MULTIPLY        332
#define GLFW_KEY_KP_SUBTRACT        333
#define GLFW_KEY_KP_ADD             334
#define GLFW_KEY_KP_ENTER           335
#define GLFW_KEY_KP_EQUAL           336
#define GLFW_KEY_LEFT_SHIFT         340
#define GLFW_KEY_LEFT_CONTROL       341
#define GLFW_KEY_LEFT_ALT           342
#define GLFW_KEY_LEFT_SUPER         343
#define GLFW_KEY_RIGHT_SHIFT        344
#define GLFW_KEY_RIGHT_CONTROL      345
#define GLFW_KEY_RIGHT_ALT          346
#define GLFW_KEY_RIGHT_SUPER        347
#define GLFW_KEY_MENU               348

#define GLFW_KEY_LAST               GLFW_KEY_MENU

#define GLFW_MOD_SHIFT           0x0001
#define GLFW_MOD_CONTROL         0x0002
#define GLFW_MOD_ALT             0x0004
#define GLFW_MOD_SUPER           0x0008
#define GLFW_MOD_CAPS_LOCK       0x0010
#define GLFW_MOD_NUM_LOCK        0x0020

#define GLFW_MOUSE_BUTTON_1         0
#define GLFW_MOUSE_BUTTON_2         1
#define GLFW_MOUSE_BUTTON_3         2
#define GLFW_MOUSE_BUTTON_4         3
#define GLFW_MOUSE_BUTTON_5         4
#define GLFW_MOUSE_BUTTON_6         5
#define GLFW_MOUSE_BUTTON_7         6
#define GLFW_MOUSE_BUTTON_8         7
#define GLFW_MOUSE_BUTTON_LAST      GLFW_MOUSE_BUTTON_8
#define GLFW_MOUSE_BUTTON_LEFT      GLFW_MOUSE_BUTTON_1
#define GLFW_MOUSE_BUTTON_RIGHT     GLFW_MOUSE_BUTTON_2
#define GLFW_MOUSE_BUTTON_MIDDLE    GLFW_MOUSE_BUTTON_3

#define GLFW_JOYSTICK_1             0
#define GLFW_JOYSTICK_2             1
#define GLFW_JOYSTICK_3             2
#define GLFW_JOYSTICK_4             3
#define GLFW_JOYSTICK_5             4
#define GLFW_JOYSTICK_6             5
#define GLFW_JOYSTICK_7             6
#define GLFW_JOYSTICK_8             7
#define GLFW_JOYSTICK_9             8
#define GLFW_JOYSTICK_10            9
#define GLFW_JOYSTICK_11            10
#define GLFW_JOYSTICK_12            11
#define GLFW_JOYSTICK_13            12
#define GLFW_JOYSTICK_14            13
#define GLFW_JOYSTICK_15            14
#define GLFW_JOYSTICK_16            15
#define GLFW_JOYSTICK_LAST          GLFW_JOYSTICK_16

#define GLFW_GAMEPAD_BUTTON_A               0
#define GLFW_GAMEPAD_BUTTON_B               1
#define GLFW_GAMEPAD_BUTTON_X               2
#define GLFW_GAMEPAD_BUTTON_Y               3
#define GLFW_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define GLFW_GAMEPAD_BUTTON_BACK            6
#define GLFW_GAMEPAD_BUTTON_START           7
#define GLFW_GAMEPAD_BUTTON_GUIDE           8
#define GLFW_GAMEPAD_BUTTON_LEFT_THUMB      9
#define GLFW_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define GLFW_GAMEPAD_BUTTON_DPAD_UP         11
#define GLFW_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define GLFW_GAMEPAD_BUTTON_DPAD_DOWN       13
#define GLFW_GAMEPAD_BUTTON_DPAD_LEFT       14
#define GLFW_GAMEPAD_BUTTON_LAST            GLFW_GAMEPAD_BUTTON_DPAD_LEFT

#define GLFW_GAMEPAD_BUTTON_CROSS       GLFW_GAMEPAD_BUTTON_A
#define GLFW_GAMEPAD_BUTTON_CIRCLE      GLFW_GAMEPAD_BUTTON_B
#define GLFW_GAMEPAD_BUTTON_SQUARE      GLFW_GAMEPAD_BUTTON_X
#define GLFW_GAMEPAD_BUTTON_TRIANGLE    GLFW_GAMEPAD_BUTTON_Y

#define GLFW_GAMEPAD_AXIS_LEFT_X        0
#define GLFW_GAMEPAD_AXIS_LEFT_Y        1
#define GLFW_GAMEPAD_AXIS_RIGHT_X       2
#define GLFW_GAMEPAD_AXIS_RIGHT_Y       3
#define GLFW_GAMEPAD_AXIS_LEFT_TRIGGER  4
#define GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define GLFW_GAMEPAD_AXIS_LAST          GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER

namespace Crown {

    enum class ButtonState
    {
        Up,
        Pressed,
        Down,
        Released
    };
	
	enum class Key
	{
        // The unknown key
        Unknown = GLFW_KEY_UNKNOWN,                        // -1
        // Printable keys  
        Space = GLFW_KEY_SPACE,                            // 32
        Apostrophe = GLFW_KEY_APOSTROPHE,                  // 39 
        Comma = GLFW_KEY_COMMA,                            // 44 
        Minus = GLFW_KEY_MINUS,                            // 45
        Period = GLFW_KEY_PERIOD,                          // 46
        Slash = GLFW_KEY_SLASH,                            // 47
        Num0 = GLFW_KEY_0,                                 // 48
        Num1 = GLFW_KEY_1,                                 // 49
        Num2 = GLFW_KEY_2,                                 // 50
        Num3 = GLFW_KEY_3,                                 // 51
        Num4 = GLFW_KEY_4,                                 // 52
        Num5 = GLFW_KEY_5,                                 // 53
        Num6 = GLFW_KEY_6,                                 // 54
        Num7 = GLFW_KEY_7,                                 // 55
        Num8 = GLFW_KEY_8,                                 // 56
        Num9 = GLFW_KEY_9,                                 // 57
        Semicolon = GLFW_KEY_SEMICOLON,                    // 59
        Equal = GLFW_KEY_EQUAL,                            // 61
        A = GLFW_KEY_A,                                    // 65
        B = GLFW_KEY_B,                                    // 66
        C = GLFW_KEY_C,                                    // 67
        D = GLFW_KEY_D,                                    // 68
        E = GLFW_KEY_E,                                    // 69
        F = GLFW_KEY_F,                                    // 70
        G = GLFW_KEY_G,                                    // 71
        H = GLFW_KEY_H,                                    // 72
        I = GLFW_KEY_I,                                    // 73
        J = GLFW_KEY_J,                                    // 74
        K = GLFW_KEY_K,                                    // 75
        L = GLFW_KEY_L,                                    // 76
        M = GLFW_KEY_M,                                    // 77
        N = GLFW_KEY_N,                                    // 78
        O = GLFW_KEY_O,                                    // 79
        P = GLFW_KEY_P,                                    // 80
        Q = GLFW_KEY_Q,                                    // 81
        R = GLFW_KEY_R,                                    // 82
        S = GLFW_KEY_S,                                    // 83
        T = GLFW_KEY_T,                                    // 84
        U = GLFW_KEY_U,                                    // 85
        V = GLFW_KEY_V,                                    // 86
        W = GLFW_KEY_W,                                    // 87
        X = GLFW_KEY_X,                                    // 88
        Y = GLFW_KEY_Y,                                    // 89
        Z = GLFW_KEY_Z,                                    // 90
        LeftBracket = GLFW_KEY_LEFT_BRACKET,               // 91  
        Backslash = GLFW_KEY_BACKSLASH,                    // 92 
        RightBracket = GLFW_KEY_RIGHT_BRACKET,             // 93 
        GraveAccent = GLFW_KEY_GRAVE_ACCENT,               // 96 
        World_1 = GLFW_KEY_WORLD_1,                        // 161
        World_2 = GLFW_KEY_WORLD_2,                        // 162
        // Function keys
        Escape = GLFW_KEY_ESCAPE,                          // 256
        Enter = GLFW_KEY_ENTER,                            // 257
        Tab = GLFW_KEY_TAB,                                // 258
        Backspace = GLFW_KEY_BACKSPACE,                    // 259
        Insert = GLFW_KEY_INSERT,                          // 260
        Delete = GLFW_KEY_DELETE,                          // 261
        Right = GLFW_KEY_RIGHT,                            // 262
        Left = GLFW_KEY_LEFT,                              // 263
        Down = GLFW_KEY_DOWN,                              // 264
        Up = GLFW_KEY_UP,                                  // 265
        PageUp = GLFW_KEY_PAGE_UP,                         // 266
        PageDown = GLFW_KEY_PAGE_DOWN,                     // 267
        Home = GLFW_KEY_HOME,                              // 268
        End = GLFW_KEY_END,                                // 269
        CapsLock = GLFW_KEY_CAPS_LOCK,                     // 280
        ScrollLock = GLFW_KEY_SCROLL_LOCK,                 // 281
        NumLock = GLFW_KEY_NUM_LOCK,                       // 282
        PrintScreen = GLFW_KEY_PRINT_SCREEN,               // 283
        Pause = GLFW_KEY_PAUSE,                            // 284
        F1 = GLFW_KEY_F1,                                  // 290
        F2 = GLFW_KEY_F2,                                  // 291
        F3 = GLFW_KEY_F3,                                  // 292
        F4 = GLFW_KEY_F4,                                  // 293
        F5 = GLFW_KEY_F5,                                  // 294
        F6 = GLFW_KEY_F6,                                  // 295
        F7 = GLFW_KEY_F7,                                  // 296
        F8 = GLFW_KEY_F8,                                  // 297
        F9 = GLFW_KEY_F9,                                  // 298
        F10 = GLFW_KEY_F10,                                // 299
        F11 = GLFW_KEY_F11,                                // 300
        F12 = GLFW_KEY_F12,                                // 301
        F13 = GLFW_KEY_F13,                                // 302
        F14 = GLFW_KEY_F14,                                // 303
        F15 = GLFW_KEY_F15,                                // 304
        F16 = GLFW_KEY_F16,                                // 305
        F17 = GLFW_KEY_F17,                                // 306
        F18 = GLFW_KEY_F18,                                // 307
        F19 = GLFW_KEY_F19,                                // 308
        F20 = GLFW_KEY_F20,                                // 309
        F21 = GLFW_KEY_F21,                                // 310
        F22 = GLFW_KEY_F22,                                // 311
        F23 = GLFW_KEY_F23,                                // 312
        F24 = GLFW_KEY_F24,                                // 313
        F25 = GLFW_KEY_F25,                                // 314
        Kp0 = GLFW_KEY_KP_0,                               // 320
        Kp1 = GLFW_KEY_KP_1,                               // 321
        Kp2 = GLFW_KEY_KP_2,                               // 322
        Kp3 = GLFW_KEY_KP_3,                               // 323
        Kp4 = GLFW_KEY_KP_4,                               // 324
        Kp5 = GLFW_KEY_KP_5,                               // 325
        Kp6 = GLFW_KEY_KP_6,                               // 326
        Kp7 = GLFW_KEY_KP_7,                               // 327
        Kp8 = GLFW_KEY_KP_8,                               // 328
        Kp9 = GLFW_KEY_KP_9,                               // 329
        KpDecimal = GLFW_KEY_KP_DECIMAL,                   // 330
        KpDivide = GLFW_KEY_KP_DIVIDE,                     // 331
        KpMultiply = GLFW_KEY_KP_MULTIPLY,                 // 332
        KpSubtract = GLFW_KEY_KP_SUBTRACT,                 // 333
        KpAdd = GLFW_KEY_KP_ADD,                           // 334
        KpEnter = GLFW_KEY_KP_ENTER,                       // 335
        KpEqual = GLFW_KEY_KP_EQUAL,                       // 336
        LeftShift = GLFW_KEY_LEFT_SHIFT,                   // 340
        LeftControl = GLFW_KEY_LEFT_CONTROL,               // 341
        LeftAlt = GLFW_KEY_LEFT_ALT,                       // 342
        LeftSuper = GLFW_KEY_LEFT_SUPER,                   // 343
        RightShift = GLFW_KEY_RIGHT_SHIFT,                 // 344
        RightControl = GLFW_KEY_RIGHT_CONTROL,             // 345
        RightAlt = GLFW_KEY_RIGHT_ALT,                     // 346
        RightSuper = GLFW_KEY_RIGHT_SUPER,                 // 347
        Menu = GLFW_KEY_MENU,                              // 348
	};


	enum class MouseButton
	{
		Left = GLFW_MOUSE_BUTTON_LEFT,                     // 0
		Right = GLFW_MOUSE_BUTTON_RIGHT,                   // 1
		Middle = GLFW_MOUSE_BUTTON_MIDDLE,                 // 2
        Button4 = GLFW_MOUSE_BUTTON_4,                     // 3
        Button5 = GLFW_MOUSE_BUTTON_5,                     // 4
        Button6 = GLFW_MOUSE_BUTTON_6,                     // 5
        Button7 = GLFW_MOUSE_BUTTON_7,                     // 6
        Button8 = GLFW_MOUSE_BUTTON_8,                     // 7
	};
	
	enum class Button
	{
		South = GLFW_GAMEPAD_BUTTON_A,						// 0
		East = GLFW_GAMEPAD_BUTTON_B,						// 1
		West = GLFW_GAMEPAD_BUTTON_X,						// 2
		Nort = GLFW_GAMEPAD_BUTTON_Y,						// 3
		LeftShoulder = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,		// 4
		RightShoulder = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,	// 5
		Back = GLFW_GAMEPAD_BUTTON_BACK,					// 6
		Start = GLFW_GAMEPAD_BUTTON_START,					// 7
		Home = GLFW_GAMEPAD_BUTTON_GUIDE,					// 8
		LeftStick = GLFW_GAMEPAD_BUTTON_LEFT_THUMB,			// 9
		RightThumb = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,		// 10
		Up = GLFW_GAMEPAD_BUTTON_DPAD_UP,					// 11
		Right = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,				// 12
		Down = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,				// 13
		Left = GLFW_GAMEPAD_BUTTON_DPAD_LEFT,				// 14
	};
	
	using ButtonMask = uint32_t;

	inline ButtonMask Bits(Button button)
	{
		return 1 << static_cast<ButtonMask>(button);
	}

	enum class ButtonSet
	{
		None, Any, All
	};
	
	enum class Axis
	{
		LX = GLFW_GAMEPAD_AXIS_LEFT_X,						// 0
		LY = GLFW_GAMEPAD_AXIS_LEFT_Y,						// 1
		RX = GLFW_GAMEPAD_AXIS_RIGHT_X,						// 2
		RY = GLFW_GAMEPAD_AXIS_RIGHT_Y,						// 3
		TL = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,				// 4
		TR = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,				// 5
	};

    const int JoystickCount = GLFW_JOYSTICK_LAST + 1;
    const int KeyCount = GLFW_KEY_LAST + 1;
    const int MouseButtonCount = GLFW_MOUSE_BUTTON_LAST + 1;
	const int ButtonCount = GLFW_GAMEPAD_BUTTON_LAST + 1;
	const int AxisCount = GLFW_GAMEPAD_AXIS_LAST + 1;
}
