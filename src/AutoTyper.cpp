#include "AutoTyper.h"

#include <fstream>

/*=== Global Public Process Function ===*/

bool AutoTyper::init() {
#ifdef __linux__
    display = XOpenDisplay(NULL);
    if (!display) {
        cerr << "[Error] Failed to open X display" << endl;
        return false;
    }
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
    SLEEP(3000 * 1000);

    std::cout << "[Typing] ..." << std::endl;
    std::string line;
    while (std::getline(file_to_type, line)) {
        if(isKeyPressed(VK_ESCAPE)) break;
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
    int key_num[130]={0,0,0,0,0,0,0,0,0,0,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,1049,11222,1051,1052,1053,1055,10222,11057,1048,1056,1107,188,109,190,111,48,49,50,51,52,53,54,55,56,57,1186,186,11188,187,1190,1191,1050,1065,1066,1067,1068,1069,1070,1071,1072,1073,1074,1075,1076,1077,1078,1079,1080,1081,1082,1083,1084,1085,1086,1087,1088,1089,1090,10219,220,221,1054,1189,192,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,0,1220,1221,1192};

    SLEEP(1000 * 1000);
    std::cout << key_num['<'] << std::endl;
    simulateChar('<');
    
    SHORT result = VkKeyScanA('\b');
    BYTE virtualKey = LOBYTE(result);  // 低字节是虚拟键码
    BYTE shiftState = HIBYTE(result);  // 高字节是 Shift 状态

    std::cout << "Character: '\\b'" << std::endl;
    std::cout << "Virtual Key Code: " << (int)virtualKey << std::endl;
    std::cout << "Shift State: " << (int)shiftState << std::endl;

    result = VkKeyScanA('\x7f');
    virtualKey = LOBYTE(result);  // 低字节是虚拟键码
    shiftState = HIBYTE(result);  // 高字节是 Shift 状态

    std::cout << "Character: '\\x7f' (Delete)" << std::endl;
    std::cout << "Virtual Key Code: " << (int)virtualKey << std::endl;
    std::cout << "Shift State: " << (int)shiftState << std::endl;

    simulateChar('\x7f');
    simulateChar('\n');
    simulateChar('\r');

    std::cout << key_num['>'] << std::endl;
    simulateChar('>');
    
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
        }
        SLEEP(type_interval);
    }
    simulateChar('\n');
}

inline int AutoTyper::countLeadingSpace(const char* str) { // assume only ' ' no '\t'
    int cnt = 0;
    while (str[cnt] == ' ') cnt++;
    return cnt;
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


/*=== OS Concerned ===*/

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

    /* Common Porcess */
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

inline void AutoTyper::keyPress(KeyCode key) {
    keybd_event(key, 0, 0, 0);
}

inline void AutoTyper::keyRelease(KeyCode key) {
    keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
}
