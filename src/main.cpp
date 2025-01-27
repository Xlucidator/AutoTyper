#include "INIParser.h"
#include "AutoTyper.h"
#include "config.h"

#include <iostream>

int main() {
    /*=== Set Configs ===*/
    initConfig();
    printConfig();
    
    /*=== AutoTyper ===*/
    AutoTyper typer;
    typer.type(io_srcfile);
    // typer.debug();

    return 0;
}