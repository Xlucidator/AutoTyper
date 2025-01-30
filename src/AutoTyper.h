#ifndef AUTOTYPER_H
#define AUTOTYPER_H

#include <iostream>
#include <string>

#ifdef _WIN32

#include <windows.h>

using KeySym = UINT; // Virtual-Key Code
using KeyCode = BYTE; // Scan Code
#define USLEEP(utime) Sleep(utime/1000.0)
constexpr auto ESCAPE_KEY = VK_ESCAPE;

#elif __linux__

#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>

#include <unordered_map>

using KeySym = ::KeySym; // X11 KeySym
using KeyCode = ::KeyCode; // X11 KeyCode
#define USLEEP(utime) usleep(utime)
constexpr auto ESCAPE_KEY = XK_Escape;

#endif

class AutoTyper {
public:
    AutoTyper()  { init();  }
    ~AutoTyper() { clear(); }

    bool init();
    bool type(const std::string& file_path);
    bool clear();

    void debug();
private:
    void processLine(const char* str);
    void simulateChar(char ch);
    
    inline bool getKeyFromChar(char ch, KeyCode& key, KeyCode& modifier);
    inline void simulateKey(KeyCode key, bool press);
    inline bool isKeyPressed(KeySym keysym);
    inline void flushDisplay();

#ifdef _WIN32
    INPUT input[5] = {};
    int idx = 0;

#elif __linux__
    Display* display = nullptr;
    std::unordered_map<KeySym, bool> keysymNeedModifier;

    bool initModifier();
#endif
};

/* Custom Char to Map Key */
#define CHAR_LEFT    0x80
#define CHAR_RIGHT   0x81
#define CHAR_UP      0x82
#define CHAR_DOWN    0x83


#endif // AUTOTYPER_H