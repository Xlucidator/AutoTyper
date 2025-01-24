#ifndef AUTO_TYPER_H
#define AUTO_TYPER_H

#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>

#include <iostream>
#include <unordered_map>

std::unordered_map<char, KeySym> char_to_keysyms = {
    {'~', XK_asciitilde},
    {'!', XK_exclam},
    {'@', XK_at},
    {'#', XK_numbersign},
    {'$', XK_dollar},
    {'%', XK_percent},
    {'^', XK_asciicircum},
    {'&', XK_ampersand},
    {'*', XK_asterisk},
    {'(', XK_parenleft},
    {')', XK_parenright},
    {'_', XK_underscore},
    {'+', XK_plus},
    {'-', XK_minus},
    {'=', XK_equal},
    {'{', XK_braceleft},
    {'}', XK_braceright},
    {'[', XK_bracketleft},
    {']', XK_bracketright},
    {'|', XK_bar},
    {'\\', XK_backslash},
    {':', XK_colon},
    {'"', XK_quotedbl},
    {';', XK_semicolon},
    {'\'', XK_apostrophe},
    {'<', XK_less},
    {'>', XK_greater},
    {'?', XK_question},
    {',', XK_comma},
    {'.', XK_period},
    {'/', XK_slash},
    {' ', XK_space},
    {'\n', XK_Return},
    {'\r', XK_Return},
    {'\t', XK_Tab},
};


inline KeySym CharToKeysym(char ch) {
    auto it = char_to_keysyms.find(ch);
    if (it != char_to_keysyms.end()) {
        return it->second;
    }
    return XStringToKeysym(&ch);
}

inline KeySym get_XStringToKeysym(char* ch) {
    KeySym keysym = XStringToKeysym(ch);
    if (keysym == 0) keysym = CharToKeysym(*ch);
    return keysym;
}

#endif // AUTO_TYPER_H