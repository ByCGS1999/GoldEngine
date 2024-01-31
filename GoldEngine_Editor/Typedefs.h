#pragma once

typedef enum {
    KEY_NULL = 0,        // Key: NULL, used for no key pressed
    // Alphanumeric keys
    KEY_APOSTROPHE = 39,       // Key: '
    KEY_COMMA = 44,       // Key: ,
    KEY_MINUS = 45,       // Key: -
    KEY_PERIOD = 46,       // Key: .
    KEY_SLASH = 47,       // Key: /
    KEY_ZERO = 48,       // Key: 0
    KEY_ONE = 49,       // Key: 1
    KEY_TWO = 50,       // Key: 2
    KEY_THREE = 51,       // Key: 3
    KEY_FOUR = 52,       // Key: 4
    KEY_FIVE = 53,       // Key: 5
    KEY_SIX = 54,       // Key: 6
    KEY_SEVEN = 55,       // Key: 7
    KEY_EIGHT = 56,       // Key: 8
    KEY_NINE = 57,       // Key: 9
    KEY_SEMICOLON = 59,       // Key: ;
    KEY_EQUAL = 61,       // Key: =
    KEY_A = 65,       // Key: A | a
    KEY_B = 66,       // Key: B | b
    KEY_C = 67,       // Key: C | c
    KEY_D = 68,       // Key: D | d
    KEY_E = 69,       // Key: E | e
    KEY_F = 70,       // Key: F | f
    KEY_G = 71,       // Key: G | g
    KEY_H = 72,       // Key: H | h
    KEY_I = 73,       // Key: I | i
    KEY_J = 74,       // Key: J | j
    KEY_K = 75,       // Key: K | k
    KEY_L = 76,       // Key: L | l
    KEY_M = 77,       // Key: M | m
    KEY_N = 78,       // Key: N | n
    KEY_O = 79,       // Key: O | o
    KEY_P = 80,       // Key: P | p
    KEY_Q = 81,       // Key: Q | q
    KEY_R = 82,       // Key: R | r
    KEY_S = 83,       // Key: S | s
    KEY_T = 84,       // Key: T | t
    KEY_U = 85,       // Key: U | u
    KEY_V = 86,       // Key: V | v
    KEY_W = 87,       // Key: W | w
    KEY_X = 88,       // Key: X | x
    KEY_Y = 89,       // Key: Y | y
    KEY_Z = 90,       // Key: Z | z
    KEY_LEFT_BRACKET = 91,       // Key: [
    KEY_BACKSLASH = 92,       // Key: '\'
    KEY_RIGHT_BRACKET = 93,       // Key: ]
    KEY_GRAVE = 96,       // Key: `
    // Function keys
    KEY_SPACE = 32,       // Key: Space
    KEY_ESCAPE = 256,      // Key: Esc
    KEY_ENTER = 257,      // Key: Enter
    KEY_TAB = 258,      // Key: Tab
    KEY_BACKSPACE = 259,      // Key: Backspace
    KEY_INSERT = 260,      // Key: Ins
    KEY_DELETE = 261,      // Key: Del
    KEY_RIGHT = 262,      // Key: Cursor right
    KEY_LEFT = 263,      // Key: Cursor left
    KEY_DOWN = 264,      // Key: Cursor down
    KEY_UP = 265,      // Key: Cursor up
    KEY_PAGE_UP = 266,      // Key: Page up
    KEY_PAGE_DOWN = 267,      // Key: Page down
    KEY_HOME = 268,      // Key: Home
    KEY_END = 269,      // Key: End
    KEY_CAPS_LOCK = 280,      // Key: Caps lock
    KEY_SCROLL_LOCK = 281,      // Key: Scroll down
    KEY_NUM_LOCK = 282,      // Key: Num lock
    KEY_PRINT_SCREEN = 283,      // Key: Print screen
    KEY_PAUSE = 284,      // Key: Pause
    KEY_F1 = 290,      // Key: F1
    KEY_F2 = 291,      // Key: F2
    KEY_F3 = 292,      // Key: F3
    KEY_F4 = 293,      // Key: F4
    KEY_F5 = 294,      // Key: F5
    KEY_F6 = 295,      // Key: F6
    KEY_F7 = 296,      // Key: F7
    KEY_F8 = 297,      // Key: F8
    KEY_F9 = 298,      // Key: F9
    KEY_F10 = 299,      // Key: F10
    KEY_F11 = 300,      // Key: F11
    KEY_F12 = 301,      // Key: F12
    KEY_LEFT_SHIFT = 340,      // Key: Shift left
    KEY_LEFT_CONTROL = 341,      // Key: Control left
    KEY_LEFT_ALT = 342,      // Key: Alt left
    KEY_LEFT_SUPER = 343,      // Key: Super left
    KEY_RIGHT_SHIFT = 344,      // Key: Shift right
    KEY_RIGHT_CONTROL = 345,      // Key: Control right
    KEY_RIGHT_ALT = 346,      // Key: Alt right
    KEY_RIGHT_SUPER = 347,      // Key: Super right
    KEY_KB_MENU = 348,      // Key: KB menu
    // Keypad keys
    KEY_KP_0 = 320,      // Key: Keypad 0
    KEY_KP_1 = 321,      // Key: Keypad 1
    KEY_KP_2 = 322,      // Key: Keypad 2
    KEY_KP_3 = 323,      // Key: Keypad 3
    KEY_KP_4 = 324,      // Key: Keypad 4
    KEY_KP_5 = 325,      // Key: Keypad 5
    KEY_KP_6 = 326,      // Key: Keypad 6
    KEY_KP_7 = 327,      // Key: Keypad 7
    KEY_KP_8 = 328,      // Key: Keypad 8
    KEY_KP_9 = 329,      // Key: Keypad 9
    KEY_KP_DECIMAL = 330,      // Key: Keypad .
    KEY_KP_DIVIDE = 331,      // Key: Keypad /
    KEY_KP_MULTIPLY = 332,      // Key: Keypad *
    KEY_KP_SUBTRACT = 333,      // Key: Keypad -
    KEY_KP_ADD = 334,      // Key: Keypad +
    KEY_KP_ENTER = 335,      // Key: Keypad Enter
    KEY_KP_EQUAL = 336,      // Key: Keypad =
    // Android key buttons
    KEY_BACK = 4,        // Key: Android back button
    KEY_MENU = 82,       // Key: Android menu button
    KEY_VOLUME_UP = 24,       // Key: Android volume up button
    KEY_VOLUME_DOWN = 25        // Key: Android volume down button
} KeyboardKey;

