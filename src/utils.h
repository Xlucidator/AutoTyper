#ifndef UTILS_H
#define UTILS_H

inline int countLeadingSpace(const char* str) { // assume only ' ' no '\t'
    int cnt = 0;
    while (str[cnt] == ' ') cnt++;
    return cnt;
}

#endif