#include "INIParser.h"
#include "AutoTyper.h"
#include "config.h"

#include <iostream>

constexpr char sep = 
#ifdef _WIN32
    '\\';
#else
    '/';
#endif

inline std::string getFullPath(std::string& base, std::string& file) {
    if (base.empty()) { return file; }
    if (base.back() == sep) base.pop_back();
    return base + sep + file;
}

int main() {
    /*=== Set Configs ===*/
    initConfig();
    printConfig();
    
    /*=== AutoTyper ===*/
    AutoTyper typer;
    std::string full_path = getFullPath(io_basedir, io_srcfile);
    typer.type(full_path);
    // typer.debug();

    return 0;
}