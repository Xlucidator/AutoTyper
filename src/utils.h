#ifndef UTILS_H
#define UTILS_H

inline int countLeadingSpace(const char* str) { // assume only ' ' no '\t'
    int cnt = 0;
    while (str[cnt] == ' ') cnt++;
    return cnt;
}

inline bool isBlank(char ch) {
    return ch == ' ' || ch == '\t';
}

inline bool isBlankTail(const char* str) {
    int i = 0;
    while (str[i] != '\0') if (!isBlank(str[i++])) return false;
    return true;
}

inline bool isBlankHead(const char* head, const char* start) {
    for (;start - head >= 0; start -= 1) if (!isBlank(start[0])) return false;
    return true;
}

#endif