enum ImGuiWindowFlags_
{
    ImGuiWindowFlags_None = 0,
    ImGuiWindowFlags_NoTitleBar = 1 << 0,   // Disable title-bar
    ImGuiWindowFlags_NoResize = 1 << 1,   // Disable user resizing with the lower-right grip
    ImGuiWindowFlags_NoMove = 1 << 2,   // Disable user moving the window
    ImGuiWindowFlags_NoScrollbar = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programmatically)
    ImGuiWindowFlags_NoScrollWithMouse = 1 << 4,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
    ImGuiWindowFlags_NoCollapse = 1 << 5,   // Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node).
    ImGuiWindowFlags_AlwaysAutoResize = 1 << 6,   // Resize every window to its content every frame
    ImGuiWindowFlags_NoBackground = 1 << 7,   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
    ImGuiWindowFlags_NoSavedSettings = 1 << 8,   // Never load/save settings in .ini file
    ImGuiWindowFlags_NoMouseInputs = 1 << 9,   // Disable catching mouse, hovering test with pass through.
    ImGuiWindowFlags_MenuBar = 1 << 10,  // Has a menu-bar
    ImGuiWindowFlags_HorizontalScrollbar = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
    ImGuiWindowFlags_NoFocusOnAppearing = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
    ImGuiWindowFlags_NoBringToFrontOnFocus = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
    ImGuiWindowFlags_AlwaysVerticalScrollbar = 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
    ImGuiWindowFlags_AlwaysHorizontalScrollbar = 1 << 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
    ImGuiWindowFlags_NoNavInputs = 1 << 16,  // No gamepad/keyboard navigation within the window
    ImGuiWindowFlags_NoNavFocus = 1 << 17,  // No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
    ImGuiWindowFlags_UnsavedDocument = 1 << 18,  // Display a dot next to the title. When used in a tab/docking context, tab is selected when clicking the X + closure is not assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
    ImGuiWindowFlags_NoDocking = 1 << 19,  // Disable docking of this window
    ImGuiWindowFlags_NoNav = ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,
    ImGuiWindowFlags_NoDecoration = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse,
    ImGuiWindowFlags_NoInputs = ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,

    // [Internal]
    ImGuiWindowFlags_NavFlattened = 1 << 23,  // [BETA] On child window: allow gamepad/keyboard navigation to cross over parent border to this child or between sibling child windows.
    ImGuiWindowFlags_ChildWindow = 1 << 24,  // Don't use! For internal use by BeginChild()
    ImGuiWindowFlags_Tooltip = 1 << 25,  // Don't use! For internal use by BeginTooltip()
    ImGuiWindowFlags_Popup = 1 << 26,  // Don't use! For internal use by BeginPopup()
    ImGuiWindowFlags_Modal = 1 << 27,  // Don't use! For internal use by BeginPopupModal()
    ImGuiWindowFlags_ChildMenu = 1 << 28,  // Don't use! For internal use by BeginMenu()
    ImGuiWindowFlags_DockNodeHost = 1 << 29,  // Don't use! For internal use by Begin()/NewFrame()
};