#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>

#include <iostream>
#include <cstring>
#include <fstream>
#include <unistd.h>
#include <vector>

#include "AutoTyper.h"

using namespace std;

char test_letter[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
char test_number[] = "1234567890";
char test_symbol[] = "~!@#$%^&*()_+-={}[]|\\:\";\'<>?,./ ";
char test_escape[] = "\t\n\r";

unordered_map<KeySym, bool> keysymNeedModifier;

bool initModifier(Display* display) {
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

inline bool findModifier(const KeySym& keysym, KeySym& modifier) {
    auto it = keysymNeedModifier.find(keysym);
    if (it != keysymNeedModifier.end()) {
        modifier = XK_Shift_L;
        return it->second;
    }
    return false;
}

bool validate(Display *display, char* test_buf) {
    bool condition = true;
    char chbuf[2] = {0, 0}; int len = strlen(test_buf);
    for (int i = 0; i < len; ++i) {
        chbuf[0] = test_buf[i];
        KeySym keysym = get_XStringToKeysym(chbuf);
        KeyCode keycode = XKeysymToKeycode(display, keysym);
        printf("ch = \'%c\', KeySym = %lu, KeyCode = %u\n", chbuf[0], keysym, keycode);
        if (keysym == 0 && keycode == 204) condition = false;
    }
    return condition;
}

inline KeySym getKeysymFromChar(char ch) {
    /* Exception
       ch | ascii |   Name   | Value
       \n |  0xa  | Linefeed | 0xff0a  <- Special, change to XK_Return (0xff0d)
       \r |  0xd  | Return   | 0xff0d */ 
    if (ch == '\n') return XK_Return;
    return (0 <= ch && ch < 0x20) ? (0xff00) | ((int) ch) : ((int) ch);
}

void simulateKey(Display *display, KeySym key, bool press) {
    KeyCode keycode = XKeysymToKeycode(display, key);
    if (keycode == 0) return; // Invalid
    XTestFakeKeyEvent(display, keycode, press, CurrentTime);
}

void simulateInputFromChar(Display *display, char ch) {
    KeySym keysym = getKeysymFromChar(ch), modifier;
    bool has_modifier = findModifier(keysym, modifier);
    printf("ch = \'%c\', KeySym = %lu, KeyCode = %u, press shift = %d\n", ch, keysym, XKeysymToKeycode(display, keysym), has_modifier);

    if (has_modifier) simulateKey(display, modifier, true);
    simulateKey(display, keysym, true);
    simulateKey(display, keysym, false);
    if (has_modifier) simulateKey(display, modifier, false);

    XFlush(display);
}

bool isKeyPressed(Display *display, KeySym key) {
    char keys[32];
    XQueryKeymap(display, keys);
    KeyCode keycode = XKeysymToKeycode(display, key);
    // printf("isKeyPressed: %d\n", keycode);
    return keys[keycode / 8] & (1 << (keycode % 8));
}

int main() {
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        cerr << "Unable to open X display" << endl;
        return 1;
    }

    initModifier(display);

    while (true) {
        if (isKeyPressed(display, XK_Escape)) break; // ESC 键退出

        printf("[Validate]\n");
        bool status[4] = {
            validate(display, test_letter),
            validate(display, test_number),
            validate(display, test_symbol),
            validate(display, test_escape)
        };
        if (!(status[0] && status[1] && status[2] && status[3])) {
            printf("Status: Letter | Number | Symbol | Escape = %d %d %d %d\n", 
                status[0], status[1], status[2], status[3]);
            printf("Check failed! Exit\n");
            break; 
        }

        sleep(3);
        printf("[Start]\n");

        ifstream file("target.cpp");
        if (!file.is_open()) {
            cerr << "Error opening target.cpp" << endl;
            continue;
        }
        
        printf("[Typing...]\n");
        char chbuf[2] = {0, 0};
        while (file.get(chbuf[0])) {
            if (isKeyPressed(display, XK_Escape)) break;
            simulateInputFromChar(display, chbuf[0]);
            usleep(5000); // 延时 0.5 毫秒
        }
        

        file.close();
        break;
    }

    XCloseDisplay(display);
    return 0;
}
