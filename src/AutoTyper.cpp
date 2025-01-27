#include "AutoTyper.h"

#include "utils.h"
#include "config.h"

#include <fstream>
#include <unistd.h>
#include <cstring>

/*=== Global Public Process Function ===*/

bool AutoTyper::init() {
#ifdef __linux__
    display = XOpenDisplay(NULL);
    if (!display) {
        std::cerr << "[Error] Failed to open X display" << std::endl;
        return false;
    }
    initModifier();
#endif
    return true;
}

bool AutoTyper::type(const std::string& file_path) {
    std::cout << "[Prepare]" << std::endl;
    std::cout << "read in from \'" << file_path << "\'" << std::endl;
    std::ifstream file_to_type(file_path);
    if (!file_to_type.is_open()) {
        std::cerr << "[Error] Failed to open \'" << file_path << "\': ";
        perror("");
        return false;
    }
    std::cout << "success, pause for 3s..." << std::endl;
    USLEEP(3000 * 1000);

    std::cout << "[Typing] ..." << std::endl;
    std::string line;
    while (std::getline(file_to_type, line)) {
        if (isKeyPressed(ESCAPE_KEY)) break;
        processLine(line.c_str());
    }

    file_to_type.close();
    return true;
}

bool AutoTyper::clear() {
#ifdef __linux__
    XCloseDisplay(display);
#endif
    return true;
}

void AutoTyper::debug() {
#ifdef _WIN32
    simulateChar('<');
    simulateChar('\x7f');
    simulateChar('\n');
    simulateChar('\r');
    simulateChar('>');
#elif __linux__
    simulateChar('a');
#endif
}


/*=== Private Function ===*/

void AutoTyper::processLine(const char* str) {
    int len = strlen(str);
    static int base_leading_space = 0;
    int cur_leading_space = 0;

    if (fit_space_autofill) {
        cur_leading_space = countLeadingSpace(str);
        int delta = cur_leading_space - base_leading_space;
        if (delta >= 0) {
            for (int i = 0; i < delta; ++i) simulateChar(' ');
        } else {
            int step = fit_view_space_as_tab ? 4 : 1;
            for (int i = 0; i > delta; i -= step) simulateChar('\b');
        }
        base_leading_space = cur_leading_space;
    }

    for (int i = cur_leading_space; i < len; ++i) {
        simulateChar(str[i]);
        if (fit_brackets_autopair && (str[i] == '{' || str[i] == '[' || str[i] == '(')) {
            simulateChar('\x7f');
            if (isBlankTail(str + i + 1)) base_leading_space += 4;
        }
        USLEEP(type_interval);
    }
    simulateChar('\n');
}

void AutoTyper::simulateChar(char ch) {  
    KeyCode key, modifier;
    bool has_modifier = getKeyFromChar(ch, key, modifier);
    printf("ch = \'%c\', KeyCode = %u, press shift = %d\n", ch, key, has_modifier);

    if (has_modifier) simulateKey(modifier, true);
    simulateKey(key, true);
    simulateKey(key, false);
    if (has_modifier) simulateKey(modifier, false);

    flushDisplay();
}


/*=== PAL: OS Concerned ===*/
#ifdef _WIN32

inline bool AutoTyper::getKeyFromChar(char ch, KeyCode& key, KeyCode& modifier) {
    /* Exception:  
        char |  Symbol   | ascii | key
             |  Delete   |  0x7f | 0x8  <- [set \x7f to VK_DELETE] Need to Seperate from VK_BACK(0x8) 
        \b   | BackSpace |  0x8  | 0x8
        \r   |  Return   |  0xd  | 0xd
        \n   | Linefeed  |  0xa  | 0xd  <- Delight to see
    */ if (ch == '\x7f') {
        key = VK_DELETE;
        return false;
    }

    /* Common Process */
    SHORT keysym = VkKeyScanA(ch);
    if (keysym == -1) {
        std::cerr << "[Error] Character \'" << ch << "\' cannot be mapped to a virtual key.\n";
        return false;
    }
    key = keysym & 0xff;
    BYTE modifier_code = keysym >> 8;
    if (modifier_code & 0x1) modifier = VK_SHIFT; // TODO: Multi modifier
    if (modifier_code & 0x2) modifier = VK_CONTROL;
    if (modifier_code & 0x4) modifier = VK_MENU;
    return modifier_code & 0x1; // Only VK_SHIFT was allowed
}

inline void AutoTyper::flushDisplay() {
    SendInput(idx, input, sizeof(INPUT));
    // std::memset(input, 0, sizeof(INPUT) * idx);
    idx = 0;
}

inline void AutoTyper::simulateKey(KeyCode key, bool press) {
    input[idx].type = INPUT_KEYBOARD;
    input[idx].ki.wVk = key;
    input[idx].ki.dwFlags = press ? 0 : KEYEVENTF_KEYUP;
    idx++;
}

inline bool AutoTyper::isKeyPressed(KeySym keysym) {
    return GetAsyncKeyState(keysym) & 0x8000;
}

// Deprecated keybd_event
// inline void AutoTyper::keyPress(KeyCode key) {
//     keybd_event(key, 0, 0, 0);
// }

// inline void AutoTyper::keyRelease(KeyCode key) {
//     keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
// }

#elif __linux__

bool AutoTyper::initModifier() {
    int min_keycode, max_keycode;
    XDisplayKeycodes(display, &min_keycode, &max_keycode);

    int keysyms_per_keycode;
    KeySym* keymap = XGetKeyboardMapping(display, min_keycode, max_keycode - min_keycode + 1, &keysyms_per_keycode);

    for (int kc = min_keycode; kc <= max_keycode; ++kc) {
        for (int i = 1; i >= 0; --i) {  // front(0) over back(1)
            KeySym keysym = keymap[(kc - min_keycode) * keysyms_per_keycode + i];
            keysymNeedModifier[keysym] = i;
        } // only in the 0 column do not need modifier
    }

    XFree(keymap);
    return true;
}

inline bool AutoTyper::getKeyFromChar(char ch, KeyCode& key, KeyCode& modifier) {
    KeySym keysym;

    /* Exception
       ch | ascii |   Name   | Value
       \n |  0xa  | Linefeed | 0xff0a  <- [set \n to XK_Return 0xff0d] Do not fit the transform
       \r |  0xd  | Return   | 0xff0d 
    */ if (ch == '\n') {
        keysym = XK_Return;
    } else {
        keysym =  (0 <= ch && ch < 0x20) ? (0xff00) | ((int) ch) : ((int) ch);
    }
   
    /* Common Porcess */
    key = XKeysymToKeycode(display, keysym);
    auto it = keysymNeedModifier.find(keysym);
    if (it != keysymNeedModifier.end()) {
        modifier = XKeysymToKeycode(display, XK_Shift_L);
        return it->second;
    }
    return false;
}

inline void AutoTyper::flushDisplay() {
    XFlush(display);
}

inline void AutoTyper::simulateKey(KeyCode key, bool press) {
    if (key == 0) {
        std::cerr << "[Error] Invalid key code" << std::endl;
        return; // Invalid
    }
    XTestFakeKeyEvent(display, key, press, CurrentTime);
}

inline bool AutoTyper::isKeyPressed(KeySym keysym) {
    char keys[32];
    XQueryKeymap(display, keys);
    KeyCode keycode = XKeysymToKeycode(display, keysym);
    return keys[keycode / 8] & (1 << (keycode % 8));
}

#endif