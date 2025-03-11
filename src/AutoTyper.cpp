#include "AutoTyper.h"

#include "utils.h"
#include "config.h"

#include <fstream>
#include <unistd.h>
#include <cstring>
#include <stack>

/*=== Global Public Process Function ===*/

bool AutoTyper::init() {
    if (type_hide_window) hideWindow();

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
    printf("[Debug]\n");
    USLEEP(2000 * 1000);

#ifdef _WIN32
    checkIME();
    simulateChar('a');
    simulateChar('b');
    simulateChar('c');
    simulateChar('\x7f');
    simulateChar('\n');

    char ch = 0x81;
    unsigned char uch = 0x81;
    printf("%d\n", ch == uch);
    switch ((unsigned char)ch) {
        case 0x81: printf("match\n"); break;
        default:break;
    }
#elif __linux__
    simulateChar('a'); simulateChar('b'); simulateChar('c');
    simulateChar(CHAR_LEFT);
    simulateChar('\x7f');
    simulateChar('\b');
#endif
}


/*=== Private Function ===*/
#define USE_METHOD_2
void AutoTyper::processLine(const char* str) {
    int len = strlen(str);
    // space_autofill
    static int base_leading_space = 0;
    int cur_leading_space = 0;
    // brace_nextline_pair
#ifdef USE_METHOD_1
    static std::stack<bool> open_brace;
#endif

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
    
    #ifdef USE_METHOD_1
        if (fit_brace_nextline_pair) {
            if (str[i] == '{') {
                open_brace.push(i == len - 1 ? 1 : 0); // only last '{' would be take into count
                if (isBlankTail(str + i + 1)) base_leading_space += 4; // '{' & '{ ' would both arouse '\t' nextline 
            } else if (str[i] == '}' && !open_brace.empty()) {
                bool need_modify = open_brace.top(); open_brace.pop();
                if (need_modify) {
                    if (isBlankHead(str, str + i - 1)) {
                        int step = fit_view_space_as_tab ? 4 : 1;
                        for (int k = 0; k <= base_leading_space; k += step) simulateChar('\b'); // backspace to last line
                        simulateChar(CHAR_DOWN); // move cursor down, reuse the '}' generated before <=> opened brace decrease
                        continue;
                    } else { // WARN: would change input content, but will not effect syntax and compile
                        simulateChar(CHAR_DOWN); // directly move cursor down
                        continue;
                    }
                }
            }
        }
    #endif
        
        simulateChar(str[i]);
        
        if (fit_brackets_autopair) {
            if ((str[i] == '{' && !fit_brace_nextline_pair) || str[i] == '[' || str[i] == '(') {
                simulateChar('\x7f');
                if (isBlankTail(str + i + 1)) base_leading_space += 4;
            }
        }

    #ifdef USE_METHOD_2
        if (fit_brace_nextline_pair) { // WARN: would change input content slightly -> one more space behind '{' with blank tail
            if (str[i] == '{' && isBlankTail(str + i + 1)) {
                simulateChar(' '); // to destroy the nextline pair
                base_leading_space += 4;
            }
        }
    #endif

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
    */
    /* Custom map: ch >= 127 */
    switch ((unsigned char)ch) {
        case '\x7f'     : key = VK_DELETE;  return false;
        case CHAR_LEFT  : key = VK_LEFT;    return false;
        case CHAR_RIGHT : key = VK_RIGHT;   return false;
        case CHAR_UP    : key = VK_UP;      return false;
        case CHAR_DOWN  : key = VK_DOWN;    return false;
        default: break;
    }

    /* Common Process */
    SHORT keysym = VkKeyScanA(ch);
    if (keysym == -1) {
        std::cerr << "[Error] Character \'" << ch << "\' cannot be mapped to a virtual key.\n";
        key = 0;
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

void AutoTyper::checkIME() {
    HWND hwnd = GetForegroundWindow();
    if (!hwnd) {
        std::cerr << "[Error] Cannot get foreground window!" << std::endl;
        return;
    }

    HIMC hIMC = ImmGetContext(hwnd);
    if (!hIMC) {
        std::cerr << "[Error] Cannot get IMM Context!" << std::endl;
        return;
    }

    DWORD conversion, sentence;
    if (ImmGetConversionStatus(hIMC, &conversion, &sentence)) {
        bool isChinese = conversion & IME_CMODE_NATIVE;
        if (isChinese) {
            std::cout << "Detect Chinese IME, Switch to English" << std::endl;
            ImmSetConversionStatus(hIMC, conversion & ~IME_CMODE_NATIVE, sentence);
        }
    }

    ImmReleaseContext(hwnd, hIMC);
}

void AutoTyper::hideWindow() {
    HWND hwnd = GetConsoleWindow();
    if (hwnd) ShowWindow(hwnd, SW_HIDE);
}

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
    */ /* Custom Mapping */
    switch ((unsigned char)ch) {
        case '\n'       : keysym = XK_Return;  break;
        case '\x7f'     : keysym = XK_Delete;  break;
        case CHAR_LEFT  : keysym = XK_Left;    break;
        case CHAR_RIGHT : keysym = XK_Right;   break;
        case CHAR_UP    : keysym = XK_Up;      break;
        case CHAR_DOWN  : keysym = XK_Down;    break;
        default: 
            keysym = (0 <= ch && ch < 0x20) ? (0xff00) | ((int) ch) : ((int) ch);
            break;
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

void AutoTyper::checkIME() {
    
}

void AutoTyper::hideWindow() {
    
}

#endif