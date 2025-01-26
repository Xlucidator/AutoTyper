#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <iostream>

bool isKeySymShifted(KeySym* keymap, int min_keycode, int max_keycode, int keysyms_per_keycode, KeySym keysym) {
    for (int kc = min_keycode; kc <= max_keycode; ++kc) {
        for (int i = 0; i < keysyms_per_keycode; ++i) {
            if (keymap[(kc - min_keycode) * keysyms_per_keycode + i] == keysym) {
                return i > 0; // 如果 KeySym 不在第 0 列，说明需要修饰键
            }
        }
    }
    return false; // 未找到 KeySym
}

void printKeymap(KeySym* keymap, int min_keycode, int max_keycode, int keysyms_per_keycode) {
    std::cout << "Keymap: " << std::endl;
    for (int kc = min_keycode; kc <= max_keycode; ++kc) {
        std::cout << "KeyCode " << kc << ": ";
        for (int i = 0; i < keysyms_per_keycode; ++i) {
            KeySym keysym = keymap[(kc - min_keycode) * keysyms_per_keycode + i];
            const char* keysym_name = XKeysymToString(keysym);
            std::cout << (keysym_name ? keysym_name : "None") << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    Display* display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "Unable to open X display!" << std::endl;
        return 1;
    }

    int min_keycode, max_keycode;
    XDisplayKeycodes(display, &min_keycode, &max_keycode);
    int keysyms_per_keycode;
    KeySym* keymap = XGetKeyboardMapping(display, min_keycode, max_keycode - min_keycode + 1, &keysyms_per_keycode);

    // printKeymap(keymap, min_keycode, max_keycode, keysyms_per_keycode);

    // 示例：检测 'A' 是否需要修饰键
    KeySym keysym = XK_a; // 'A'
    if (isKeySymShifted(keymap, min_keycode, max_keycode, keysyms_per_keycode, keysym)) {
        std::cout << "KeySym " << XKeysymToString(keysym) << " requires a modifier key (e.g., Shift)." << std::endl;
    } else {
        std::cout << "KeySym " << XKeysymToString(keysym) << " does not require a modifier key." << std::endl;
    }

    printf("0x%x\n", XkbCharToInt('\n'));

    XFree(keymap);
    XCloseDisplay(display);
    return 0;
}