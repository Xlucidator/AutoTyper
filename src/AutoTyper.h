#ifndef AUTOTYPER_H
#define AUTOTYPER_H

#include <iostream>
#include <string>

#include "config.h"

#ifdef _WIN32

#include <windows.h>

using KeySym = UINT; // Virtual-Key Code
using KeyCode = BYTE; // Scan Code
#define SLEEP(utime) Sleep(utime/1000.0)

#elif __linux__

#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>

using KeySym = ::KeySym; // X11 KeySym
using KeyCode = ::KeyCode; // X11 KeyCode
#define SLEEP(utime) usleep(utime)

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
    inline int countLeadingSpace(const char* str);
    
    inline bool getKeyFromChar(char ch, KeyCode& key, KeyCode& modifier);
    inline void simulateKey(KeyCode key, bool press);
    inline void keyPress(KeyCode key);
    inline void keyRelease(KeyCode key);
    inline bool isKeyPressed(KeySym keysym);
    inline void flushDisplay();

#ifdef _WIN32
    INPUT input[5] = {};
    int idx = 0;

#elif __linux__
    Display* display = nullptr;
#endif
};

#endif // AUTOTYPER_H