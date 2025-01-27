#include "AutoTyper.h"

#include <fstream>


bool AutoTyper::init() {
#ifdef __linux__
    display = XOpenDisplay(NULL);
    if (!display) {
        cerr << "[Error] Failed to open X display" << endl;
        return false;
    }
#endif

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


/* Private Function */

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

/* OS Concerned */
void AutoTyper::simulateChar(char ch) {
    SHORT keysym = VkKeyScanA(ch);
    if (keysym == -1) {
        std::cout << "Character '" << ch << "' cannot be mapped to a virtual key.\n";
        return;
    }
    KeyCode key = keysym & 0xff;
    bool has_modifier = (keysym >> 8) & 0x1; // TODO: other modifier (& 0xff) & 0x2/0x4 
    printf("ch = \'%c\', KeySym = %u, KeyCode = %u, press shift = %d\n", ch, keysym, key, has_modifier);

    idx = 0;
    if (has_modifier) simulateKey(VK_SHIFT, true);
    simulateKey(key, true);
    simulateKey(key, false);
    if (has_modifier) simulateKey(VK_SHIFT, false);

    SendInput(idx, input, sizeof(INPUT));
}

inline void AutoTyper::simulateKey(KeyCode key, bool press) {
    input[idx].type = INPUT_KEYBOARD;
    input[idx].ki.wVk = key;
    (!press) && (input[idx].ki.dwFlags = KEYEVENTF_KEYUP);
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
